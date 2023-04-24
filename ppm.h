/*
file: eratosthenes.h
name: The C programming language - project 1 A.) (https://www.fit.vutbr.cz/study/courses/IJC/public/DU1.html.en)
created: 16. 3. 2023
tools used: VS Code, Vim, GDB, Valgrind, gcc v11.3.0
description: Interface for ppm.c. Defines structure for storing ppm dimensions and data.
*/
#ifndef PPM_H
#define PPM_H

#include "error.h"
#include <ctype.h>
#include <string.h>

#define MAX_X 16000
#define MAX_Y 16000

struct ppm {
    unsigned xsize;
    unsigned ysize;
    char data[];   // RGB bajty, celkem 3*xsize*ysize
};

struct ppm *ppm_read(const char * filename);

void ppm_free(struct ppm *p);

#endif //PPM_H
