#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "rsa.h"

/*
 * Prints the usage message
 */
void usage(void){
	printf(
	    "\n"
	    "Usage:\n"
	    "    rsa -g \n" 
	    "    rsa -i in_file -o out_file -k key_file [-d | -e]\n" 
	    "    rsa -h\n"
	    "\n"
	    "Options:\n"
	    " -i    path    Path to input file\n"
	    " -o    path    Path to output file\n"
	    " -k    path    Path to key file\n"
	    " -d            Decrypt input and store results to output\n"
	    " -e            Encrypt input and store results to output\n"
	    " -g            Generates a pair of keys and saves them to 2 files\n"
 		" -h            Shows this help message\n"
	);
	exit(EXIT_FAILURE);
}


/*
 * Checks the validity of the arguments
 *
 * arg0: path to input file
 * arg1: path to output file
 * arg2: path to key file
 * arg3: operation mode
 */
void check_args(char *input_file, char *output_file, char *key_file, int op_mode){
	if ((!input_file) && (op_mode != 2)) {
		printf("Error: No input file!\n");
		usage();
	}

	if ((!output_file) && (op_mode != 2)) {
		printf("Error: No output file!\n");
		usage();
	}

	if ((!key_file) && (op_mode != 2)) {
		printf("Error: No user key!\n");
		usage();
	}

	if (op_mode == -1) {
		printf("Error: No mode\n");
		usage();
	}
}


/*
 * Performs RSA key generation and stores the keys into 2 files
 *
 * Encrypts the input file and stores the ciphertext to the output file
 *
 * Decrypts the input file and stores the plaintext to the output file
 */
int main(int argc, char **argv){
	int opt;			/* used for command line arguments */
	int op_mode;			/* operation mode                  */
	char *input_file;		/* path to the input file          */
	char *output_file;		/* path to the output file         */
	char *key_file;			/* path to the key file            */

	//init arguments
	input_file = NULL;
	output_file = NULL;
	key_file = NULL;
	op_mode = -1;

	//get arguments
	while ((opt = getopt(argc, argv, "i:o:k:degh")) != -1) {
		switch (opt) {
		case 'i':
			input_file = strdup(optarg);
			break;
		case 'o':
			output_file = strdup(optarg);
			break;
		case 'k':
			key_file = strdup(optarg);
			break;
		case 'd':
			/* if op_mode == 0 the tool decrypts */
			op_mode = 0;
			break;
		case 'e':
			/* if op_mode == 1 the tool encrypts */
			op_mode = 1;
			break;
		case 'g':
			/* if op_mode == 2 the tool performs keygen */
			op_mode = 2;
			break;
		case 'h':
		default:
			usage();
		}
	}


	//check arguments
	check_args(input_file, output_file, key_file, op_mode);


	//execute function
	switch (op_mode) {
	case 0:
		rsa_decrypt(input_file, output_file, key_file);
		break;
	case 1:
		rsa_encrypt(input_file, output_file, key_file);
		break;
	case 2:
		rsa_keygen();
		break;
	default:
		break;
	}

		

	//clean up
	free(input_file);
	free(output_file);
	free(key_file);


	return 0;
}
