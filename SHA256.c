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

uint32_t Sig2(uint32_t x){
	// Section  4.1.2 of SHA Standard
	return ROTR(x, 4) ^ ROTR(x, 18) ^ SHR(x, 3);
}

uint32_t Sig3(uint32_t x){
	// Section  4.1.2 of SHA Standard
	return ROTR(x, 17) ^ ROTR(x, 19) ^ SHR(x, 10);
}

int main(int argc, char *argv[]){
	uint32_t x = 0x0f0f0f0f;
	uint32_t y = 0xcccccccc;
	uint32_t z = 0x55555555;

	printf("x= %08x\ny= %08x\nz= %08x\n", x, y, z);

	printf("Ch(x,y,z) = %08x\n", Ch(x,y,z));
	printf("Maj(x,y,z) = %08x\n", Maj(x,y,z));
	
	printf("SHA(x,4) = %08x\n", SHR(x, 4));
	printf("ROTR(x,4) = %08x\n", ROTR(x, 4));
	
	printf("Sig0(x) = %08x\n", Sig0(x));
	printf("Sig1(x) = %08x\n", Sig1(x));
	printf("Sig2(x) = %08x\n", Sig2(x));
	printf("Sig3(x) = %08x\n", Sig3(x));

	return 0;
}
