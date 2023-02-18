/** @file rsa.h*/ 

#ifndef _RSA_H
#define _RSA_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>

# define RSA_SIEVE_LIMIT 255

/**
 * Sieve of Eratosthenes Algorithm
 * https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes
 * 
 * @param limit A limit
 * @param primes_sz The size of the generated primes list. Empty argument used as ret val
 *
 * @return The prime numbers that are less or equal to the limit
 */
size_t* sieve_of_eratosthenes(size_t , size_t *);


/**
 * Greatest Common Divisor
 *
 * @param a The first number
 * @param b The second number
 *
 * @return The GCD of the two numbers
 */
size_t gcd(size_t , size_t );


/**
 * Chooses 'e' where 
 *     1 < e < fi(n) AND gcd(e, fi(n)) == 1
 *
 * @param primes table with primes (from sieve_of_eratosthenes() )
 * @param primeLen size of table with primes
 * @param fin fi(n)
 *
 * @return The chosen 'e'
 */
size_t choose_e(size_t *, size_t , size_t );


/**
 * Calculates the modular inverse x so that ax=1(modm)
 * Uses Extended Euclidean Algorithm
 *
 * @param a a
 * @param m m
 *
 * @return x
 */
size_t mod_inverse(size_t , size_t );


/**
 * Generates an RSA key pair and saves
 * each key in a different file
 */
void rsa_keygen(void);


/**
 * Encrypts an input file and dumps the ciphertext into an output file
 *
 * @param input_file path to input file
 * @param output_file path to output file
 * @param key_file path to key file
 */
void rsa_encrypt(char *, char *, char *);


/**
 * Decrypts an input file and dumps the plaintext into an output file
 *
 * @param input_file path to input file
 * @param output_file path to output file
 * @param key_file path to key file
 */
void rsa_decrypt(char *, char *, char *);

#endif /* _RSA_H */
