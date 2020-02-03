// Aaron Burns - SHA 256 functions
// Sourced from: https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf
#include<stdio.h>
#include<stdint.h>

uint32_t Ch(uint32_t x, uint32_t y, uint32_t z){
	// Section 4.1.2 of SHA Standard
	return (x & y) ^ (!x & z)
}

int main(int argc, char *argv[]){

	return 0;
}
