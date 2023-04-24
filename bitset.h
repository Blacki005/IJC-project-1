/*
file: bitset.h
name: The C programming language - project 1 A.) (https://www.fit.vutbr.cz/study/courses/IJC/public/DU1.html.en)
created: 24. 2. 2023
tools used: VS Code, Vim, GDB, Valgrind, gcc v11.3.0
description: This file defines macros and inline functions needed for creating or allocating bit field and getting/setting specific bits. Bit field is array of unsigned long values. Size of the field (allocated memory) is stored at pos. 0.
*/

#ifndef BITSET_H
#define BITSET_H

#include <assert.h>
#include "error.h"

//definice datovych typu a maker
typedef unsigned long *bitset_t;
typedef unsigned long bitset_index_t;

#define UL_SIZE sizeof(unsigned long)
#define UL_BIT_SIZE (sizeof(unsigned long) * 8)

/*-----------------------------------------------------
||              DEFINICE NEZBYTNYCH MAKER            ||
-----------------------------------------------------*/

//Vytvori na zasobniku bitove pole o velikosti "size" a ulozi "size" na 0. pozici pole. Inicializuje pole hodnotami '0'.
#define bitset_create(bitfield, size)  \
    static_assert(size > 0, "bitset_alloc: Unable to allocate memory");    \
    \
    unsigned long bitfield[1 + (size / UL_BIT_SIZE) + ((size % UL_BIT_SIZE) ? 1 : 0)] = {0,};   \
    \
    bitfield[0] = size           
 

//Vytvori na hromade bitove pole o velikosti "size" a ulozi "size" na 0. pozici pole. Calloc inicializuje pole hodnotami '0'.
#define bitset_alloc(bitfield, size)   \
    assert(size > 0);    \
    \
    bitset_t bitfield = calloc((1 + (size / UL_BIT_SIZE) + ((size % UL_BIT_SIZE) ? 1 : 0)), UL_SIZE);  \
    \
    (bitfield == NULL) ? (error_exit("bitset_alloc: Unable to allocate memory"),0) : (bitfield[0] = size, 0)

#ifndef USE_INLINE
/*------------------------------------------------------
||              DEFINICE PODMINENYCH MAKER            ||
------------------------------------------------------*/

#define bitset_free(bitfield)(free(bitfield))

//Vraci pocet bitu v bitovem poli vytvorenem makry bitset_create nebo bitset_alloc.
#define bitset_size(bitfield)(bitfield[0])

//Ekvivalent funkce bitset_setbit upraveny do ternarniho vyrazu.
#define bitset_setbit(bitfield, index, value)   \
    (index < bitset_size(bitfield)) ?   \
    (   \
    (value) ?   \
    (bitfield[1 + (index / UL_BIT_SIZE)] |= (1UL << index % UL_BIT_SIZE)) : \
    (bitfield[1 + (index / UL_BIT_SIZE)] ^= (1UL << index % UL_BIT_SIZE))   \
    ) : \
    (error_exit("bitset_getbit: Index %lu out of range 0..%lu", index, bitfield[0]), 1)

//Ekvivalent funkce bitset_getbit upraveny do ternarniho vyrazu.
#define bitset_getbit(bitfield, index) (    \
    (index < bitset_size(bitfield)) ?   \
    (1 & (bitfield[(index + UL_BIT_SIZE) / UL_BIT_SIZE] >> ((index + UL_BIT_SIZE) % (UL_BIT_SIZE)))) :  \
    (error_exit("bitset_getbit: Index %lu out of range 0..%lu", index, bitfield[0]), 1)   \
    )

#else
/*-------------------------------------------------------
||             DEFINICE PODMINENYCH FUNKCI             ||
-------------------------------------------------------*/

static inline void bitset_free(bitset_t bitfield) {
    free(bitfield);
}

//Vraci pocet bitu v bitovem poli vytvorenem makry bitset_create nebo bitset_alloc.
static inline unsigned long bitset_size (bitset_t bitfield) {
    return bitfield[0];
}

//Nastavi v bitovem poli "bitfield" bit na indexu "bit_index" na 0 pokud je "value" = 0 a na 1 pokud je "value" nenulovy vyraz.
static inline void bitset_setbit(bitset_t bitfield, bitset_index_t index, long long value) {

    //kontrola zda index neni za hranicemi pole
    if (index >= bitset_size(bitfield)) {
        error_exit("bitset_setbit: Index %lu out of range 0..%lu", index, bitset_size(bitfield));
    }

    if (value) {
        //index % UL_BIT_SIZE odecte od indexu vsechny nasobky velikosti unsigned long, o tuto hodnotu se bitove posune hodnota 1UL (=uns. long) a vytvori se tak bitova maska pro konkretni polozku pole
        bitset_index_t bitmask = 1UL << index % UL_BIT_SIZE;

        //odkaz do bitfield je zvetseny o 1, protoze prvni polozka obsahuje velikost pole
        //v poli na polozce odpovidajici indexu se aplikuje bitova maska
        bitfield[1 + index/UL_BIT_SIZE] |= bitmask;
        
    }
    else {
        //vyraz je nulovy a bitova maska se nastavi na 0 (=toggle - nepredpoklada se nastavovani bitu na stejnou hodnotu jakou uz ma)
        bitset_index_t bitmask = 0;

        //toggle hodnoty bitu XORem
        bitfield[1 + index / UL_BIT_SIZE] ^= bitmask;
    }
}

//Vraci hodnotu bitu na indexu "index" v poli "bitfield"
static inline signed char bitset_getbit(bitset_t bitfield ,bitset_index_t index) {

    //kontrola zda index neni za hranicemi pole
    if (index >= bitset_size(bitfield)) {
        error_exit("bitset_getbit: Index %lu out of range 0..%lu", index, bitset_size(bitfield));
    }    
    
    //k indexu se pricte velikost unsigned long kvuli prvnimu mistu v poli, ktere obsahuje velikost pole
    index += UL_BIT_SIZE;

    //glob_index je globalni index pro pole (tj. index unsigned long ve kterem se bit nachazi)
    unsigned glob_index = index / UL_BIT_SIZE;

    //v polozce pole ve ktere se bit nachazi se provede posun o bit_index % UL_BIT_SIZE a pokud je hledany bit 1, vysledkem je liche cislo
    signed char bit_value = bitfield[glob_index] >> (index % (UL_BIT_SIZE));

    //pokud je cislo liche, vrati 1 a jinak 0
    return 1 & bit_value;
}

#endif

#endif //BITSET_H
