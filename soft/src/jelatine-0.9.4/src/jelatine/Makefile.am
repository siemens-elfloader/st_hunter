## Process this file with automake to produce Makefile.in

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

AM_CPPFLAGS = -DJEL_CLASSPATH_DIR='"${pkgdatadir}/${classpath}"'

bin_PROGRAMS = jelatine

jelatine_SOURCES = \
    array.c array.h \
    bytecode.h bytecode.c \
    classfile.c classfile.h \
    constantpool.c constantpool.h \
    class.c class.h \
    field.c field.h \
    header.c header.h \
    interpreter.c interpreter.h \
    java_lang_Class.h \
    java_lang_Exception.h \
    java_lang_String.h \
    java_lang_Thread.h \
    java_lang_Throwable.h \
    java_lang_ref_Reference.h \
    java_lang_ref_WeakReference.h \
    jelatine_VMResourceStream.h \
    jstring.c jstring.h \
    kni.c kni.h \
    loader.c loader.h \
    main.c \
    memory.c memory.h \
    method.c method.h \
    native.c native.h \
    opcodes.h \
    print.c print.h \
    thread.c thread.h \
    utf8_string.c utf8_string.h \
    util.c util.h \
    vm.c vm.h \
    wrappers.h
