/*
file: eratosthenes.h
name: The C programming language - project 1 A.) (https://www.fit.vutbr.cz/study/courses/IJC/public/DU1.html.en)
created: 24. 2. 2023
tools used: VS Code, Vim, GDB, Valgrind, gcc v11.3.0
description: Interface for "error.c".
*/

#ifndef ERROR_H
#define ERROR_H

#include <stdlib.h>
#include <stdio.h>
#include "stdarg.h"

void warning(const char *fmt, ...);

void error_exit(const char *fmt, ...);

#endif //ERROR_H
