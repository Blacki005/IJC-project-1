/*
file: eratosthenes.c
name: The C programming language - project 1 A.) (https://www.fit.vutbr.cz/study/courses/IJC/public/DU1.html.en)
created: 24. 2. 2023
tools used: VS Code, Vim, GDB, Valgrind, gcc v11.3.0
description: This program expects pointer to nulled bit field as input. It sets bits with odd, non-prime index to '1'.
*/
//
#include "eratosthenes.h"

void Eratosthenes (bitset_t bitset) {

    //nastaveni hodnot bitu na indexu 0 a 1 na '1' - 0 a 1 nejsou prvocisla
    bitset_setbit(bitset, 0, 1);
    bitset_setbit(bitset, 1, 1);

    //iterace skrz licha cisla mensi nez sqrt(velikost) - i*i je pouzito, aby modul nevyzadoval zahrnuti <math.h>
    for (bitset_index_t i = 3; (i*i) <= bitset_size(bitset); i += 2) {

        //nastaveni vsech lichych nasobku prvocisel na '1'
        if (bitset_getbit(bitset, i) == 0) {

            //k j se pricitaji dvojnasobky prvocisel, protoze soucet lichych cisel je vzdy cislo sude a tedy ho algoritmus neuvazuje
            for (bitset_index_t j = i*i; j < bitset_size(bitset); j += i*2) {
                bitset_setbit(bitset, j, 1);
            };
        };
    };
    return;
}
