###############################################################################
##   Copyright © 2005-2009 by Gabriele Svelto                                ##
##   gabriele.svelto@gmail.com                                               ##
##                                                                           ##
##   This file is part of Jelatine.                                          ##
##                                                                           ##
##   Jelatine is free software: you can redistribute it and/or modify        ##
##   it under the terms of the GNU General Public License as published by    ##
##   the Free Software Foundation, either version 3 of the License, or       ##
##   (at your option) any later version.                                     ##
##                                                                           ##
##   Jelatine is distributed in the hope that it will be useful,             ##
##   but WITHOUT ANY WARRANTY; without even the implied warranty of          ##
##   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           ##
##   GNU General Public License for more details.                            ##
##                                                                           ##
##   You should have received a copy of the GNU General Public License       ##
##   along with Jelatine.  If not, see <http://www.gnu.org/licenses/>.       ##
###############################################################################

AC_INIT([Jelatine], [0.9.4], [gabriele.svelto@gmail.com], [jelatine])
AM_INIT_AUTOMAKE

# Prerequisite autoconf version
AC_PREREQ([2.60])

# Inform autoconf to output a config.h file
AC_CONFIG_HEADERS([config.h])

# Check if main.c is present in the 'src' directory
AC_CONFIG_SRCDIR([src/jelatine/main.c])

# Set to 'lib' the directory where AC_LIBOBJ replacements are present
AC_CONFIG_LIBOBJ_DIR([lib])

# Set to 'm4' the directory where aclocal should look for extra macros
AC_CONFIG_MACRO_DIR([m4])

# Jelatine definitions templates
AH_TEMPLATE([JEL_JARFILE_SUPPORT], [Enabled if JAR file support is needed])
AH_TEMPLATE([JEL_SOCKET_SUPPORT], [Enabled if socket support is used])
AH_TEMPLATE([JEL_THREAD_POSIX], [Enabled when POSIX thread support is used])
AH_TEMPLATE([JEL_THREAD_PTH], [Enabled when GNU Pth thread support is used])
AH_TEMPLATE([JEL_THREAD_NONE], [Enabled when thread support is disabled])
AH_TEMPLATE([JEL_THREADED_INTERPRETER],
    [Enabled if the threaded, optimized interpreter is needed])
AH_TEMPLATE([JEL_FINALIZER], [Enabled if object finalization is needed])
AH_TEMPLATE([JEL_FP_SUPPORT], [Enabled if floating-point support is needed])
AH_TEMPLATE([JEL_POINTER_REVERSAL],
    [Enabled if the pointer reversal based garbage collector is needed])
AH_TEMPLATE([JEL_TRACE], [Enabled if bytecode/method tracing is needed])
AH_TEMPLATE([JEL_PRINT], [Enabled if bytecode/method printing is needed])
AH_TEMPLATE([JEL_CLASSPATH_DIR], [Holds the default classpath directory])
AH_TEMPLATE([NDEBUG], [Enabled if debugging code must be removed])

# Define shell variables

AS_VAR_SET(want_preverifier, yes)
AS_VAR_SET(jar_support, yes)
AS_VAR_SET(socket_support, yes)
AS_VAR_SET(threaded, auto)
AS_VAR_SET(thread_model, auto)
AS_VAR_SET(finalizer, yes)
AS_VAR_SET(fp_support, yes)

################################################################################
# Define the extra arguments the user can pass to the configure script         #
################################################################################

AC_ARG_ENABLE(
    [preverifier],
    [AC_HELP_STRING(
        [--disable-preverifier],
        [Do not build the preverifier]
    )],
    [AS_VAR_SET(want_preverifier, no)]
)

AC_ARG_ENABLE(
    [jar],
    [AC_HELP_STRING(
        [--disable-jar],
        [Disables JAR file support in the VM]
    )],
    [AS_VAR_SET(jar_support, no)]
)

AC_ARG_ENABLE(
    [socket],
    [AC_HELP_STRING(
        [--disable-socket],
        [Disables socket support in the VM]
    )],
    [AS_VAR_SET(socket_support, no)]
)

AC_ARG_ENABLE(
    [threaded],
    [AC_HELP_STRING(
        [--disable-threaded],
        [Disables optimized threaded interpreter]
    )],
    [AS_VAR_SET(threaded, no)],
)

AC_ARG_WITH(
    [thread-model],
    [AC_HELP_STRING(
        [--with-thread-model=<model>],
        [Selects the thread model used by the VM (pthread, pth, none) [default=auto]])],
    [AS_VAR_SET(thread_model, $withval)],
)

AC_ARG_ENABLE(
    [finalizer],
    [AC_HELP_STRING(
        [--disable-finalizer],
        [Disables object finalization support]
    )],
    [AS_VAR_SET(finalizer, no)],
)

AC_ARG_ENABLE(
    [fp-support],
    [AC_HELP_STRING(
        [--disable-fp-support],
        [Disables fp support in the VM]
    )],
    [AS_VAR_SET(fp_support, no)],
)

AC_ARG_ENABLE(
    [prgc],
    [AC_HELP_STRING(
        [--disable-prgc],
        [Disables the pointer reversal algorythm in the garbage collector]
    )],
    ,
    [AC_DEFINE([JEL_POINTER_REVERSAL], [1])]
)

AC_ARG_ENABLE(
    [debug],
    [AC_HELP_STRING(
        [--enable-debug],
        [Enables debugging code]
    )],
    [AS_VAR_SET(debug, yes)],
    [AS_VAR_SET(debug, no)
    AC_DEFINE([NDEBUG], [1])]
)

AC_ARG_ENABLE(
    [tracing],
    [AC_HELP_STRING(
        [--enable-tracing],
        [Enables bytecode/method tracing facilities]
    )],
    [AC_DEFINE([JEL_TRACE], [1])]
)

AC_ARG_ENABLE(
    [printing],
    [AC_HELP_STRING(
        [--enable-printing],
        [Enables bytecode/method printing facilites]
    )],
    [AC_DEFINE([JEL_PRINT], [1])]
)

################################################################################
# Check for programs                                                           #
################################################################################

# Check for a working C compiler

AC_PROG_CC_C99

AS_IF([test "x$ac_cv_prog_cc_c99" = xno],
    [AC_MSG_WARN([You don't seem to have a C99 compiler, compilation may fail])]
)

# Enable POSIX extensions for targets which disables them by default
AC_USE_SYSTEM_EXTENSIONS

# Check for a usable 'jar' program
AC_CHECK_PROGS([JAR], [jar fastjar], [no])

AS_IF([test "x$JAR" = xno],
    [AS_IF([test "x$jar_support" = xyes],
        [AC_MSG_WARN(['jar' not found, JAR file support disabled])]
    )
    AS_VAR_SET(jar_support, no)]
)

################################################################################
# Check for libraries                                                          #
################################################################################

# JAR support
PKG_CHECK_ZZIPLIB([ZZIP],
   [0.13.00],
   [AS_VAR_SET(have_zzip, yes)],
   [AS_VAR_SET(have_zzip, no)]
)

AS_IF([test "x$jar_support" = xyes],
    [AS_IF([test "x$have_zzip" = xyes],
        [LIBS="$LIBS $ZZIP_LIBS"
        CFLAGS="$CFLAGS $ZZIP_CFLAGS"
        AC_SUBST([LIBS])
        AC_SUBST([CFLAGS])
        AC_DEFINE([JEL_JARFILE_SUPPORT], [1])],
        [AS_VAR_SET(jar_support, no)
        AC_MSG_WARN([JAR file support disabled, zziplib may not be installed or you may have an outdated version])]
    )],
)

# POSIX threads
ACX_PTHREAD([AS_VAR_SET(have_pthread, yes)])

# libpth
AC_CHECK_PTH([2.0.0], [], [], [no], [AS_VAR_SET(have_pth, yes)])

# Select the thread model
AS_IF([test "x$thread_model" = xauto],
    [AS_IF([test "x$have_pthread" = xyes],
        [AS_VAR_SET(thread_model, pthread)],
        [AS_IF([test "x$have_pth" = xyes],
            [AS_VAR_SET(thread_model, pth)],
            [AS_VAR_SET(thread_model, none)]
        )]
    )]
)

AS_CASE(AS_VAR_GET(thread_model),
    [pthread],
    [AS_IF([test "x$have_pthread" = xyes],
        [AC_DEFINE([JEL_THREAD_POSIX], [1])
        LIBS="$LIBS $PTHREAD_LIBS"
        CFLAGS="$CFLAGS $PTHREAD_CFLAGS"
        CC="$PTHREAD_CC"
        AC_SUBST([LIBS])
        AC_SUBST([CFLAGS])
        AC_SUBST([CC])],
        [AC_MSG_ERROR([POSIX threads headers and libraries were not found])]
    )],
    [pth],
    [AS_IF([test "x$have_pth" = xyes],
        [AC_DEFINE([JEL_THREAD_PTH], [1])
        CPPFLAGS="$CPPFLAGS $PTH_CPPFLAGS"
        CFLAGS="$CFLAGS $PTH_CFLAGS"
        LDFLAGS="$LDFLAGS $PTH_LDFLAGS"
        LIBS="$LIBS $PTH_LIBS"
        AC_SUBST([CPPFLAGS])
        AC_SUBST([CFLAGS])
        AC_SUBST([LDFLAGS])
        AC_SUBST([LIBS])],
        [AC_MSG_ERROR([GNU Pth was not found])]
    )],
    [none],
    [AC_DEFINE([JEL_THREAD_NONE], [1])],
    [AC_MSG_ERROR([Unknown thread model: AS_VAR_GET(thread_model)])]
)

# Deal with finalization support
AS_IF([test "x$finalizer" = xyes],
    [AS_IF([test "x$thread_model" = xnone],
        [AS_VAR_SET(finalizer, no)
        AC_MSG_WARN([Finalization disabled, thread support is required])]
    )],
    [AS_IF([test "x$jar_support" = xyes],
        [AC_MSG_WARN([JAR support enabled without finalization, this might cause problems when using java.class.GetResourceAsStream()])]
    )]
)

AS_IF([test "x$finalizer" = xyes],
    [AC_DEFINE([JEL_FINALIZER], [1])]
)

# Floating point support
AC_CHECK_LIBM

AS_IF([test "x$fp_support" = xyes],
    [LIBS="$LIBS $LIBM"
    AC_SUBST([LIBM])]
)

# Other libraries
AC_SEARCH_LIBS([iconv], [iconv]) # Needed on some hosts for the preverifier
AC_SEARCH_LIBS([clock_gettime], [rt])

################################################################################
# Check for header files                                                       #
################################################################################

# Check for the standard C includes
AC_HEADER_STDC

AS_IF([test "x$ac_cv_header_stdc" != xyes],
    [AC_MSG_WARN([You don't seem to have standard C headers])]
)

AC_HEADER_STDBOOL
AC_HEADER_TIME

AC_CHECK_HEADERS([arpa/inet.h dirent.h fcntl.h iconv.h inttypes.h langinfo.h math.h stdarg.h sys/stat.h sys/types.h unistd.h])

AC_CHECK_HEADERS([sys/socket.h netinet/in.h netdb.h], [],
    [AS_VAR_SET(socket_support, no)])

################################################################################
# Check for types                                                              #
################################################################################

# Check for the presence of size_t
AC_TYPE_SIZE_T

# Check the size of pointer and long integer types
AC_CHECK_SIZEOF([void *])
AC_CHECK_SIZEOF([long])
AC_CHECK_SIZEOF([long long])

# Check for intX_t types
AC_TYPE_INT8_T
AC_TYPE_UINT8_T
AC_TYPE_INT16_T
AC_TYPE_UINT16_T
AC_TYPE_INT32_T
AC_TYPE_UINT32_T
AC_TYPE_INT64_T
AC_TYPE_UINT64_T
AC_TYPE_INTPTR_T
AC_TYPE_UINTPTR_T

# Check for the presence of ptrdiff_t
AC_CHECK_TYPES([ptrdiff_t])

################################################################################
# Check for structures                                                         #
################################################################################

# Unused

################################################################################
# Check for compiler features                                                  #
################################################################################

AC_C_CONST
AC_C_INLINE
AC_C_BIGENDIAN
AX_C_FLOAT_WORDS_BIGENDIAN

# Check for built-in functions
ACX_CHECK_BUILTIN([__builtin_nan], [double my_nan =], [""])

# Check for function attributes

ACX_CHECK_ATTRIBUTE([const], [int], [void])
ACX_CHECK_ATTRIBUTE([malloc], [void *], [void])
ACX_CHECK_ATTRIBUTE([noreturn], [void], [void])
ACX_CHECK_ATTRIBUTE([pure], [int], [void])

# Check for label-as-values gcc extension

AC_MSG_CHECKING([for gcc's labels-as-values extension])
AC_COMPILE_IFELSE(
    [AC_LANG_SOURCE(
        [[
            int main()
            {
                void *happy_label;

                happy_label = &&happy;
                goto *happy_label;

            happy:
                return 0;
            }
        ]]
    )],
    [AC_MSG_RESULT([yes])
    AS_VAR_SET(have_labels_as_values, yes)],
    [AC_MSG_RESULT([no])]
)

AS_IF([test "x$threaded" = xauto && test "x$have_labels_as_values" = xyes],
    [AS_VAR_SET(threaded, yes)
    AC_DEFINE([JEL_THREADED_INTERPRETER], [1])]
)

# Check for local storage support
AX_TLS

################################################################################
# Check for functions                                                          #
################################################################################

AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_FUNC_VPRINTF
AC_CHECK_FUNC([vsnprintf], , [AC_MSG_ERROR([snprintf() is required])])
AC_CHECK_FUNC([memcpy], , [AC_LIBOBJ([memcpy])])
AC_CHECK_FUNC([memmove], , [AC_LIBOBJ([memmove])])
AC_CHECK_FUNCS([gettimeofday clock_gettime])
ACX_FUNC_VA_COPY

# Tests for fp support
AC_CHECK_FUNCS([fmod sin cos tan sqrt ceil floor], ,
    [AS_IF([test "x$fp_support" = xyes],
        [AC_MSG_ERROR([Standard math.h functions are needed for FP support])]
    )]
)

# Thread related tests
AS_IF([test "x$thread_model" = xpthread],
    [AC_CHECK_FUNCS([pthread_yield pthread_yield_np])]
)

################################################################################
# Check for system services                                                    #
################################################################################

# Unused

################################################################################
# Do automatic tests                                                           #
################################################################################

# Check for floating point support

AS_IF([test "x$fp_support" = xyes],
    [AC_DEFINE([JEL_FP_SUPPORT], [1])]
)

# Check for jar support

AS_IF([test "x$jar_support" = xyes],
    [AS_VAR_SET(classpath, "classpath.jar")],
    [AS_VAR_SET(classpath, "classpath/")]
)

# Check for socket support

AS_IF([test "x$socket_support" = xyes],
    [AC_DEFINE([JEL_SOCKET_SUPPORT], [1])]
)

# Set preverifier path

AS_IF([test "x$want_preverifier" = xyes],
    [AS_VAR_SET(preverifier, "../preverifier/preverifier")],
    [AS_VAR_SET(preverifier, "preverifier")]
)

AS_IF([test "x$cross_compiling" = xyes],
    [AS_VAR_SET(preverifier, "preverifier")]
)

# Set output variables

AM_CONDITIONAL([COND_PREVERIFIER], [test "x$want_preverifier" = xyes])
AC_SUBST([preverifier])
AC_SUBST([classpath])
AC_SUBST([jar_support])

AC_CONFIG_FILES([
    Makefile
    Doxyfile
    docs/Makefile
    m4/Makefile
    src/Makefile
    src/classpath/Makefile
    src/preverifier/Makefile
    src/jelatine/Makefile
])

AC_OUTPUT

#####################
# Print configuration
#####################
cat <<EOF | tee -a config.log

## -------------- ##
## Configuration. ##
## -------------- ##

Configuration for jelatine $VERSION
        Build preverifier: $want_preverifier
        With floating-point support: $fp_support
        With JAR support: $jar_support
        With socket support: $socket_support
        With threaded interpreter: $threaded
        With thread model: $thread_model
        With finalization: $finalizer
        Debugging: $debug

EOF
