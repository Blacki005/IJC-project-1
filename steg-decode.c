/*
file: primes.c
name: The C programming language - project 1 A.) (https://www.fit.vutbr.cz/study/courses/IJC/public/DU1.html.en)
created: 24. 2. 2023
tools used: VS Code, Vim, GDB, Valgrind, gcc v11.3.0
description: This program takes name of .ppm file as command line argument and prints to stdout message hidden in LSB of prime bytes starting with START_PRIME. Maximal length of hidden message is determined by MAXIMAL_MESSAGE_LENGTH.
Restrictions: Message length is restricted by MAX_MESSAGE_LENGTH, start prime is set to START_PRIME.
*/

#include "ppm.h"
#include "eratosthenes.h"

#define START_PRIME 101
#define MAX_MESSAGE_LENGTH 500

/*
 * Function below is copied from: https://www.cl.cam.ac.uk/~mgk25/ucs/utf8_check.c
 * Author: Dr Markus Kuhn
 * Description: 
    * The utf8_check() function scans the '\0'-terminated string starting
    * at s. It returns a pointer to the first byte of the first malformed
    * or overlong UTF-8 sequence found, or NULL if the string contains
    * only correct UTF-8. It also spots UTF-8 sequences that could cause
    * trouble if converted to UTF-16, namely surrogate characters
    * (U+D800..U+DFFF) and non-Unicode positions (U+FFFE..U+FFFF).
 * Author's website: <http://www.cl.cam.ac.uk/~mgk25/> -- 2005-03-30
 * License: http://www.cl.cam.ac.uk/~mgk25/short-license.html
 */

unsigned char *utf8_check(unsigned char *s)
{
  while (*s) {
    if (*s < 0x80)
      /* 0xxxxxxx */
      s++;
    else if ((s[0] & 0xe0) == 0xc0) {
      /* 110XXXXx 10xxxxxx */
      if ((s[1] & 0xc0) != 0x80 ||
	  (s[0] & 0xfe) == 0xc0)                        /* overlong? */
	return s;
      else
	s += 2;
    } else if ((s[0] & 0xf0) == 0xe0) {
      /* 1110XXXX 10Xxxxxx 10xxxxxx */
      if ((s[1] & 0xc0) != 0x80 ||
	  (s[2] & 0xc0) != 0x80 ||
	  (s[0] == 0xe0 && (s[1] & 0xe0) == 0x80) ||    /* overlong? */
	  (s[0] == 0xed && (s[1] & 0xe0) == 0xa0) ||    /* surrogate? */
	  (s[0] == 0xef && s[1] == 0xbf &&
	   (s[2] & 0xfe) == 0xbe))                      /* U+FFFE or U+FFFF? */
	return s;
      else
	s += 3;
    } else if ((s[0] & 0xf8) == 0xf0) {
      /* 11110XXX 10XXxxxx 10xxxxxx 10xxxxxx */
      if ((s[1] & 0xc0) != 0x80 ||
	  (s[2] & 0xc0) != 0x80 ||
	  (s[3] & 0xc0) != 0x80 ||
	  (s[0] == 0xf0 && (s[1] & 0xf0) == 0x80) ||    /* overlong? */
	  (s[0] == 0xf4 && s[1] > 0x8f) || s[0] > 0xf4) /* > U+10FFFF? */
	return s;
      else
	s += 4;
    } else
      return s;
  }

  return NULL;
}

int main (int argc, char **argv) {
    if (argc > 2) {
      error_exit("Too many arguments! Expected: ./steg-decode <filename.ppm>");
    }

    //nacte data a dimenze .ppm obrazku do struktury ppm
    struct ppm *img_data = ppm_read(argv[1]);
    if (img_data == NULL) {
      error_exit("Unable to read the file!");
    };

    const unsigned data_size = 3 * img_data->xsize * img_data->ysize;

    //alokuje/vytvori bitove pole o velikosti rovne velikosti .ppm obrazku v bytech, v tomto poli oznaci prvociselne byty
    bitset_alloc(bitfield, data_size);
    if (bitfield == NULL) {
      ppm_free(img_data);
      error_exit("Error allocating memory!");
    }

    Eratosthenes(bitfield);

    //iteruje pres byty s prvociselnym indexem a uklada jejich LSB hodnoty do c, ktere se pak zapisi do stringu message
    unsigned char LSB = 0;
    unsigned char shift = 0;
    unsigned char c;
    //message a message index slouzi k ulozeni zpravy do stringu a kontrole spravnosti UTF-8 kodovani a ukonceni stringu
    int mess_index = 0;
    unsigned char message[MAX_MESSAGE_LENGTH];

    for (unsigned i = START_PRIME;i < bitset_size(bitfield); i+=2) {
        if (bitset_getbit(bitfield, i) == 0) {
            
            LSB = (img_data->data[i + 1] & 1);
            //postupne plni char c LSB, dokud nenaplni 8 bitu
            c |= LSB << shift;
            shift++;

            //kompletni byte tiskne na stdout a nuluje shift citac
            if (shift == 8) {
              message[mess_index] = c;

              if (c == '\0') {
                  break;
              }

            mess_index++;

            if (mess_index >= MAX_MESSAGE_LENGTH) {
              ppm_free(img_data);
              bitset_free(bitfield);
              error_exit("Hidden message is too long, change MAX_MESSAGE_LENGTH");
            }
            c = 0;
            shift = 0;
            }
        }
    }

    //kontrola validity zpravy (UTF-8 a zakonceni '\0') a jeji tisk na stdout
    if (utf8_check(message) != NULL) {
        ppm_free(img_data);
        bitset_free(bitfield);
        error_exit("Message contains malformed UTF-8 sequence!");
    }
    if (message[mess_index] != '\0') {
        ppm_free(img_data);
        bitset_free(bitfield);
        error_exit("Message string does not contain terminating character!");
    }

    printf("%s\n", message);

    ppm_free(img_data);
    bitset_free(bitfield);
    return 0;
}

