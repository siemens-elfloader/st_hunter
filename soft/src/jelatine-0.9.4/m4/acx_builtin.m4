#
# SYNOPSIS
#
#   ACX_CHECK_BUILTIN(BUILTIN, [RESULT], [PARAMS])
#
# DESCRIPTION
#
#   Check if the compiler supports one of gcc's builtin functions. The builtins
#   may also be provided by other compilers. The BUILTIN parameter is the name
#   of the builtin function, the RESULT is the source which will hold the
#   value returned by the builtin (if any) and the PARAMS contains the
#   parameters passed to the builtin funciton. For example checking for
#   __builtin_clz():
#
#   ACX_CHECK_BUILTIN([__builtin_clz], [int val =], [1])
#
# LAST MODIFICATION
#
#   2009-01-26
#
# COPYLEFT
#
#   Copyright © 2008-2009 Gabriele Svelto <gabriele.svelto@gmail.com>
#
#   Copying and distribution of this file, with or without
#   modification, are permitted in any medium without royalty provided
#   the copyright notice and this notice are preserved.

AC_DEFUN([ACX_CHECK_BUILTIN],
    [AS_VAR_PUSHDEF([ac_var], [acx_cv_have_$1])
    AC_CACHE_CHECK(
        [for $1],
        ac_var,
        [AC_LINK_IFELSE([AC_LANG_PROGRAM([], [[$2 $1($3);]])],
            [AS_VAR_SET(ac_var, yes)],
            [AS_VAR_SET(ac_var, no)])])

    AS_IF([test AS_VAR_GET(ac_var) = yes],
        [AC_DEFINE_UNQUOTED(AS_TR_CPP(HAVE_$1), 1,
            [Define to 1 if the system has the `$1' built-in function])],
        [])
    AS_VAR_POPDEF([ac_var])])
