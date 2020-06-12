#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXDATA 64

char sub_bit(char bit1, char bit2) {
	return ((bit1 - '0') ^ (bit2 - '0') == 0)? '0': '1';
}

char* multiply(char *str, char bit) {
	char *prod = (char *)calloc(strlen(str), sizeof(char));
	int i;
	for (i = 0; i < strlen(str); i++) {
		prod[i] = (bit == '0')? '0': str[i];
	}
	return prod;
}

char* sub(char *str1, char *str2) {
	char *diff = (char *)calloc(strlen(str1 - 1), sizeof(char));
	int i;
	for (i = 1; i < strlen(str1); i++) {
		diff[i - 1] = sub_bit(str1[i], str2[i]);
	}
	return diff;
}

char* encode_crc(char *bitstream, char *divisor) {
	
	int bitLen = strlen(bitstream), divLen = strlen(divisor), crcLen = divLen - 1;
	int newBitLen = bitLen + crcLen;
	// printf("bitstream is of length: %d; divisor is of length: %d\n", bitLen, divLen);

	char *crc_encoded = (char *)calloc(newBitLen, sizeof(char));
	char *dividend = (char *)calloc(newBitLen, sizeof(char));
	

	strncpy(dividend, bitstream, bitLen);
	memset(dividend + bitLen, '0', crcLen);

	int i = crcLen, j;
	char *rem = (char *)calloc(crcLen, sizeof(char));
	char *curr = (char *)calloc(divLen, sizeof(char));
	strncpy(rem, dividend, crcLen);

	while (i < newBitLen) { // loops through the dividend

		strcpy(curr, rem); // copying the remainder as the subtruend of the next subtraction
		curr[crcLen] = dividend[i]; // bringing down 1 bit from the dividend to attach at the end of the rem
		// printf(" %s\n", curr);		

		char *prod = multiply(divisor, curr[0]); // finding the minuend
		// printf("-%s\n\n", prod);
		strcpy(rem, sub(curr, prod)); // subtracting and putting the result as remainder

		i++; // increment
	}

	strcpy(crc_encoded, bitstream);
	strcat(crc_encoded, rem);

	return crc_encoded;
} 

int main(int argc, char const *argv[]) {
	char *bitstream = (char *)calloc(MAXDATA, sizeof(char));
	char *divisor = (char *)calloc(MAXDATA, sizeof(char));

	while(1) {
		printf("Enter a dataword: ");
		fgets(bitstream, MAXDATA, stdin);
		bitstream[strlen(bitstream)-1] = '\0';

		printf("Enter a generator code: ");
		fgets(divisor, MAXDATA, stdin);
		divisor[strlen(divisor)-1] = '\0';

		printf("CRC(%s, %s) = %s\n\n", bitstream, divisor, encode_crc(bitstream, divisor));

		printf("Do you want to continue?(Y/N): ");
		char ch;
		//getchar(); // \n
		ch = getchar();
		
		getchar();
		if (ch == 'Y' || ch == 'y') {
			continue;
		}
		break;
	}

	return 0;
}