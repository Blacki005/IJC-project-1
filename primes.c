/*
file: primes.c
name: The C programming language - project 1 A.) (https://www.fit.vutbr.cz/study/courses/IJC/public/DU1.html.en)
created: 24. 2. 2023
tools used: VS Code, Vim, GDB, Valgrind, gcc v11.3.0
description: This program creates bit field of size 230,000,000 and sets all bits with non-prime odd index to '1'. 10 higest primes are outputted to stdout, time of execution is outputed to stderr.
*/

#include <time.h>
#include "eratosthenes.h"

int main () {
	//spusti odpocet doby vypoctu, alokuje/vytvori pole o velikosti 230000000 a nulovane pole pro 10 nejvyssich prvocisel
	double start = clock();
	bitset_create(pole, 230000000);
	unsigned highest_primes[10] = {0,};

	//nastavi bity s lichym, neprvociselnym indexem na 1
	Eratosthenes(pole);

	//iteruje pres pole a uklada 10 nejvyssich prvocisel do pole "highest_primes"
	int flag = 9;
	for (unsigned i = 229999999; flag >= 0; i-=2) {

		if(bitset_getbit(pole, i) == 0) {
			highest_primes[flag] = i;
			flag--;
		}
	}

	for (unsigned i = 0; i < 10; i++) {
		printf("%u\n", highest_primes[i]);
	}

	//bitset_free(pole); //uncomment in case of using bitset_alloc
	
	fprintf(stderr, "Time=%.3g\n", (double)(clock()-start)/CLOCKS_PER_SEC);
	return 0;
}

