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

/** \file vm.h
 * Virtual machine definition and interface */

/** \def JELATINE_VM_H
 * vm.h inclusion macro */

#ifndef JELATINE_VM_H
#   define JELATINE_VM_H (1)

#include "wrappers.h"
#include "class.h"
#include "memory.h"
#include "thread.h"

/******************************************************************************
 * Virtual machine interface                                                  *
 ******************************************************************************/

extern void vm_run(const char *, const char *, char *, size_t, int, char **);

#if JEL_TRACE
extern bool vm_opt_trace_bytecodes( void );
extern bool vm_opt_trace_methods( void );
#endif // JEL_TRACE

#if JEL_PRINT
extern bool vm_opt_print_bytecodes( void );
extern bool vm_opt_print_methods( void );
#endif // JEL_PRINT

/******************************************************************************
 * Inlined virtual machine functions                                          *
 ******************************************************************************/

/** Terminates abruptly the VM */

static inline void vm_fail( void )
{
    // TODO: This should be improved
    exit(1);
} // vm_fail()

/******************************************************************************
 * Options type definitions                                                   *
 ******************************************************************************/

/** Virtual machine options structure, holds the global options of the virtual
 * machine obtained by parsing the command-line */

struct options_t {
    char *classpath; ///< Default classpath
    char *boot_classpath; ///< Default classpath for system classes
    size_t heap_size; ///< Heap size
    size_t stack_size; ///< Size of a thread's stack
    char *main_class; ///< Name of the class holding the main() method
    char **jargs; ///< Arguments of the Java application
    int jargs_n; ///< Number of arguments

#if JEL_TRACE
    bool trace_methods; ///< True if method tracing is enabled
    bool trace_opcodes; ///< True if opcode tracing is enabled
#endif // JEL_TRACE

#if JEL_PRINT
    bool print_methods; ///< True if method printing is enabled
    bool print_opcodes; ///< True if opcode printing is enabled
    bool print_memory; ///< True if memory operations printing is enabled
#endif // JEL_PRINT

    bool version; ///< True if the machine should print its version number
    bool help; ///< True if the machines should print the help notice
};

/** Typedef for the ::struct options_t */
typedef struct options_t options_t;

extern options_t options;

/******************************************************************************
 * Virtual machine options interface                                          *
 ******************************************************************************/

/** Sets the global option 'classpath'
 * \param classpath A pointer to a string holding the new value of the
 * classpath */

void opts_set_classpath(char *classpath);

/** Gets the global option 'classpath'
 * \returns The classpath string */

char *opts_get_classpath( void );

/** Sets the global option 'default classpath'
 * \param boot_classpath A pointer to a string holding the new value of the
 * boot classpath */

void opts_set_boot_classpath(char *boot_classpath);

/** Gets the global option 'boot classpath'
 * \returns The boot classpath string */

char *opts_get_boot_classpath( void );

/** Sets the global option 'heap size'
 * \param size The maximum size of the heap */

void opts_set_heap_size(size_t size);

/** Gets the global option 'heap size'
 * \returns The maximum size of the heap */

size_t opts_get_heap_size( void );

/** Sets the global option 'stack size'
 * \param size The default size of a thread's stack */

void opts_set_stack_size(size_t size);

/** Gets the global option 'stack size'
 * \returns size The default size of a thread's stack */

size_t opts_get_stack_size( void );

/** Sets the global option 'main class'
 * \param class_name A string holding the name of the class which main method
 * should be called by the virtual machine */

void opts_set_main_class(char *class_name);

/** Gets the global option 'main class'
 * \returns A string holding the name of the main class */

char *opts_get_main_class( void );

/** Sets the global option 'jargs'
 * \param jargs A pointer to an array of strings holding the parameters passed
 * to the Java application */

void opts_set_jargs(char **jargs);

/** Gets the global option 'jargs'
 * \returns An array of strings holding the arguments passed to the Java
 * application */

char **opts_get_jargs( void );

/** Sets the global option 'jargs_n'
 * \param n The number of arguments passed to the Java application */

void opts_set_jargs_n(int n);

/** Gets the global option 'jargs_n'
 * \returns The number of arguments passed to the Java application */

int opts_get_jargs_n( void );

#if JEL_TRACE

/** Sets the global option 'trace methods'
 * \param enable true if method tracing must be enabled, false otherwise */

void opts_set_trace_methods(bool enable);

/** Gets the global option 'trace methods'
  * \returns true if method tracing must be enabled, false otherwise */

bool opts_get_trace_methods( void );

/** Sets the global option 'trace opcodes'
 * \param enable true if opcode tracing must be enabled, false otherwise */

void opts_set_trace_opcodes(bool enable);

/** Gets the global option 'trace opcodes'
 * \returns true if opcode tracing must be enabled, false otherwise */

bool opts_get_trace_opcodes( void );

#endif // JEL_TRACE

#if JEL_PRINT
/** Sets the global option 'print methods'
 * \param enable true if method printing must be enabled, false otherwise */

void opts_set_print_methods(bool enable);

/** Gets the global option 'print methods'
 * \returns true if method printing must be enabled, false otherwise */

bool opts_get_print_methods( void );

/** Sets the global option 'print opcodes'
 * \param enable true if opcode printing must be enabled, false otherwise */

void opts_set_print_opcodes(bool enable);

/** Gets the global option 'print opcodes'
 * \returns true if method tracing must be enabled, false otherwise */

bool opts_get_print_opcodes( void );

/** Sets the global option 'print memory'
 * \param enable true if memory printing must be enabled, false otherwise */

void opts_set_print_memory(bool enable);

/** Gets the global option 'print memory'
 * \returns true if memory printing must be enabled, false otherwise */

bool opts_get_print_memory( void );

#endif // JEL_PRINT

/** Sets the global option 'version'
 * \param enable true if version information must be displayed, false
 * otherwise */

void opts_set_version(bool enable);

/** Gets the global option 'version'
 * \returns true if version information must be displayed, false otherwise */

bool opts_get_version( void );

/** Sets the global option 'help'
 * \param true if help information must be displayed, false otherwise */

void opts_set_help(bool enable);

/** Gets the global option 'help'
 * \returns true if help information must be displayed, false otherwise */

bool opts_get_help( void );

#endif // !JELATINE_VM_H
