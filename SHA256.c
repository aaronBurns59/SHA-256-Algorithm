// Aaron Burns - SHA 256 functions
// Sourced from: https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf
#include<stdio.h>
#include<stdint.h>
#include<inttypes.h>

// Section 4.2.2
const uint32_t K[] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f
};

// total amount of memory is 512 bits
// the message that is going to be hashed is stored in this union structure
union block{
	uint32_t threeTwo[16];
	uint64_t sixFour[8];
	uint8_t eight[64];
};

// Ch is for choose, you use x to determine what bits from z or y you take from 
uint32_t Ch(uint32_t x, uint32_t y, uint32_t z){
	// Section 4.1.2 of SHA Standard
	// ~ NOT in c
	// ^ XOR in c
	return (x & y) ^ (~x & z);
}
// If there are two or more 1s in any bit position between x, y and z, 
// its going to take a 1 and if there is less thentwo 1s we get a zero in the output 
uint32_t Maj(uint32_t x, uint32_t y, uint32_t z){
	// Section 4.1.2 of SHA Standard
	return (x & y) ^ (x & z) ^ (y & z);
}

uint32_t SHR(uint32_t x, int n){
	// Section 3.2 of SHA Standard
	// bit-shift x to the right the value of n	
	return x>>n;
}

uint32_t ROTR(uint32_t x, int n){
	// Section 3.2 of SHA Standard
	// bit-shift x to the right the value of n	
	return (x>>n) | (x << (32 - n));
}

uint32_t Sig0(uint32_t x){
	// Section  4.1.2 of SHA Standard
	return ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22);
}

uint32_t Sig1(uint32_t x){
	// Section  4.1.2 of SHA Standard
	return ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25);
}

uint32_t sig0(uint32_t x){
	// Section  4.1.2 of SHA Standard
	return ROTR(x, 7) ^ ROTR(x, 18) ^ SHR(x, 3);
}

uint32_t sig1(uint32_t x){
	// Section  4.1.2 of SHA Standard
	return ROTR(x, 17) ^ ROTR(x, 19) ^ SHR(x, 10);
}

uint64_t NoZerosBytes(uint64_t nobits){
	// the number of bits from the file retrieved from the read file block of code ()
	// ULL : Unsigned Long Long integer (Makes sure c saves var as a 64 bit integer)
	// The final output needs to be a multiple of 512 bits
	uint64_t result = 512ULL - (nobits % 512ULL);

	// find out if there is enough room in the last block to do the padding or will a new block be needed
	if (result < 65)// 65 (length of integer and the 1)
		result += 512;
	
	// get the number of 0 bytes needed to pad between the 1 and the 64 bit integer printed at the end
	result -=72;
	return (result/ 8ULL); 
}
	// handy list of constantsi
	// these are the 4 possible scenarios nextblock can come across
enum flag {READ, PAD0, FINISH};
// *status gets the enum value of the current state
// *nobits is for this function to keep track of the bits it has read
int nextblock(union block *M, FILE *infile, uint64_t *nobits, enum flag *status){
	
	// check the status of the padding flag before anything is read in
	if(*status == FINISH)
		return 0;

	if(*status == PAD0)
	{
		// < 56 because you need to leave room for the last 8 bytes of the message 64 - 8 = 56
		for(int i = 0; i < 56; i++)
			M->eight[0];
		// need to stick the 64 bit message into the last 8 bytes of the now padded message
		// index 7 is the last byte of this version of M in its 64 bit form
		// make the last byte of M equal to the message passed in
		// the rest of the bytes in M are now 0's except for the first bit which is a single 1-bit
		M->sixFour[7]  = *nobits;
		// the padding is done, set the status to finish
		*status= FINISH;
		return 1;
	}// if

	size_t noBytesRead = fread(M->eight, 1, 64, infile);
	if(noBytesRead == 64)
		return 1;

	// can fit all the padding in the last block
	if(noBytesRead < 56)
	{
		// this will be the position to put the 1 bit
		M->eight[noBytesRead] = 0x80;
		for(int i = noBytesRead + 1; i < 56; i++)
			M->eight[i] = 0;
		M->sixFour[7] = *nobits;
		*status = FINISH;
		return 1;
	}// if

	
}

void nexthash(union block *M, uint32_t *H){
	uint32_t W[64];
	uint32_t T1,T2,a,b,c,d,e,f,g,h;
	int t;	

	for(t = 0; t <= 15; t++){
		// The union M->threeTwo allows W to be handled as a 32-bit integer
		W[t] = M->threeTwo[t];
	}
	// Section 6.2.2	
	for(t = 16; t <= 63; t++){
		W[t]= sig1(W[t-2]) + W[t-7] + sig0(W[t-15]) + W[t-16];
	}
	// Store the values in H in 8 different variables
	// automatically gets the index from the pointer
	a = H[0]; b = H[1]; c = H[2]; d = H[3];
	e = H[4]; f = H[5]; g = H[6]; h = H[7];

	for(t = 0; t <= 63; t++){
		T1 = h + Sig1(e) + Ch(e,f,g) + K[t] + W[t];
		T2 = Sig0(a) + Maj(a,b,c);
		h=g; g=f; f=e;
		e=d + T1;
		d=c; c=b; b=a;
		a=T1 + T2;
	
		H[0] = a + H[0]; 
		H[1] = b + H[1];
		H[2] = c + H[2];
		H[3] = d + H[3];
		H[4] = e + H[4];
		H[5] = f + H[5];
		H[6] = g + H[6];
		H[7] = h + H[7];
	}
}

int main(int argc, char* argv[]){
	if (argc != 2){
		printf("Error: Expecting single filename as argument\n");
		// if main returns anything other then 0 that indicates an error
		return 1;
	}
	
	// declare for reading file
	FILE *infile = fopen(argv[1], "rb");

	if(!infile){
		printf("Error: Could not open file %s\n", argv[1]);
		return 1;	
	}
	
	// keep track of the number of bits read from the input
	// Current padded message block
	union block M;

	// Section 5.3.3
	uint32_t H[] = {
	0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
	0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19 
	};
	
	uint64_t nobits = 0;
	enum flag status = READ;

	// reads from the file until the end it reads blocks and returns padded blocks
	// reads in an 8-bit value
	while (nextblock(&M, infile, nobits, &status)){
		// calculate the next hash value	
		// uses the 32-bit value
		nexthash(&M, &H);
	}	

	// need to close the file when finished with it
		
	// print the last hash value
	for(int i; i < 8; i++)
		printf("%02" PRIX32, H[i]);
	printf("\n");

	fclose(infile);
	
	return 0;
}
