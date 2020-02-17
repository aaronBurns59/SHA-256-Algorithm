// Aaron Burns
// Padding for SHA-256

// used for input and output for files 
#include<stdio.h>
// includes formatters for printing 64 bit integers
#include<inttypes.h>

union block{
	uint64_t sixFour[8];
	uint32_t threeTwo[16];
	uint8_t eight[64];
};

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
	
	// read a byte at a time from the input f
	uint8_t b;
	// keep track of the number of bits read from the input
	uint64_t nobits;
	union block M;
	uint8_t i;

	//  '&' is the address of b not b itself
	// allows us to overwrite the value stored in the address b
	// It reads the file 1 byte at a time
	// nobits += 8 : gets the number of bits read
	for(nobits = 0, i = 0; fread(&M.eight[i], 1, 1, infile) == 1; nobits += 8){
		// PRIx8 is the correct format specifier for hexidecimal
		// b is the byte to be printed
		// %02 means give each byte 2 spaces ((if it's 9 make it 09))
		printf("%02" PRIx8, M.eight[i]);
	}

	// this is the 1 appended to the message block
	printf("%02", PRIx8, 0x80);	// bits : 1000 0000
	
	for(uint64_t i = NoZerosBytes(nobits); i > 0; i--)
		printf("%02" PRIx8, 0x00);
	// prints the number of bits to the screen 
	printf("%016" PRIx64 "\n", nobits);
	
	// need to close the file when finished with it
	fclose(infile);

	return 0;
}
