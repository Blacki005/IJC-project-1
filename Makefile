#file: Makefile
#name: The C programming language - project 1 A.) (https://www.fit.vutbr.cz/study/courses/IJC/public/DU1.html.en)
#created: 19. 3. 2023
#tools used: VS Code, Vim, GDB, Valgrind, gcc v11.3.0
#description: Makefile that builds executable files "primes" (uses macros), "primes-i" (uses inline functions) and "steg-decode". "make run" sets stack memory to 30MB and runs all three programs.
CFLAGS = -std=c11 -pedantic -Wall -Wextra


all: primes primes-i steg-decode

run: steg-decode primes-i primes
	./steg-decode du1-obrazek.ppm
	ulimit -s 30000 && ./primes
	ulimit -s 30000 && ./primes-i

clean:
	rm *.o

#varianta bez inline funkci:
primes: primes.o eratosthenes.o error.o
	gcc $(CFLAGS) primes.o eratosthenes.o error.o -o primes
############################

#varianta s inline funkcemi:
primes-i: primes-i.o eratosthenes.o error.o
	gcc $(CFLAGS) primes-i.o eratosthenes.o error.o -o primes-i
############################

#steg decode:
steg-decode: ppm.o eratosthenes.o error.o steg-decode.o
	gcc $(CFLAGS) ppm.o eratosthenes.o error.o steg-decode.o -o steg-decode
#############

#creating object files:
primes-i.o: primes.c eratosthenes.c bitset.h error.h
	gcc $(CFLAGS) -DUSE_INLINE -c primes.c -o primes-i.o

steg-decode.o: steg-decode.c ppm.h error.h eratosthenes.h bitset.h
	gcc $(CFLAGS) -c steg-decode.c -o steg-decode.o

primes.o: primes.c eratosthenes.h bitset.h error.h
	gcc $(CFLAGS) -c primes.c -o primes.o

eratosthenes.o: eratosthenes.c eratosthenes.h bitset.h error.h
	gcc $(CFLAGS) -c eratosthenes.c -o eratosthenes.o

error.o: error.c error.h
	gcc $(CFLAGS) -c error.c -o error.o

ppm.o: ppm.c ppm.h error.h
	gcc $(CFLAGS) -c ppm.c -o ppm.o
