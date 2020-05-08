#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1024

int main() {
	char *bitstream = (char *)calloc(MAX, sizeof(char));
	char *parity_bitstream = addParity(bitstream);
	char *parity_2d_bitstream = add2DParity(bitstream, 8);

	printf("Original bit stream: %s\n", bitstream);
	printf("Parity added bit stream: %s\n", bitstream);
	printf("2D Parity added bit stream: %s\n", bitstream);


	return 0;
}