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

/** \file vm.c
 * Virtual machine object implementation */

#include "wrappers.h"

#include "class.h"
#include "jstring.h"
#include "interpreter.h"
#include "loader.h"
#include "memory.h"
#include "method.h"
#include "thread.h"
#include "utf8_string.h"
#include "util.h"
#include "vm.h"

#include "java_lang_Thread.h"

/******************************************************************************
 * Virtual machine related type definitions                                   *
 ******************************************************************************/

/** Represents the manager used for handling threads */

struct virtual_machine_t {
    // Tracing and printing facilities
#if JEL_TRACE
    bool trace_bytecodes; ///< True if bytecode tracing is enabled
    bool trace_methods; ///< True if method tracing is enabled
#endif // JEL_TRACE
#if JEL_PRINT
    bool print_bytecodes; ///< True if bytecode printing is enabled
    bool print_methods; ///< True if method printing is enabled
#endif // JEL_PRINT
};

/** Typedef for the ::struct virtual_machine_t */
typedef struct virtual_machine_t virtual_machine_t;

/******************************************************************************
 * Virtual machine options interface                                          *
 ******************************************************************************/

/** Sets the global option 'classpath'
 * \param classpath A pointer to a string holding the new value of the
 * classpath */

void opts_set_classpath(char *classpath)
{
    options.classpath = classpath;
} // opts_set_classpath()

/** Gets the global option 'classpath'
 * \returns The classpath string */

char *opts_get_classpath( void )
{
    return options.classpath;
} // opts_get_classpath()

/** Sets the global option 'default classpath'
 * \param boot_classpath A pointer to a string holding the new value of the
 * boot classpath */

void opts_set_boot_classpath(char *boot_classpath)
{
    options.boot_classpath = boot_classpath;
} // opts_set_boot_classpath()

/** Gets the global option 'boot classpath'
 * \returns The boot classpath string */

char *opts_get_boot_classpath( void )
{
    return options.boot_classpath;
} // opts_get_boot_classpath()

/** Sets the global option 'heap size'
 * \param size The maximum size of the heap */

void opts_set_heap_size(size_t size)
{
    options.heap_size = size;
} // opts_set_heap_size()

/** Gets the global option 'heap size'
 * \returns The maximum size of the heap */

size_t opts_get_heap_size( void )
{
    return options.heap_size;
} // opts_get_heap_size()

/** Sets the global option 'stack size'
 * \param size The default size of a thread's stack */

void opts_set_stack_size(size_t size)
{
    options.stack_size = size;
} // opts_set_stack_size()

/** Gets the global option 'stack size'
 * \returns size The default size of a thread's stack */

size_t opts_get_stack_size( void )
{
    return options.stack_size;
} // opts_get_stack_size()

/** Sets the global option 'main class'
 * \param class_name A string holding the name of the class which main method
 * should be called by the virtual machine */

void opts_set_main_class(char *class_name)
{
    options.main_class = class_name;
} // opts_set_main_class()

/** Gets the global option 'main class'
 * \returns A string holding the name of the main class */

char *opts_get_main_class( void )
{
    return options.main_class;
} // opts_get_main_class()

/** Sets the global option 'jargs'
 * \param jargs A pointer to an array of strings holding the parameters passed
 * to the Java application */

void opts_set_jargs(char **jargs)
{
    options.jargs = jargs;
} // opts_set_jargs()

/** Gets the global option 'jargs'
 * \returns An array of strings holding the arguments passed to the Java
 * application */

char **opts_get_jargs( void )
{
    return options.jargs;
} // opts_get_jargs()

/** Sets the global option 'jargs_n'
 * \param n The number of arguments passed to the Java application */

void opts_set_jargs_n(int n)
{
    options.jargs_n = n;
} // opts_set_jargs_n()

/** Gets the global option 'jargs_n'
 * \returns The number of arguments passed to the Java application */

int opts_get_jargs_n( void )
{
    return options.jargs_n;
} // opts_set_jargs_n()

#if JEL_TRACE

/** Sets the global option 'trace methods'
 * \param enable true if method tracing must be enabled, false otherwise */

void opts_set_trace_methods(bool enable)
{
    options.trace_methods = enable;
} // opts_set_trace_methods()

/** Gets the global option 'trace methods'
  * \returns true if method tracing must be enabled, false otherwise */

bool opts_get_trace_methods( void )
{
    return options.trace_methods;
} // opts_get_trace_methods()

/** Sets the global option 'trace opcodes'
 * \param enable true if opcode tracing must be enabled, false otherwise */

void opts_set_trace_opcodes(bool enable)
{
    options.trace_opcodes = enable;
} // opts_set_trace_opcodes()

/** Gets the global option 'trace opcodes'
 * \returns true if opcode tracing must be enabled, false otherwise */

bool opts_get_trace_opcodes( void )
{
    return options.trace_opcodes;
} // opts_get_trace_opcodes()

#endif // JEL_TRACE

#if JEL_PRINT
/** Sets the global option 'print methods'
 * \param enable true if method printing must be enabled, false otherwise */

void opts_set_print_methods(bool enable)
{
    options.print_methods = enable;
} // opts_set_print_methods()

/** Gets the global option 'print methods'
 * \returns true if method printing must be enabled, false otherwise */

bool opts_get_print_methods( void )
{
    return options.print_methods;
} // opts_get_print_methods()

/** Sets the global option 'print opcodes'
 * \param enable true if opcode printing must be enabled, false otherwise */

void opts_set_print_opcodes(bool enable)
{
    options.print_opcodes = enable;
} // opts_set_print_opcodes()

/** Gets the global option 'print opcodes'
 * \returns true if method tracing must be enabled, false otherwise */

bool opts_get_print_opcodes( void )
{
    return options.print_opcodes;
} // opts_get_print_opcodes()

/** Sets the global option 'print memory'
 * \param enable true if memory printing must be enabled, false otherwise */

void opts_set_print_memory(bool enable)
{
    options.print_memory = enable;
} // opts_set_print_memory()

/** Gets the global option 'print memory'
 * \returns true if memory printing must be enabled, false otherwise */

bool opts_get_print_memory( void )
{
    return options.print_memory;
} // opts_get_memory()

#endif // JEL_PRINT

/** Sets the global option 'version'
 * \param enable true if version information must be displayed, false
 * otherwise */

void opts_set_version(bool enable)
{
    options.version = enable;
} // opts_set_version()

/** Gets the global option 'version'
 * \returns true if version information must be displayed, false otherwise */

bool opts_get_version( void )
{
    return options.version;
} // opts_get_version

/** Sets the global option 'help'
 * \param true if help information must be displayed, false otherwise */

void opts_set_help(bool enable)
{
    options.help = enable;
} // opts_set_help()

/** Gets the global option 'help'
 * \returns true if help information must be displayed, false otherwise */

bool opts_get_help( void )
{
    return options.help;
} // opts_get_help()

/******************************************************************************
 * Globals                                                                    *
 ******************************************************************************/

/** Global options of the VM */

options_t options = {
    NULL, // classpath
    JEL_CLASSPATH_DIR, // boot_classpath
    128 * 1024, // heap_size
    4096, // stack_size
    NULL, // main_class
    NULL, // jargs
    0, // jargs_n

#if JEL_TRACE
    false, // trace_methods
    false, // trace_opcodes
#endif // JEL_TRACE

#if JEL_PRINT
    false, // print_methods
    false, // print_opcodes
    false, // print_memory
#endif // JEL_PRINT

    false, // version
    false // help
};

/** Global virtual-machine pointer */
virtual_machine_t vm;

/******************************************************************************
 * Local function prototypes                                                  *
 ******************************************************************************/

static void vm_init(size_t, const char *, const char *);
static void vm_teardown( void );

/******************************************************************************
 * Virtual machine related functions implementation                           *
 ******************************************************************************/

/** Launches the JVM
 *
 * The \a main parameter used by vm_run() must be a class name in the Java
 * internal format (es: java/lang/Object), the class-path must end with a
 * trailing slash (es: /usr/java/classpath/)
 *
 * \param main Class containing the main() method using the internal name format
 * \param jargc Number of arguments to be passed to the JVM
 * \param jargv Actual arguments passed to the JVM
 * \param heap_size The size of the heap in bytes
 * \param classpath Colon separated list of directories and JAR files where to
 * find the application classes
 * \param bootclasspath Single directory or JAR file where to find the system
 * classes */

void vm_run(const char *classpath, const char *bootclasspath, char *main,
            size_t heap_size, int jargc, char **jargv)
{
    char *main_class;
    thread_t main_thread;
    class_t *cl, *thread_cl;
    method_t *method;
    uintptr_t args, ref;
    uintptr_t *args_data;
    int exception;

    // The main thread is initialized early because we need it for exceptions
    tm_init();
    thread_init(&main_thread);
    tm_register(&main_thread);

    c_try {
        vm_init(heap_size, classpath, bootclasspath);

        /* We initialize the temporary root pointers storage of the main thread
         * so that we can push temporary roots early in the bootstrap process */
        main_thread.roots.capacity = THREAD_TMP_ROOTS;
        main_thread.roots.pointers = gc_malloc(THREAD_TMP_ROOTS
                                               * sizeof(uintptr_t *));

        main_class = utf8_intern(main, strlen(main));

        /* Check if the 'main' string purged from the trailing '0' is in the
         * limited UTF8 format supported by Java */
        if (!utf8_check(main_class)) {
            c_throw(JAVA_LANG_NOCLASSDEFFOUNDERROR,
                    "Invalid class name: %s", main_class);
        }

        /* java.lang.String, java.lang.Class, java.lang.Thread and the array
         * class [C must be preloaded as they are needed by constant string
         * objects and class objects respectively */
        bcl_preload_bootstrap_classes();

        // Resolve the bootstrap classes
        bcl_resolve_class(NULL, "java/lang/Object");
        bcl_resolve_class(NULL, "[C");
        bcl_resolve_class(NULL, "java/lang/String");
        thread_cl = bcl_resolve_class(NULL, "java/lang/Thread");

        // Enable the garbage-collector now that it is safe to do so
        gc_enable(true);

        // Setup the dummy methods
        init_dummy_methods();

#if JEL_FINALIZER
        /* Create the finalizer thread: find the jelatine.VMFinalizer class and
         * find its run() method */
        cl = bcl_resolve_class(NULL, "jelatine/VMFinalizer");
        method = mm_get(cl->method_manager, "run", "()V");
        assert(method != NULL);

        /* Create the finalizer Java thread object, since it is not created
         * by VMThread.start() we have to do it by hand */
        ref = gc_new(thread_cl);
        JAVA_LANG_THREAD_REF2PTR(ref)->priority = 5;
        thread_push_root(&ref);
        // After this call has returned the finalizer thread will be running
        thread_launch(&ref, method);
        thread_pop_root();
#endif // JEL_FINALIZER

        cl = bcl_resolve_class(NULL, main_class);
        // Find the main() method
        method = mm_get(cl->method_manager, "main", "([Ljava/lang/String;)V");

        if (method == NULL) {
            c_throw(JAVA_LANG_NOCLASSDEFFOUNDERROR,
                    "Unable to find the main() method in class: %s",
                    main_class);
        } else if (!method_is_public(method)
                   || method_is_native(method)
                   || !method_is_static(method))
        {
            c_throw(JAVA_LANG_NOCLASSDEFFOUNDERROR,
                    "main() method is not public, native or non-static");
        }

        method->access_flags |= ACC_MAIN;

        // Prepare the 'args' array holding command line arguments
        args = gc_new_array_ref(bcl_resolve_class(NULL, "[Ljava/lang/String;"),
                                jargc);
        args_data = array_ref_get_data((array_t *) args);
        thread_push_root(&args);

        for (int i = 0; i < jargc; i++) {
            args_data[-i] =
                JAVA_LANG_STRING_PTR2REF(jstring_create_literal(jargv[i]));
        }

        // Launch the main thread
        ref = thread_create_main(&main_thread, method, &args);
        thread_pop_root();

        if (ref != JNULL) {
            // TODO: Print the type of the unknown exception and its contents
            c_throw(JAVA_LANG_VIRTUALMACHINEERROR,
                    "Uncaught exception in main thread");
        }
    } c_catch (exception) {
        c_print_exception(exception);
        c_clear_exception();
        vm_fail();
    }

    /* The thread manager is destroyed before the other VM structures because
     * we need to stop all the running threads before doing anything else */
    tm_teardown();
    vm_teardown();
} // vm_run()

/** Initializes the basic subsystems of the virtual machine.
 * \param heap_size The size of the heap in bytes
 * \param classpath The classpath for user classes
 * \param bootclasspath The classpath for system classes */

static void vm_init(size_t heap_size, const char *classpath,
                    const char *bootclasspath)
{
    // Initialize the various subsystems
    gc_init(heap_size);
    monitor_init();
    string_manager_init(6, 2);
    jsm_init(6, 2);
    classpath_init(classpath ? classpath : ".", bootclasspath);
    bcl_init();

    // Initialize the VM structure
    memset(&vm, 0, sizeof(virtual_machine_t));
} // vm_init()

/** Destroys the virtual machine instance */

static void vm_teardown( void )
{
    classpath_teardown();
    gc_teardown();
} // vm_teardown()
