#include "ske.h"
#include "prf.h"
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* memcpy */
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#ifdef LINUX
#define MMAP_SEQ MAP_PRIVATE|MAP_POPULATE
#else
#define MMAP_SEQ MAP_PRIVATE
#endif

/* NOTE: since we use counter mode, we don't need padding, as the
 * ciphertext length will be the same as that of the plaintext.
 * Here's the message format we'll use for the ciphertext:
 * +------------+--------------------+-------------------------------+
 * | 16 byte IV | C = AES(plaintext) | HMAC(C) (32 bytes for SHA256) |
 * +------------+--------------------+-------------------------------+
 * */

/* we'll use hmac with sha256, which produces 32 byte output */
#define HM_LEN 32
#define KDF_KEY "qVHqkOVJLb7EolR9dsAMVwH1hRCYVx#I"
/* need to make sure KDF is orthogonal to other hash functions, like
 * the one used in the KDF, so we use hmac with a key. */

/** Generates HMAC and AES KEY
 * Hash KDF_KEY with SHA-512 to get 32 bytes of key
 * and split in half for 
 * HMAC and other half for AES
 * 
 * precondition: 
 * K = SKE_KEY object containing HMAC and AES Key
 * entropy = random
 * entLen = length of entropy
 * 
 * postcondition:
 * K's HMAC and AES Key gets updated
 */

int ske_keyGen(SKE_KEY* K, unsigned char* entropy, size_t entLen)
{
	/* TODO: write this.  If entropy is given, apply a KDF to it to get
	 * the keys (something like HMAC-SHA512 with KDF_KEY will work).
	 * If entropy is null, just get a random key (you can use the PRF). */

	// Variable for temporary key storage of length KLEN_SKE
	// Note KLEN_SKE is 32
	size_t i = KLEN_SKE*2;
	unsigned char tempKey[i];

	// If entropy is given apply KDF - HMACSHA512 elseif is null randBytes for random key
	if(entropy)
	{
		/* Computes the MAC of the entLen bytes at entropy using hash
		 * function EVP_sha512 and the key, KDF_KEY which is HM_LEN 
		 * bytes long
		 * 
		 * Output goes in tempKey and size in NULL
		 */
		HMAC(EVP_sha512(),KDF_KEY,HM_LEN,entropy,entLen,
				tempKey,NULL);
	}
	else
	{
		/* Random key of KLEN_SKE length
		 *
		 * Output goes in tempKey
		 */ 
		randBytes(tempKey,i);
	}

	// Copy values into the associated Keys in the object K
	memcpy(K->hmacKey, tempKey//lower tempkey, KLEN_SKE);
       	memcpy(K->aesKey, tempKey//upper tempkey, KLEN_SKE);	
	return 0;
}
size_t ske_getOutputLen(size_t inputLen)
{
	return AES_BLOCK_SIZE + inputLen + HM_LEN;
}
size_t ske_encrypt(unsigned char* outBuf, unsigned char* inBuf, size_t len,
		SKE_KEY* K, unsigned char* IV)
{
	/* TODO: finish writing this.  Look at ctr_example() in aes-example.c
	 * for a hint.  Also, be sure to setup a random IV if none was given.
	 * You can assume outBuf has enough space for the result. */
	return 0; /* TODO: should return number of bytes written, which
	             hopefully matches ske_getOutputLen(...). */
}
size_t ske_encrypt_file(const char* fnout, const char* fnin,
		SKE_KEY* K, unsigned char* IV, size_t offset_out)
{
	/* TODO: write this.  Hint: mmap. */
	return 0;
}
size_t ske_decrypt(unsigned char* outBuf, unsigned char* inBuf, size_t len,
		SKE_KEY* K)
{
	/* TODO: write this.  Make sure you check the mac before decypting!
	 * Oh, and also, return -1 if the ciphertext is found invalid.
	 * Otherwise, return the number of bytes written.  See aes-example.c
	 * for how to do basic decryption. */
	return 0;
}
size_t ske_decrypt_file(const char* fnout, const char* fnin,
		SKE_KEY* K, size_t offset_in)
{
	/* TODO: write this. */
	return 0;
}
