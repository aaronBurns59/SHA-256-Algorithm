// Aaron Burns - SHA 256 functions
// Sourced from: https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf
#include<stdio.h>
#include<stdint.h>

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

int main(int argc, char *argv[]){
	uint32_t x = 0x0f0f0f0f;
	uint32_t y = 0xcccccccc;
	uint32_t z = 0x55555555;

	printf("x= %08x\ny= %08x\nz= %08x\n", x, y, z);

	printf("Ch(x,y,z) = %08x\n", Ch(x,y,z));
	printf("Maj(x,y,z) = %08x\n", Maj(x,y,z));
	
	return 0;
}
