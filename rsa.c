#include "rsa.h"

/*
 * Sieve of Eratosthenes Algorithm
 *
 * arg0: A limit
 * arg1: The size of the generated primes list. Empty argument used as ret val
 *
 * ret:  The prime numbers that are less or equal to the limit
 */
size_t* sieve_of_eratosthenes(size_t limit, size_t *primes_sz){
	char *table=(char *)malloc(limit-1);
    //initialize to 'P'
    size_t i;
    for(i=0; i<limit-1; i++){
        table[i]='P';
    }
    *primes_sz=limit-1;
    //run algorithm
    size_t currNum=2;
    while(currNum<=limit){
        if(table[currNum-2]!='P'){
            currNum++;
            continue;
        }
        size_t numCounter;
        for(numCounter=currNum*2; numCounter<=limit; numCounter+=currNum){
            if(table[numCounter-2]=='P'){
                table[numCounter-2]='C';
                (*primes_sz)--;
            }
        }
        currNum++;
    }
    //create and return prime array
    size_t *primeArr = (size_t*)malloc(sizeof(size_t)*(*primes_sz));
    size_t primeCounter=0;
    for(i=0; i<limit-1; i++){
        if(table[i]=='P'){
            primeArr[primeCounter]=i+2;
            primeCounter++;
        }
    }
    free(table);
    return primeArr;
}


/*
 * Greatest Common Divisor
 *
 * arg0: first number
 * arg1: second number
 *
 * ret: the GCD
 */
size_t gcd(size_t a, size_t b){
    while(a!=b){
        if(a>b){
            a-=b;
        }else{
            b-=a;
        }
    }
    return a;
}


/*
 * Chooses 'e' where 
 *     1 < e < fi(n) AND gcd(e, fi(n)) == 1
 *
 * arg0: table with primes (from sieve_of_eratosthenes() )
 * arg1: size of table with primes
 * arg2: fi(n)
 *
 * ret: 'e'
 */
size_t choose_e(size_t *primes, size_t primeLen, size_t fin){
    //choose highest number in the table that satisfies the conditions
	while(primeLen>0){
        primeLen--;
        if(primes[primeLen]<fin && gcd(primes[primeLen],fin)==1){
            return primes[primeLen];
        }
    }
    //should not reach this point unless the limit in sieve of eratosthenes was too low
    size_t tablesize;
    //limit in sieve is proportional to primeLen so that it is increased recursively until number is found
    size_t* newtable=sieve_of_eratosthenes(1000*primeLen, &tablesize); 
    size_t tmp=choose_e(newtable,tablesize,fin);
    free(newtable);
    return tmp;
}


/*
 * Calculates the modular inverse x so that ax=1(modm)
 * Uses Extended Euclidean Algorithm
 *
 * arg0: a
 * arg1: m
 *
 * ret: x
 */
size_t mod_inverse(size_t a, size_t m){
    size_t originalm=m;
    size_t t=0,newt=1;
    while(a){
        size_t q=m/a;
        size_t tmp;
        tmp=a;
        a=m-q*a;
        m=tmp;
        tmp=newt;
        newt=t-q*newt;
        t=tmp;
    }
    while(((long)t)<0){
        t=t+originalm;
    }
	return t;
}


/*
 * Generates an RSA key pair and saves
 * each key in a different file
 */
void rsa_keygen(void){
	size_t primeLen;
    srand(time(NULL));
    size_t *primes=sieve_of_eratosthenes(RSA_SIEVE_LIMIT,&primeLen);
    size_t p=primes[rand()%primeLen];
    size_t q=primes[rand()%primeLen];
    size_t n=p*q;
    size_t fin=(p-1)*(q-1);
    size_t e=choose_e(primes,primeLen,fin);
    size_t d=mod_inverse(e,fin);
    free(primes);
	FILE *publicfile=fopen("public.key","w");
    FILE *privatefile=fopen("private.key","w");
    if(publicfile==NULL || privatefile==NULL){
        printf("Error while opening files to write");
        return;
    }
    if(fwrite(&n,sizeof(size_t),1,publicfile)!=1 || fwrite(&d,sizeof(size_t),1,publicfile)!=1){
        printf("Error while writing to public.key");
        return;
    }
    if(fwrite(&n,sizeof(size_t),1,privatefile)!=1 || fwrite(&e,sizeof(size_t),1,privatefile)!=1){
        printf("Error while writing to private.key");
        return;
    }
    fclose(publicfile);
    fclose(privatefile);
}


/*
 * Encrypts an input file and dumps the ciphertext into an output file
 *
 * arg0: path to input file
 * arg1: path to output file
 * arg2: path to key file
 */
void rsa_encrypt(char *input_file, char *output_file, char *key_file){
    //read message from input file
    FILE *infile=fopen(input_file,"r");
    if(infile==NULL){
        printf("Error while opening input file");
        return;
    }
    fseek(infile,0L,SEEK_END);
    int mlen=ftell(infile);
    rewind(infile);
    unsigned char *message=(unsigned char *)malloc(mlen);
    if(fread(message,1,mlen,infile)!=mlen){
        printf("Error while reading from input file");
        return;
    }
    fclose(infile);
    //get key from file
    FILE *keyfile=fopen(key_file,"r");
    if(keyfile==NULL){
        printf("Error while opening key file");
        return;
    }
    size_t exp,mod;
    if(fread(&mod,sizeof(size_t),1,keyfile)!=1 || fread(&exp,sizeof(size_t),1,keyfile)!=1){
        printf("Error while reading from key file");
        return;
    }
    fclose(keyfile);
    //execute encryption
    size_t *cipher=(size_t *)malloc(mlen*sizeof(size_t));
    int i;
    for(i=0; i<mlen; i++){
        size_t m=(size_t)message[i];
        size_t m_original=(size_t)message[i];
        int j;
        for(j=1; j<exp; j++){
            m*=m_original;
            m%=mod;
        }
        cipher[i]=m;
    }
    free(message);
    //write to output file
    FILE *outfile=fopen(output_file,"w");
    if(outfile==NULL){
        printf("Error while opening output file");
        return;
    }
    if(fwrite(cipher,sizeof(size_t),mlen,outfile)!=mlen){
        printf("Error while writing to output file");
        return;
    }
    fclose(outfile);
    free(cipher);
}


/*
 * Decrypts an input file and dumps the plaintext into an output file
 *
 * arg0: path to input file
 * arg1: path to output file
 * arg2: path to key file
 */
void rsa_decrypt(char *input_file, char *output_file, char *key_file){
    //read cipher from input file
    FILE *infile=fopen(input_file,"r");
    if(infile==NULL){
        printf("Error while opening input file");
        return;
    }
    fseek(infile,0L,SEEK_END);
    int clen=ftell(infile);
    if(clen % 8){
        printf("Error: bad input file");
        return;
    }
    rewind(infile);
    size_t *cipher=(size_t *)malloc(clen);
    if(fread(cipher,8,clen/8,infile)!=clen/8){
        printf("Error while reading from input file");
        return;
    }
    fclose(infile);
    //get key from file
    FILE *keyfile=fopen(key_file,"r");
    if(keyfile==NULL){
        printf("Error while opening key file");
        return;
    }
    size_t exp,mod;
    if(fread(&mod,sizeof(size_t),1,keyfile)!=1 || fread(&exp,sizeof(size_t),1,keyfile)!=1){
        printf("Error while reading from key file");
        return;
    }
    fclose(keyfile);
    //execute decryption
    unsigned char *message=(unsigned char *)malloc(clen/8);
    int i;
    for(i=0; i<clen/8; i++){
        size_t m=cipher[i];
        int j;
        for(j=1; j<exp; j++){
            m*=cipher[i];
            m%=mod;
        }
        message[i]=(unsigned char)m;
    }
    free(cipher);
    //write to output file
    FILE *outfile=fopen(output_file,"w");
    if(outfile==NULL){
        printf("Error while opening output file");
        return;
    }
    if(fwrite(message,1,clen/8,outfile)!=clen/8){
        printf("Error while writing to output file");
        return;
    }
    fclose(outfile);
    free(message);
}
