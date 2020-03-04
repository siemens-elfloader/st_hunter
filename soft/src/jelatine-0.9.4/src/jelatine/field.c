/***************************************************************************
 *   Copyright © 2005-2009 by Gabriele Svelto                              *
 *   gabriele.svelto@gmail.com                                             *
 *                                                                         *
 *   This file is part of Jelatine.                                        *
 *                                                                         *
 *   Jelatine is free software: you can redistribute it and/or modify      *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   Jelatine is distributed in the hope that it will be useful,           *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with Jelatine.  If not, see <http://www.gnu.org/licenses/>.     *
 ***************************************************************************/

/** \file field.c
 * Field structure implementation */

#include "wrappers.h"

#include "class.h"
#include "classfile.h"
#include "constantpool.h"
#include "field.h"
#include "memory.h"
#include "utf8_string.h"

#include "java_lang_ref_Reference.h"
#include "java_lang_ref_WeakReference.h"
#include "java_lang_String.h"
#include "java_lang_Thread.h"
#include "java_lang_Throwable.h"
#include "jelatine_VMResourceStream.h"

/******************************************************************************
 * Field implementation                                                       *
 ******************************************************************************/

/** Checks if a field descriptor is valid
 * \param field A pointer to a field */

static void parse_field_descriptor(const char *desc)
{
    uint32_t i, j; // A couple of counters
    uint32_t dimensions; // Dimensions of an array

    i = 0;

    if (desc[0] == 0) {
        goto error;
    }

    switch (desc[i]) {
        case 'B':
        case 'C':
#if JEL_FP_SUPPORT
        case 'D':
        case 'F':
#endif // JEL_FP_SUPPORT
        case 'I':
        case 'J':
        case 'S':
        case 'Z':
            i++;
            goto basic;

        case 'L':
            j = i + 1;

            while (desc[j] != ';' && desc[j] != 0) {
                j++;
            }

            if (desc[j] == 0) {
                goto error;
            }

            return;

        case '[':
            dimensions = 0;

            while (desc[i] == '[') {
                i++;
                dimensions++;

                if ((desc[i] == 0) || dimensions > 255) {
                    goto error;
                }
            }

            switch (desc[i]) {
                case 'B':
                case 'C':
#if JEL_FP_SUPPORT
                case 'D':
                case 'F':
#endif // JEL_FP_SUPPORT
                case 'I':
                case 'J':
                case 'S':
                case 'Z':
                    if (desc[i + 1] != 0) {
                        goto error;
                    }

                    // Basic array class
                    return;

                case 'L':
                    while (desc[i] != ';' && desc[i] != 0) {
                        i++;
                    }

                    if (desc[i] == 0) {
                        goto error;
                    }

                    return;

                default:
                    goto error;
            }

        default:
            goto error;
    }

basic:
    if (desc[i] == 0) {
        return;
    }

error:
    c_throw(JAVA_LANG_NOCLASSDEFFOUNDERROR, "Invalid field descriptor");
} // parse_field_descriptor()

/** Check that a field's access flag are coherent, throws an exception upon
 * failure
 * \param access_flags A field's access flags
 * \param interface True if the owning class is an interface */

void check_field_access_flags(uint16_t access_flags, bool interface)
{
    if (((access_flags & ACC_PUBLIC) && (access_flags & ACC_PROTECTED))
        || ((access_flags & ACC_PUBLIC) && (access_flags & ACC_PRIVATE))
        || ((access_flags & ACC_PROTECTED) && (access_flags & ACC_PRIVATE))
        || ((access_flags & ACC_FINAL) && (access_flags & ACC_VOLATILE)))
    {
        c_throw(JAVA_LANG_NOCLASSDEFFOUNDERROR, "Illegal field's access flags");
    }

    if (interface) {
        if (!((access_flags & ACC_PUBLIC) && (access_flags & ACC_STATIC)
            && (access_flags & ACC_FINAL)))
        {
            c_throw(JAVA_LANG_NOCLASSDEFFOUNDERROR,
                    "Interface has non-static, public, final field");
        }

        if (access_flags & ACC_TRANSIENT) {
            c_throw(JAVA_LANG_NOCLASSDEFFOUNDERROR,
                    "Interface field has ACC_TRANSIENT access flag set");
        }
    }
} // check_field_access_flags()

/** Returns a pointer to the actual data of a static field
 * \param field A pointer to a static field
 * \returns The address of the static field data */

void *sfield_get_data_ptr(static_field_t *field)
{
    switch (field->descriptor[0]) {
        case 'L': // FALLTHROUGH
        case '[': return &(field->field.reference_data);
        case 'B': // FALLTHROUGH
        case 'Z': return &(field->field.byte_data);
        case 'C': return &(field->field.char_data);
        case 'S': return &(field->field.short_data);
        case 'I': return &(field->field.int_data);
        case 'J': return &(field->field.long_data);
#if JEL_FP_SUPPORT
        case 'F': return &(field->field.float_data);
        case 'D': return &(field->field.double_data);
#endif // JEL_FP_SUPPORT
        default:
            dbg_unreachable();
            return NULL;
    }
} // sfield_get_data_ptr()

/******************************************************************************
 * Field manager implementation                                               *
 ******************************************************************************/

/** Creates the field manager
 * \param ic The number of instance fields which the manager will hold
 * \param sc The number of static fields which the manager will hold
 * \returns A pointer to a newly created field manager */

field_manager_t *fm_create(uint32_t ic, uint32_t sc)
{
    field_manager_t *fm = gc_palloc(sizeof(field_manager_t));

#ifndef NDEBUG
    fm->reserved_instance = ic;
    fm->reserved_static = sc;
#endif // !NDEBUG

    fm->instance_fields = gc_palloc(ic * sizeof(field_t));
    fm->static_fields = gc_palloc(sc * sizeof(static_field_t));

    return fm;
} // fm_create()

/** Adds an instance field to the manager
 * \param fm The field manager
 * \param name The field name
 * \param descriptor The field descriptor
 * \param access_flags The field access flags */

void fm_add_instance(field_manager_t *fm, char *name, char *descriptor,
                     uint16_t access_flags)
{
    assert(fm->instance_count < fm->reserved_instance);

    field_t *field = fm->instance_fields + fm->instance_count;

    if ((fm_get_instance(fm, name, descriptor) != NULL)
        || (fm_get_static(fm, name, descriptor) != NULL))
    {
        c_throw(JAVA_LANG_NOCLASSDEFFOUNDERROR,
                "Duplicated fields with same name and descriptor");
    }

    parse_field_descriptor(descriptor);
    field->name = name;
    field->descriptor = descriptor;
    field->access_flags = access_flags;
    fm->instance_count++;
} // fm_add_instance()

/** Adds a static field to the manager
 * \param fm The field manager
 * \param name The field name
 * \param descriptor The field descriptor
 * \param access_flags The field access flags
 * \param cp A pointer to the constant pool
 * \param const_index The index of the constant field value, 0 if the field is
 * not constant */

void fm_add_static(field_manager_t *fm, char *name, char *descriptor,
                   uint16_t access_flags, const_pool_t *cp,
                   uint16_t const_index)
{
    assert(fm->static_count < fm->reserved_static);

    static_field_t *curr;

    if ((fm_get_instance(fm, name, descriptor) != NULL)
        || (fm_get_static(fm, name, descriptor) != NULL))
    {
        c_throw(JAVA_LANG_NOCLASSDEFFOUNDERROR,
                "Duplicated fields with same name and descriptor");
    }

    parse_field_descriptor(descriptor);
    curr = fm->static_fields + fm->static_count;

    curr->name = name;
    curr->descriptor = descriptor;
    curr->access_flags = access_flags;

    if (const_index != 0) {
        switch (descriptor[0]) {
            case '[':
            case 'L': // This is a final static field pointing to a string
                if (strcmp(descriptor, "Ljava/lang/String;") != 0) {
                    c_throw(JAVA_LANG_NOCLASSDEFFOUNDERROR,
                            "A final static reference with a constant "
                            "initializer does not point to a java.lang.String "
                            "object");
                }

                curr->field.reference_data = cp_get_ref(cp, const_index);
                break;

            case 'B': // This is a final static byte field
                curr->field.byte_data = cp_get_integer(cp, const_index);
                break;

            case 'Z': // This is a final static bool field
                if (cp_get_integer(cp, const_index)) {
                    curr->field.byte_data = 1;
                }

                break;

            case 'C': // This is a final static char field
                curr->field.char_data = cp_get_integer(cp, const_index);
                break;

            case 'S': // This is a final static short field
                curr->field.short_data = cp_get_integer(cp, const_index);
                break;

            case 'I': // This is a final static float field
                curr->field.int_data = cp_get_integer(cp, const_index);
                break;

#if JEL_FP_SUPPORT
            case 'F': // This is a final static float field
                curr->field.float_data = cp_get_float(cp, const_index);
                break;
#endif // JEL_FP_SUPPORT

            case 'J': // This is a final static long field
                curr->field.long_data = cp_get_long(cp, const_index);
                break;

#if JEL_FP_SUPPORT
            case 'D': // This is a final static double field
                curr->field.double_data = cp_get_double(cp, const_index);
                break;
#endif // JEL_FP_SUPPORT

            default:
                dbg_unreachable();
        }
    }

    fm->static_count++;
} // fm_add_static()

/** Finds an instance field by its name and descriptor
 * \param fm A pointer to the field manager
 * \param name The field name
 * \param descriptor The descriptor name
 * \returns A pointer to the field or NULL */

field_t *fm_get_instance(field_manager_t *fm, const char *name,
                         const char *descriptor)
{
    field_t *fields = fm->instance_fields;
    uint32_t count = fm->instance_count;

    for (size_t i = 0; i < count; i++) {
        if ((strcmp(fields[i].name, name) == 0)
            && (strcmp(fields[i].descriptor, descriptor) == 0))
        {
            return fields + i;
        }
    }

    return NULL;
} // fm_get_instance()

/** Finds a static field by its name and descriptor
 * \param fm A pointer to the field manager
 * \param name The field name
 * \param descriptor The descriptor name
 * \returns A pointer to the field or NULL */

static_field_t *fm_get_static(field_manager_t *fm, const char *name,
                              const char *descriptor)
{
    static_field_t *fields = fm->static_fields;
    uint32_t count = fm->static_count;

    for (size_t i = 0; i < count; i++) {
        if ((strcmp(fields[i].name, name) == 0)
            && (strcmp(fields[i].descriptor, descriptor) == 0))
        {
            return fields + i;
        }
    }

    return NULL;
} // fm_get_static()

/** Marks the objects pointed by the static fields of a class
 * \param fm A pointer to the class' field manager */

void fm_mark_static(field_manager_t *fm)
{
    static_field_t *curr = fm->static_fields;

    for (size_t i = 0; i < fm->static_count; i++) {
        if ((curr->descriptor[0] == '[') || (curr->descriptor[0] == 'L')) {
            gc_mark_reference(curr->field.reference_data);
        }

        curr++;
    }
} // fm_mark_static()
