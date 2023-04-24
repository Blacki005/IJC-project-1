/*
file: eratosthenes.h
name: The C programming language - project 1 A.) (https://www.fit.vutbr.cz/study/courses/IJC/public/DU1.html.en)
created: 24. 2. 2023
tools used: VS Code, Vim, GDB, Valgrind, gcc v11.3.0
description: Defines functions to print warning and error messages to stderr. Both functions can take variable count of arguments.
*/

#include "error.h"

//Vytiskne varovani na stderr, neukonci program.
void warning(const char *fmt, ...) {
    va_list argv;
    va_start(argv, *fmt);

    fprintf(stderr, "Warning: ");
    vfprintf(stderr, fmt, argv);
    fprintf(stderr, "\n");

    va_end(argv);
    return;
}

//Vytiskne chybovou hlasku na stderr, korektne ukonci program s chybovym kodem 1.
void error_exit(const char *fmt, ...) {
    va_list argv;
    va_start(argv, *fmt);

    fprintf(stderr, "Error: ");
    vfprintf(stderr, fmt, argv);
    fprintf(stderr, "\n");

    va_end(argv);
    exit(1);
}
