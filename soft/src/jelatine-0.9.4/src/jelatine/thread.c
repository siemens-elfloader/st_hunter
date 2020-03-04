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

/** \file thread.c
 * Implementation of the virtual machine thread */

#include "wrappers.h"

#include "class.h"
#include "interpreter.h"
#include "kni.h"
#include "loader.h"
#include "memory.h"
#include "thread.h"
#include "vm.h"

#include "java_lang_Thread.h"

/******************************************************************************
 * Thread manager                                                             *
 ******************************************************************************/

/** Payload passed to a new thread's thread_start() routine. Holds all the
 * information necessary to bootstrap the thread. */

struct thread_payload_t {
    method_t *run; ///< The run() method of the thread
    uintptr_t *ref; ///< A pointer to the thread object
#if JEL_THREAD_POSIX
    pthread_t pthread; ///< POSIX thread
    pthread_cond_t pthread_cond; ///< POSIX condition variable
#elif JEL_THREAD_PTH
    pth_t pth; ///< GNU/Pth thread
    pth_cond_t pth_cond; ///< GNU/Pth condition variable
#endif
};

/** Typedef for the ::struct thread_payload_t type */
typedef struct thread_payload_t thread_payload_t;

/** Represents a Java monitor */

struct monitor_t {
    struct monitor_t *next; ///< Next monitor in the list
    uintptr_t ref; ///< Object associated with this monitor
    thread_t *owner; ///< Owner thread, NULL if the monitor is unlocked
    size_t count; ///< Number of times the monitor was acquired
#if JEL_THREAD_POSIX
    pthread_cond_t *pthread_cond; ///< POSIX condition variable
#elif JEL_THREAD_PTH
    pth_cond_t *pth_cond; ///< GNU/Pth condition variable
#endif
};

/** Typedef for the ::struct monitor_t type */
typedef struct monitor_t monitor_t;

/** Define the minimum capacity (i.e. number of buckets) of the monitor table */
#define TM_CAPACITY (4)

/** Thread manager, contains all the threads and is used to interact with them
 * globally */

struct thread_manager_t {
#if JEL_THREAD_POSIX
    pthread_mutex_t lock; ///< Global VM lock
#elif JEL_THREAD_PTH
    pth_mutex_t lock; ///< Global VM lock
    size_t switch_n; ///< Number of context switches
#endif
    size_t active; ///< Number of active threads
    thread_t *queue; ///< Doubly-linked queue of active threads
    size_t capacity; ///< Capacity of the monitor hash-table
    size_t entries; ///< Number of used entries of the monitor hash-table
    monitor_t *buckets; ///< Buckets of the monitor hash-table
};

/** Typedef for the ::struct thread_manager_t type */
typedef struct thread_manager_t thread_manager_t;

/******************************************************************************
 * Local declarations                                                         *
 ******************************************************************************/

/** \var self
 * Thread-local self pointer of a thread */

#if JEL_THREAD_POSIX
#   ifdef TLS
static TLS thread_t *self;
#   else
static pthread_key_t self;
#   endif // TLS
#elif JEL_THREAD_PTH
static pth_key_t self;
#else
static thread_t *self;
#endif

/** Thread manager singleton */
static thread_manager_t tm;

/******************************************************************************
 * Thread manager implementation                                              *
 ******************************************************************************/

/** Number of global lock acquisition/release couples before yielding when
 * using GNU/Pth */
#define PTH_SWITCH_N (500)

/** Initialize the thread manager, this does not include the initialization of
 * the Java monitors subsystem because the thread manager is initialized before
 * the heap manager */

void tm_init( void )
{
#if JEL_THREAD_POSIX
    pthread_mutexattr_t attr;
#endif // JEL_THREAD_POSIX

    memset(&tm, 0, sizeof(thread_manager_t));

    /* When using multi-threading the global machine lock is recursive so that
     * the code can enter more than one syncrhonized sections safely */
#if JEL_THREAD_POSIX
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&tm.lock, &attr);
    pthread_mutexattr_destroy(&attr);
#elif JEL_THREAD_PTH
    if (!pth_init()) {
        vm_fail();
    }

    pth_mutex_init(&tm.lock);
    tm.switch_n = PTH_SWITCH_N;
#endif

#if JEL_THREAD_POSIX && !defined(TLS)
    pthread_key_create(&self, NULL);
#elif JEL_THREAD_PTH
    pth_key_create(&self, NULL);
#endif
} // tm_init()

/** Tears down the thread manager */

void tm_teardown( void )
{
#if JEL_THREAD_POSIX
    thread_t *thread;

    tm_lock();
    tm_stop_the_world();

    for (thread = tm.queue; thread != NULL; thread = thread->next) {
        pthread_cancel(thread->pthread);
    }

    tm_unlock();
    pthread_mutex_destroy(&tm.lock);
#   ifndef TLS
    pthread_key_delete(self);
#   endif // !TLS
#elif JEL_THREAD_PTH
    pth_key_delete(self);
    pth_kill();
#endif
} // tm_teardown()

/** Register a new thread in the thread manager
 * \param thread A pointer to the new thread */

void tm_register(thread_t *thread)
{
    thread->next = tm.queue;
    tm.queue = thread;
    tm.active++;
} // tm_register()

/** Unregister a dying thread
 * \param thread A thread which has finished execution */

void tm_unregister(thread_t *thread)
{
    thread_t *prev, *curr;

    prev = NULL;
    curr = tm.queue;

    while (thread != curr) {
        prev = curr;
        curr = curr->next;
    }

    if (prev == NULL) {
        tm.queue = curr->next;
    } else {
        prev->next = curr->next;
    }

    tm.active--;
} // tm_unregister_thread()

/** Returns the number of active threads in the system
 * \returns Returns the number of active threads */

uint32_t tm_active( void )
{
#if JEL_FINALIZER
    return tm.active - 1;
#else
    return tm.active;
#endif // JEL_FINALIZER
} // tm_active()

/** Marks all the references reacheable from the threads */

void tm_mark( void )
{
    jword_t *scan;
    thread_t *thread;

    for (thread = tm.queue; thread != NULL; thread = thread->next) {
        gc_mark_reference(thread->obj);

        // Mark the temporary roots
        for (size_t i = 0; i < thread->roots.used; i++) {
            gc_mark_potential(*(thread->roots.pointers[i]));
        }

        // Crawl the stack
        if (thread->stack != NULL) {
            for (scan = thread->stack; scan < thread->sp; scan++) {
                gc_mark_potential(*((uintptr_t *) scan));
            }
        }

        // Mark the pending exception
        gc_mark_reference(thread->exception);
    }
} // tm_mark()

/** Hash function used for object references
 * \param ref A reference to a Java object */

static size_t tm_hash(uintptr_t ref)
{
#if SIZEOF_VOID_P == 4
    return (ref >> 2);
#else
    return (ref >> 3);
#endif
} // tm_hash()

/** Purge the monitor table by clearing the entries associated with dead
 * objects, also purge the associated conditions */

void tm_purge( void )
{
    size_t hash, entries = 0;
    monitor_t *entry, *other;
    header_t *header;

    for (size_t i = 0; i < tm.capacity; i++) {
        entry = tm.buckets + i;

        if (entry->ref) {
            header = (header_t *) entry->ref;

            if (!header_is_marked(header)) {
                /* The object referenced by this monitor is dead, let's purge
                 * the monitor then */
#if JEL_THREAD_POSIX
                if (entry->pthread_cond) {
                    pthread_cond_destroy(entry->pthread_cond);
                    gc_free(entry->pthread_cond);
                }
#elif JEL_THREAD_PTH
                if (entry->pth_cond) {
                    /* HACK, GROSS: This relies on internal information of
                     * GNU/Pth, we should find a better way for checking that
                     * there are no waiters */
                    assert(entry->pth_cond->cn_waiters == 0);
                    gc_free(entry->pth_cond);
                }
#endif
                memset(entry, 0, sizeof(monitor_t));
            } else {
                entries++;
            }
        }
    }

    tm.entries = entries;

    /* This first phase will scan the buckets looking for the remaining
     * monitors. If a monitor is not in the bucket corresponding to its hash
     * value we check the bucket corresponding to the hash value. If it is
     * empty the monitor is moved there, if it is full but the monitor there
     * does not correspond to the hash value the two monitors are swapped.
     * This phase also clears the 'next' field of the monitors so that we can
     * re-chain them in the next phase */

    for (size_t i = 0; i < tm.capacity; i++) {
        entry = tm.buckets + i;

        if (entry->ref) {
            entry->next = NULL;
            hash = tm_hash(entry->ref) & (tm.capacity - 1);

            if (i != hash) {
                other = tm.buckets + hash;

                if (other->ref == JNULL) {
                    *other = *entry;
                    memset(entry, 0, sizeof(monitor_t));
                } else if ((tm_hash(other->ref) & (tm.capacity - 1)) != hash) {
                    monitor_t dummy = *other;
                    *other = *entry;
                    *entry = dummy;
                }
            }
        }
    }

    /* We look for the entries that are in the buckets which are not associated
     * with their hash value. Those entries are chained after the entries in
     * the buckets associated with their hash value */

    for (size_t i = 0; i < tm.capacity; i++) {
        entry = tm.buckets + i;

        if (entry->ref) {
            hash = tm_hash(entry->ref) & (tm.capacity - 1);

            if (i != hash) {
                entry->next = tm.buckets[hash].next;
                tm.buckets[hash].next = entry;
            }
        }
    }
} // tm_purge()

/** Rehash the monitor table, growing or shrinking it
 * \param grow true if the table must be grown, false if it must be shrinked */

static void tm_rehash(bool grow)
{
    size_t capacity, hash;
    size_t i, j;
    monitor_t *buckets, *entry;

    if (grow) {
        capacity = tm.capacity * 2;
    } else {
        capacity = tm.capacity / 2;
    }

    buckets = gc_malloc(capacity * sizeof(monitor_t));

    for (i = 0; i < tm.capacity; i++) {
        entry = tm.buckets + i;

        if (entry->ref) {
            j = hash = tm_hash(entry->ref) & (capacity - 1);

            while (buckets[j].ref != JNULL) {
                j = (j + 1) & (capacity - 1);
            }

            buckets[j].ref = entry->ref;
            buckets[j].owner = entry->owner;
            buckets[j].count = entry->count;
#if JEL_THREAD_POSIX
            buckets[j].pthread_cond = entry->pthread_cond;
#elif JEL_THREAD_PTH
            buckets[j].pth_cond = entry->pth_cond;
#endif

            // Chain the entry if there was a clash
            if (j == hash) {
                buckets[j].next = NULL;
            } else {
                buckets[j].next = buckets[hash].next;
                buckets[hash].next = buckets + j;
            }
        }
    }

    gc_free(tm.buckets);
    tm.capacity = capacity;
    tm.buckets = buckets;
} // tm_rehash()

#if JEL_THREAD_POSIX || JEL_THREAD_PTH

/** Try to gain the ownership of the machine-wide lock.
 * This function will return only when the caller has the ownership of the lock
 * in the meantime the calling thread is considered in a safe zone if. It is
 * safe to call this function repeatedly from the same thread as long as
 * tm_unlock() is called the same number of times to release the lock
 * multithreading is enabled */

void tm_lock( void )
{
    thread_self()->safe++; // Enter or re-enter into a safe zone
#if JEL_THREAD_POSIX
    pthread_mutex_lock(&tm.lock);
#elif JEL_THREAD_PTH
    pth_mutex_acquire(&tm.lock, FALSE, NULL);
#endif
} // tm_lock()

/** Release the ownership of the machine-wide lock */

void tm_unlock( void )
{
    assert(thread_self()->safe != 0);
    thread_self()->safe--; // Exit a safe zone if 'safe' drops to 0
#if JEL_THREAD_POSIX
    pthread_mutex_unlock(&tm.lock);
#elif JEL_THREAD_PTH
    pth_mutex_release(&tm.lock);

    if (--tm.switch_n == 0) {
        tm.switch_n = PTH_SWITCH_N;
        pth_yield(NULL);
    }
#endif
} // tm_unlock()

/** Returns a pointer to the VM global lock. We should do away with this method
 * but the garbage collector still uses it explicitely when registering
 * finalizable objects
 * \returns A pointer to the VM global lock */

void *tm_get_lock( void )
{
    return &tm.lock;
} // tm_get_lock()

/** Waits for all threads to stop. The calling thread must have taken the VM
 * lock for this to happen safely */

void tm_stop_the_world( void )
{
    thread_t *thread;
    bool stopped = false;

    // Wait for all the threads to stop
    while (!stopped) {
        stopped = true;

        for (thread = tm.queue; thread != NULL; thread = thread->next) {
            /* If the 'safe' field is 1 then the thread has been stopped, the
             * calling thread will have safe >= 1. */
            if (thread->safe == 0) {
                stopped = false;
            }
        }

        thread_yield();
    }
} // tm_stop_the_world()

#endif // JEL_THREAD_POSIX || JEL_THREAD_PTH

/******************************************************************************
 * Monitors                                                                   *
 ******************************************************************************/

/** Initialization of the Java monitors subsystem */

void monitor_init( void )
{
    tm.capacity = TM_CAPACITY;
    tm.buckets = gc_malloc(TM_CAPACITY * sizeof(monitor_t));
} // monitor_init()

/** Implements the MONITOR_ENTER opcode
 * \param thread The thread requesting the monitor
 * \param ref A Java object reference */

void monitor_enter(thread_t *thread, uintptr_t ref)
{
    monitor_t *entry;
    size_t hash;
    bool done;

    do {
        done = true;
        tm_lock();

        hash = tm_hash(ref) & (tm.capacity - 1);

        // Check if the monitor is already present
        entry = tm.buckets + hash;

        while (entry && (entry->ref != ref)) {
            entry = entry->next;
        }

        if (entry) {
            // The monitor is already present
            if (entry->owner == NULL) {
                entry->owner = thread;
                entry->count = 1;
            } else if (entry->owner == thread) {
                entry->count++;
            } else {
                // This monitor is contended, spin
                tm_unlock();
                thread_yield();
                done = false;
            }
        } else {
            // The monitor is not there, let's insert a new one
            size_t i = hash;

            while (tm.buckets[i].ref != JNULL) {
                i = (i + 1) & (tm.capacity - 1);
            }

            entry = tm.buckets + i;
            entry->ref = ref;
            entry->owner = thread;
            entry->count = 1;

            // Chain the entry if there was a clash
            if (i == hash) {
                entry->next = NULL;
            } else {
                entry->next = tm.buckets[hash].next;
                tm.buckets[hash].next = entry;
            }

            // Check if we must grow the table
            tm.entries++;

            if (tm.entries == tm.capacity) {
                tm_rehash(true);
            } else if (tm.entries < (tm.capacity / 4)) {
                tm_rehash(false);
            }
        }
    } while (!done);

    tm_unlock();
} // monitor_enter()

/** Implements the MONITOR_EXIT opcode
 * \param thread A pointer to the thread relinquishing the monitor
 * \param ref The object on which the monitor was taken
 * \returns true if everything went well, false if \a thread didn't own the
 * monitor and thus MONITOR_EXIT would fail */

bool monitor_exit(thread_t *thread, uintptr_t ref)
{
    monitor_t *entry;
    size_t hash;
    bool done = false;

    tm_lock();
    hash = tm_hash(ref) & (tm.capacity - 1);

    // Look for the monitor
    entry = tm.buckets + hash;

    while (entry && (entry->ref != ref)) {
        entry = entry->next;
    }

    if (entry) {
        if (entry->owner == thread) {
            if (entry->count == 1) {
                entry->owner = NULL;
            }

            entry->count--;
            done = true;
        }
    }

    tm_unlock();

    return done;
} // monitor_exit()

/******************************************************************************
 * Thread implementation                                                      *
 ******************************************************************************/

/** Sets the thread's own ::thread_t structure
 * \param thread A pointer to the thread's own structure */

static void thread_set_self(thread_t *thread)
{
#if JEL_THREAD_POSIX
#   ifdef TLS
    self = thread;
#   else
    pthread_setspecific(self, thread);
#   endif // TLS
#elif JEL_THREAD_PTH
    pth_key_setdata(self, thread);
#else
    self = thread;
#endif
} // thread_set_self()

/** Return a pointer to the execution thread's ::thread_t structure
 * \returns A pointer to this thread own structure */

thread_t *thread_self( void )
{
#if JEL_THREAD_POSIX
#   ifdef TLS
    return self;
#else
    return (thread_t *) pthread_getspecific(self);
#endif // TLS
#elif JEL_THREAD_PTH
    return (thread_t *) pth_key_getdata(self);
#else
    return self;
#endif
} // thread_self()

/** Push a temporary root on the current thread's stack
 * \param ref A pointer to an object reference to be pushed on the temporary
 * root stack */

void thread_push_root(uintptr_t *ref)
{
    thread_t *thread = thread_self();
    uintptr_t **pointers;

    thread->roots.pointers[thread->roots.used++] = ref;

    /* Grow the stack so that a free slot is always available when entering
     * this function */
    if (thread->roots.used == thread->roots.capacity) {
        pointers = gc_malloc((thread->roots.capacity + 1)
                             * sizeof(uintptr_t *));
        memcpy(pointers, thread->roots.pointers,
               thread->roots.capacity * sizeof(uintptr_t *));
        gc_free(thread->roots.pointers);
        thread->roots.pointers = pointers;
        thread->roots.capacity++;
    }
} // thread_push_root()

/** Pop a temporary root from a thread's stack */

void thread_pop_root( void )
{
    thread_self()->roots.used--;
} // thread_pop_root()

/** Initializes all the thread-local structures required for normal operation
 * of a thread including the thread's self reference
 * \param thread A pointer to the thread's own structure */

void thread_init(thread_t *thread)
{
    memset(thread, 0, sizeof(thread_t));
    thread_set_self(thread);

#if JEL_THREAD_POSIX
    pthread_cond_init(&thread->pthread_cond, NULL);
#elif JEL_THREAD_PTH
    pth_cond_init(&thread->pth_cond);
#endif
} // thread_init()

/** Creates the main thread. This function doesn't return until the main thread
 * has finished execution as it uses the current native/green thread instead of
 * creating a new one
 * \param thread The main thread pre-allocated structure
 * \param run The main method
 * \param args A pointer to a reference to the arguments array
 * \returns The reference to an uncaught exception if the main method terminated
 * abruptly or JNULL */

uintptr_t thread_create_main(thread_t *thread, method_t *run, uintptr_t *args)
{
    class_t *thread_cl = bcl_find_class("java/lang/Thread");
    size_t stack_size = opts_get_stack_size();

    /* Contrary to the startup of a 'regular' thread we do not initialize nor
     * register the thread as the main thread has already been initialized and
     * registered even though it is only partially initialized */

    // Build the stack
    thread->stack = gc_malloc(stack_size);
    thread->sp = thread->stack;
    thread->fp = (stack_frame_t *) ((char *) thread->stack + stack_size);

    /* Create the first Java thread structure, since the first thread is not
     * created by VMThread.start() we have to do it by hand */
    thread->obj = gc_new(thread_cl);
    JAVA_LANG_THREAD_REF2PTR(thread->obj)->vmThread = (uintptr_t) thread;
    JAVA_LANG_THREAD_REF2PTR(thread->obj)->priority = 5;
    JAVA_LANG_THREAD_REF2PTR(thread->obj)->name =
        JAVA_LANG_STRING_PTR2REF(jstring_create_from_utf8("Thread-0"));

    // HACK: Push the arguments on top of the stack
    *((uintptr_t *) thread->sp) = *args;

    // Run the new thread's code
    interpreter(run);

    // Mark the thread as dead, none can join on it anymore after this point
    tm_lock();
#if JEL_THREAD_POSIX
    pthread_cond_broadcast(&thread->pthread_cond);
#elif JEL_THREAD_PTH
    pth_cond_notify(&thread->pth_cond, true);
#endif
    tm_unregister(thread);
    JAVA_LANG_THREAD_REF2PTR(thread->obj)->vmThread = JNULL;
    tm_unlock();

    gc_free(thread->roots.pointers);
    gc_free(thread->stack);

    return thread->exception;
} // thread_create_main()

/** Implements the functionality required by the java.lang.Thread.sleep()
 * method, waiting for the number of milliseconds specified by \a ms
 * \param ms The number of milliseconds to wait */

void thread_sleep(int64_t ms)
{
    thread_t *self = thread_self();

    tm_lock();

    if (!self->interrupted) {
#if JEL_THREAD_PTH
        pth_event_t ev;
        pth_cond_t cond = PTH_COND_INIT; // Dummy condition variable

        ev = pth_event(PTH_EVENT_TIME,
                       pth_timeout(ms / 1000,
                                   (ms % 1000) * 1000));
        self->pth_int = &cond;
        pth_cond_await(&cond, &tm.lock, ev);
        self->pth_int = NULL;
        pth_event_free(ev, PTH_FREE_ALL);
#elif JEL_THREAD_POSIX
        struct timespec req = get_time_with_offset(ms, 0);
        pthread_cond_t cond = PTHREAD_COND_INITIALIZER; // Dummy condition variable

        self->pthread_int = &cond;
        pthread_cond_timedwait(&cond, &tm.lock, &req);
        self->pthread_int = NULL;
#else
        struct timespec req = { ms / 1000, (ms % 1000) * 1000000 };

        //nanosleep(&req, NULL);
        usleep(ms);
#endif
    }

    if (self->interrupted) {
        self->interrupted = false;
        KNI_ThrowNew("java/lang/InterruptedException", NULL);
    }

    tm_unlock();
} // thread_sleep()

#if JEL_THREAD_POSIX || JEL_THREAD_PTH

/** Startup function used as the entry point of a new thread. Both the payload
 * passed in \a arg and the new thread structures (including the stack) will be
 * fred by this function upon ending execution
 * \param arg A pointer to the thread payload structure
 * \returns Nothing important */

static void *thread_start(void *arg)
{
    size_t stack_size = opts_get_stack_size();
    thread_payload_t *payload = (thread_payload_t *) arg;
    method_t *run = payload->run;
    uintptr_t *ref = payload->ref;
    thread_t thread;
    int exception;

    // Initialize the new thread
    thread_init(&thread);

    // Build the stack
    thread.stack = gc_malloc(stack_size);
    thread.sp = thread.stack;
    thread.fp = (stack_frame_t *) ((char *) thread.stack + stack_size);

    // Create the temporary roots area
    thread.roots.capacity = THREAD_TMP_ROOTS;
    thread.roots.pointers = gc_malloc(THREAD_TMP_ROOTS * sizeof(uintptr_t *));

    tm_lock();

    // Link the java.lang.Thread object to the VM thread and register it
    thread.obj = *ref;
    JAVA_LANG_THREAD_REF2PTR(thread.obj)->vmThread = (uintptr_t) &thread;
    JAVA_LANG_THREAD_REF2PTR(thread.obj)->priority = 5;
    tm_register(&thread);

    // Inform the parent thread that we're starting execution
#if JEL_THREAD_POSIX
    thread.pthread = payload->pthread;
    pthread_cond_signal(&payload->pthread_cond);
#elif JEL_THREAD_PTH
    thread.pth = payload->pth;
    pth_cond_notify(&payload->pth_cond, false);
#endif
    tm_unlock();

    // HACK: Push the 'this' pointer on top of the stack
    *((uintptr_t *) thread.stack) = thread.obj;

    c_try {
        interpreter(run); // Run the new thread's code
    } c_catch (exception) {
        c_print_exception(exception);
        c_clear_exception();
        vm_fail();
    }

    // Mark the thread as dead, none can join on it anymore after this point
    tm_lock();

#if JEL_THREAD_POSIX
    pthread_cond_broadcast(&thread.pthread_cond);
#elif JEL_THREAD_PTH
    pth_cond_notify(&thread.pth_cond, true);
#endif
    tm_unregister(&thread);
    JAVA_LANG_THREAD_REF2PTR(thread.obj)->vmThread = JNULL;

    /* We can safely release the stack and root pointers now that the thread
     * is not visible anymore to the garbage collector */
    gc_free(thread.roots.pointers);
    gc_free(thread.stack);

    tm_unlock();

    // Wait for the joining thread to wake up
#if JEL_THREAD_POSIX
    while (pthread_cond_destroy(&thread.pthread_cond) != 0) {
        ;
    }
#elif JEL_THREAD_PTH
    /* HACK, GROSS: This relies on internal information of GNU/Pth, we should
     * find a better way for checking that there are no more waiters */
    while (thread.pth_cond.cn_waiters != 0) {
        pth_yield(NULL);
    }
#endif

    if (thread.exception != JNULL) {
        // TODO: Print the exception and stack trace
        dbg_error("Uncaught exception");
        vm_fail();
    }

#if JEL_THREAD_POSIX
    pthread_exit(NULL);
#elif JEL_THREAD_PTH
    pth_exit(NULL);
#endif

    return NULL;
} // thread_start()

/** Creates a new thread executing the provided method
 * \param obj The java.lang.Thread object associated with this thread
 * \param run The first method executed by this thread */

void thread_launch(uintptr_t *ref, method_t *run)
{
    thread_payload_t *payload;
#if JEL_THREAD_POSIX
    int res;
    pthread_attr_t attr;
#elif JEL_THREAD_PTH
    pth_attr_t attr;
#endif

    // Prepare the thread payload
    payload = gc_malloc(sizeof(thread_payload_t));
    payload->run = run;
    payload->ref = ref;
#if JEL_THREAD_POSIX
    pthread_cond_init(&payload->pthread_cond, NULL);
#elif JEL_THREAD_PTH
    pth_cond_init(&payload->pth_cond);
#endif

    // We take the global lock for we will be waiting for the new thread
    tm_lock();

    // FIXME: Check if thread creation fails and shut down the machine 'cleanly'
#if JEL_THREAD_POSIX
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    res = pthread_create(&payload->pthread, &attr, thread_start, payload);

    if (res) {
        dbg_error("Unable to create a new thread");
        vm_fail();
    }

    pthread_attr_destroy(&attr);
#elif JEL_THREAD_PTH
    attr = pth_attr_new();
    pth_attr_set(attr, PTH_ATTR_JOINABLE, FALSE);

    payload->pth = pth_spawn(attr, thread_start, payload);

    if (payload->pth == NULL) {
        dbg_error("Unable to create a new thread");
        vm_fail();
    }

    pth_attr_destroy(attr);
#endif

    // Wait for the new thread
#if JEL_THREAD_POSIX
    pthread_cond_wait(&payload->pthread_cond, &tm.lock);
#else
    pth_cond_await(&payload->pth_cond, &tm.lock, NULL);
#endif
    tm_unlock();

    // Cleanup
#if JEL_THREAD_POSIX
    pthread_cond_destroy(&payload->pthread_cond);
#endif // JEL_THREAD_POSIX
    gc_free(payload);
} // thread_launch()

/** Interrupts a thread. This function is used for implementing the
 * java.lang.Thread.interrupt() method and can interrupt immediately the
 * java.lang.Object.wait() and java.lang.Thread.sleep() methods but only
 * asynchronously the java.lang.Thread.join() method
 * \param thread A pointer to the thread to be interrupted */

void thread_interrupt(thread_t *thread)
{
    tm_lock();
    thread->interrupted = true;

#if JEL_THREAD_POSIX
    if (thread->pthread_int) {
        pthread_cond_signal(thread->pthread_int);
    }
#elif JEL_THREAD_PTH
    if (thread->pth_int) {
        pth_cond_notify(thread->pth_int, false);
    }
#endif

    tm_unlock();
} // thread_interrupt()

/** Causes the currently executing thread object to temporarily pause and allow
 * other threads to execute */

void thread_yield( void )
{
#if JEL_THREAD_POSIX
#   if HAVE_PTHREAD_YIELD_NP
    pthread_yield_np();
#   else
    pthread_yield();
#endif // JEL_THREAD_POSIX
#elif JEL_THREAD_PTH
    pth_yield(NULL);
#endif
} // thread_yield()

/** Joins on the passed thread
 * \param thread A pointer to the reference to the Java thread the function
 * will join on */


void thread_join(uintptr_t *thread)
{
    thread_t *self = thread_self();
    thread_t *target;

    tm_lock();

    if (!self->interrupted) {
        target = (thread_t *) JAVA_LANG_THREAD_REF2PTR(*thread)->vmThread;

        if (target != NULL) {
#if JEL_THREAD_POSIX
            self->pthread_int = &target->pthread_cond;
            pthread_cond_wait(&target->pthread_cond, &tm.lock);
            self->pthread_int = NULL;
#elif JEL_THREAD_PTH
            self->pth_int = &target->pth_cond;
            pth_cond_await(&target->pth_cond, &tm.lock, NULL);
            self->pth_int = NULL;
#endif
        }
    }

    if (self->interrupted) {
        self->interrupted = false;
        KNI_ThrowNew("java/lang/InterruptedException", NULL);
    }

    tm_unlock();
} // thread_join()

/** Implements the functionality required by java.lang.Object.wait() and
 * friends, waiting on an object until it is notified by notify() or notifyAll()
 * or until a timeout occurs (if \a nanos or \a millis or both ar non-zero)
 * \param ref The object on which to wait
 * \param millis Number of milliseconds before a timeout occurs
 * \param nanos Number of nanoseconds before a timeout occurs
 * \returns true if everything goes well, false if the monitor associated with
 * \a ref was not owned \a thread. In the latter case an
 * IllegalMonitorStateException should be thrown */

bool thread_wait(uintptr_t ref, uint64_t millis, uint32_t nanos)
{
    thread_t *self = thread_self();
    monitor_t *entry;
    size_t hash;

    tm_lock();

    // Look for the monitor
    hash = tm_hash(ref) & (tm.capacity - 1);
    entry = tm.buckets + hash;

    while (entry && (entry->ref != ref)) {
        entry = entry->next;
    }

    if (entry && !self->interrupted) {
        if ((entry->owner == self) && (entry->count == 1)) {
            // Release the lock and wait
            entry->owner = NULL;
            entry->count = 0;

#if JEL_THREAD_POSIX
            if (entry->pthread_cond == NULL) {
                entry->pthread_cond = gc_malloc(sizeof(pthread_cond_t));
                pthread_cond_init(entry->pthread_cond, NULL);
            }

            self->pthread_int = entry->pthread_cond;

            if ((millis == 0) && (nanos == 0)) {
                pthread_cond_wait(entry->pthread_cond, &tm.lock);
            } else {
                struct timespec req = get_time_with_offset(millis, nanos);

                pthread_cond_timedwait(entry->pthread_cond, &tm.lock, &req);
            }

            self->pthread_int = NULL;

#elif JEL_THREAD_PTH
            if (entry->pth_cond == NULL) {
                entry->pth_cond = gc_malloc(sizeof(pth_cond_t));
                pth_cond_init(entry->pth_cond);
            }

            self->pth_int = entry->pth_cond;

            if ((millis == 0) && (nanos == 0)) {
                pth_cond_await(entry->pth_cond, &tm.lock, NULL);
            } else {
                pth_event_t ev;

                ev = pth_event(PTH_EVENT_TIME,
                               pth_timeout(millis / 1000,
                                           (millis % 1000) * 1000000 + nanos));
                pth_cond_await(entry->pth_cond, &tm.lock, ev);
                pth_event_free(ev, PTH_FREE_ALL);
            }

            self->pth_int = NULL;
#endif

            // Re-acquire the monitor
            tm_unlock();
            monitor_enter(self, ref);
        }
    } else {
        tm_unlock();
    }

    if (self->interrupted) {
        self->interrupted = false;
        KNI_ThrowNew("java/lang/InterruptedException", NULL);
    }

    return entry ? true : false;
} // thread_wait()

/** Implements the functionliaty required by java.lang.Object.notify()
 * \param ref The object used for the notification
 * \param broadcast If true, wake up all the threads waiting on this object,
 * otherwise wake up only one thread
 * \returns true if everything goes well, false if the monitor associated with
 * \a ref was not owned by \a thread. In the latter case an
 * IllegalMonitorStateException should be thrown */

bool thread_notify(uintptr_t ref, bool broadcast)
{
    thread_t *self = thread_self();
    monitor_t *entry;
    size_t hash;
    bool res = false;

    tm_lock();

    // Look for the monitor
    hash = tm_hash(ref) & (tm.capacity - 1);
    entry = tm.buckets + hash;

    while (entry && (entry->ref != ref)) {
        entry = entry->next;
    }

    if (entry) {
        if (entry->owner == self) {
            // Notify the waiting thread
#if JEL_THREAD_POSIX
            if (entry->pthread_cond != NULL) {
                if (broadcast) {
                    pthread_cond_broadcast(entry->pthread_cond);
                } else {
                    pthread_cond_signal(entry->pthread_cond);
                }
            }
#elif JEL_THREAD_PTH
            if (entry->pth_cond != NULL) {
                pth_cond_notify(entry->pth_cond, broadcast);
            }
#endif
            res = true;
        }
    }

    tm_unlock();
    return res;
} // thread_notify()

#endif // JEL_THREAD_POSIX || JEL_THREAD_PTH

