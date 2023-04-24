/*
file: eratosthenes.h
name: The C programming language - project 1 A.) (https://www.fit.vutbr.cz/study/courses/IJC/public/DU1.html.en)
created: 16. 3. 2023
tools used: VS Code, Vim, GDB, Valgrind, gcc v11.3.0
description: Defines functions for opening ppm file and storing its dimensions + data in structure, and for freeing this structure's allocated memory on heap.
restrictions: Resolution of ppm image is set to be max. 16000 * 16000 bytes.
*/

#include "ppm.h"

//funkce otevre soubor filename a ulozi z nej data do struktury ppm. Soubor musi byt ve formatu ppm.
struct ppm *ppm_read(const char *filename) {

    //otevreni souboru a kontrola uspesnoti otevreni
    FILE *soubor = fopen(filename, "rb");
    if (soubor == NULL) {
        error_exit("Error reading file!");
    };

    //alokace pameti pro strukturu ukladajici dimenze obrazku
    struct ppm *img_data = calloc(1, sizeof(struct ppm));
    if (img_data == NULL) {
        error_exit("Error allocating memory!");
    }

    //cteni formatu souboru a overeni spravnosti
    char format[3] = {'\0'};
    fgets(format, sizeof(format), soubor);
    if (strncmp(format, "P6", 2)) {
        warning("Incorrect file format!");
        ppm_free(img_data);
        return NULL;
    }

    //precteni dimenzi a barevne hloubky obrazku
    fscanf(soubor, "%u %u", &(img_data->xsize), &(img_data->ysize));
    if (img_data->xsize > MAX_X || img_data->ysize > MAX_Y) {
        ppm_free(img_data);
        error_exit("Image is too large!");
    }

    int color_depth = 0;
    fscanf(soubor, "%d", &color_depth);

    //realokace pameti pro data obrazku ve strukture ppm
    img_data = realloc(img_data, sizeof(struct ppm) + 3 * img_data->xsize * img_data->ysize);
    if (img_data == NULL) {
        error_exit("Error allocating memory!");
    };

    //kopirovani dat obrazku do struktury ppm
    char c;
    for (int i = 0; (c = fgetc(soubor)) != EOF; i++) {
        img_data->data[i] = c;
    }
    
    if (fclose(soubor) == EOF) {
        warning("The file was not closed successfully!");
    }

    return img_data;
}

void ppm_free(struct ppm *p) {
    free(p);
}
