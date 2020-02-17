// Working with unions in c
// Aaron Burns

#include <stdio.h>
#include <inttypes.h>

union endian{
	// takes up 2 bytes
	uint8_t ei[2];
	uint16_t si;
};

int main(int argc, char* argv ){
	// declaring an instance of the union created above
	union endian my_union;
	// assigning a hexidecimal value to the 16 bit int in the union
	my_union.si = 0xff;	

	printf("16bit: %04" PRIX16 "\n", my_union.si);
	printf(" 8bit: %02" PRIX8 "%02" PRIX8 "\n", my_union.ei[0], my_union.ei[1]);
			
	return 0;
}
