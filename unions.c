// Working with unions in c
// Aaron Burns

#include <stdio.h>
#include <inttypes.h>

// A union is like a struct but all of the elements in a union occupy the same memory address
// the two
// 8-bit integers and the one 16-bit integer occupy the same bits of memory
// allows us to treat the same 16 bits of memory as two different types
union endian{
	// takes up 2 bytes// there are two 8-bit integers
	uint8_t ei[2];
	uint16_t si;
};

int main(int argc, char* argv ){
	// declaring an instance of the union created above
	union endian my_union;	
	// assigning a hexidecimal value to the 16-bit int in the union
	my_union.si = 0xff;	
	my_union.ei[0] = 0x2;
	my_union.ei[1] = 0xab;
	// PRIX16 & PRIX8 are types that can be run on any machine to give types the same size of memory
	printf("16-bit: %04" PRIX16 "\n", my_union.si);
	printf(" 8-bit: %02" PRIX8 " %02" PRIX8 "\n", my_union.ei[0], my_union.ei[1]);
			
	return 0;
}
