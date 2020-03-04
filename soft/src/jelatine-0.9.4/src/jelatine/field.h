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

/** \file field.h
 * Field structure and interface */

/** \def JELATINE_FIELD_H
 * class.h inclusion macro */

#ifndef JELATINE_FIELD_H
#   define JELATINE_FIELD_H (1)

#include "wrappers.h"

#include "classfile.h"

/******************************************************************************
 * Field types definitions                                                    *
 ******************************************************************************/

/** \struct field_t
 * Internal field representation */

struct field_t {
    char *name; ///< Name pointer
    char *descriptor; ///< Descriptor pointer
    uint16_t access_flags; ///< Access flags
    int16_t offset; ///< Offset from the instance base
};

/** Typedef for ::struct field_t */
typedef struct field_t field_t;

/** \struct static_field_t
 * Internal byte-sized static field representation
 *
 * Note that the memory reserved for the actual field  */

struct static_field_t {
    char *name; ///< Name pointer
    char *descriptor; ///< Descriptor pointer
    uint16_t access_flags; ///< Access flags
    union {
        int8_t byte_data;
        uint16_t char_data;
        int16_t short_data;
        int32_t int_data;
        int64_t long_data;
#if JEL_FP_SUPPORT
        float float_data;
        double double_data;
#endif // JEL_FP_SUPPORT
        uintptr_t reference_data;
    } field; ///< Data used by the static field
};

/** Typedef for ::struct static_field_t */
typedef struct static_field_t static_field_t;

/** Representation of a field attributes */

struct field_attributes_t {
    bool constant_value_found; ///< True when 'ConstantValue' attribute is found
    uint16_t constant_value_index; ///< Index of the constant value
};

/** Typedef for ::struct field_attributes_t */
typedef struct field_attributes_t field_attributes_t;

/******************************************************************************
 * Field interface                                                            *
 ******************************************************************************/

extern void check_field_access_flags(uint16_t, bool);
extern void *sfield_get_data_ptr(static_field_t *);

/******************************************************************************
 * Field inlined functions                                                    *
 ******************************************************************************/

/** Checks if a static field is private
 * \param field A pointer to a static field
 * \returns true if the field is private, false otherwise */

static inline bool sfield_is_private(static_field_t *field)
{
    return field->access_flags & ACC_PRIVATE;
} // sfield_is_private()

/** Checks if a static field is protected
 * \param field A pointer to a static field
 * \returns true if the field is protected */

static inline bool sfield_is_protected(static_field_t *field)
{
    return field->access_flags & ACC_PROTECTED;
} // sfield_is_protected()

/** Checks if a static field is public
 * \param field A pointer to a static field
 * \returns true if the field is public, false otherwise */

static inline bool sfield_is_public(static_field_t *field)
{
    return field->access_flags & ACC_PUBLIC;
} // sfield_is_public()

/**  Checks if a field is private
 * \param field A pointer to a field_t structure
 * \returns true if the field is private, false otherwise */

static inline bool field_is_private(field_t *field)
{
    return field->access_flags & ACC_PRIVATE;
} // field_is_private()

/**  Checks if a field is protected
 * \param field A pointer to a field_t structure
 * \returns true if the field is protected, false otherwise */

static inline bool field_is_protected(field_t *field)
{
    return field->access_flags & ACC_PROTECTED;
} // field_is_protected()

/**  Checks if a field is public
 * \param field A pointer to a field_t structure
 * \returns true if the field is public, false otherwise */

static inline bool field_is_public(field_t *field)
{
    return field->access_flags & ACC_PUBLIC;
} // field_is_public()

/******************************************************************************
 * Field manager type definition                                              *
 ******************************************************************************/

/** Represents the field manager of a class */

struct field_manager_t {
#ifndef NDEBUG
    uint32_t reserved_instance; ///< Number of instance fields at creation time
    uint32_t reserved_static; ///< Number of static fields at creation time
#endif // !NDEBUG
    uint32_t instance_count; ///< Number of instance fields
    field_t *instance_fields; ///< Instance fields descriptors
    uint32_t static_count; ///< Number of static fields
    static_field_t *static_fields; ///< Static fields descriptors
};

/** Typedef for ::struct field_manager_t */
typedef struct field_manager_t field_manager_t;

/******************************************************************************
 * Field manager interface                                                    *
 ******************************************************************************/

extern field_manager_t *fm_create(uint32_t, uint32_t);
extern void fm_add_instance(field_manager_t *, char *, char *, uint16_t);
void fm_add_static(field_manager_t *, char *, char *, uint16_t, const_pool_t *,
                   uint16_t);
extern field_t *fm_get_instance(field_manager_t *, const char *, const char *);
extern static_field_t *fm_get_static(field_manager_t *, const char *,
                                     const char *);
extern void fm_mark_static(field_manager_t *);

/******************************************************************************
 * Field iterator                                                             *
 ******************************************************************************/

/** Instance field iterator, usually allocated on the stack when used */

struct field_iterator_t {
    field_t *fields; ///< Pointer to the instance fields of a class
    size_t entries; ///< Number of fields
    size_t index; ///< Current index in the fields array
};

/** Typedef for the ::struct field_iterator_t type */
typedef struct field_iterator_t field_iterator_t;

/** Creates an instance field iterator from the field manager \a fm
 * \param fm A pointer to a class' field manager
 * \returns An initialized instance field iterator */

static inline field_iterator_t field_itr(field_manager_t *fm)
{
    field_iterator_t itr;

    itr.fields = fm->instance_fields;
    itr.entries = fm->instance_count;
    itr.index = 0;

    return itr;
} // instance_field_itr()

/** Returns true if another instance field is available
 * \param itr A field iterator
 * \returns true if there are more fields, false otherwise */

static inline bool field_itr_has_next(field_iterator_t itr)
{
    return itr.index < itr.entries;
} // field_itr_has_next()

/** Returns the next instance field, if the iterator has just been
 * created it returns the data of the first field (if it is present)
 * \param itr A pointer to the field iterator
 * \returns A pointer to an instance field */

static inline field_t *field_itr_get_next(field_iterator_t *itr)
{
    assert(field_itr_has_next(*itr));

    return itr->fields + itr->index++;
} // field_itr_get_next()

#endif // !JELATINE_FIELD_H
