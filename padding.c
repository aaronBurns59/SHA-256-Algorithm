// Aaron Burns
// Padding for SHA-256

#include<stdio.h>
// includes formatters for printing 64 bit integers
#include<inttypes.h>

int main(int argc, char* argv[]){
	if (argc != 2){
		printf("Error: Expecting single filename as argument\n");
		return 1;
	}
	
	FILE *infile = fopen(argv[1], "rb");

	if(!infile){
		printf("Error: Could not open file %s", argv[1]);
		return 1;	
	}
	
	fclose(infile);

	return 0;
}
