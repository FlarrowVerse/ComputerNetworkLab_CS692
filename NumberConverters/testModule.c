#include "numberConverter.h"

int main() {
	while (true) {
		int base, i;		
		char *bin = (char *)calloc(32, sizeof(char)), *hex = (char *)calloc(8, sizeof(char));
		long dec, oct;
		
		// input of base
		printf("Enter a number base(2,8,10,16): ");
		scanf("%d", &base);
		if (base != 2 && base != 8 && base != 10 && base != 16) {
			printf("Invalid Base. Try Again.\n");
			continue;
		}
		// input of number in given base
		printf("Enter a number in given base: ");
		scanf("%s", bin);
		
		
		switch (base) {
		case 2: 
			dec = binaryToDecimal(bin);
			oct = binaryToOctal(bin);
			hex = binaryToHexadecimal(bin);
		break;
		case 8: oct = atoi(bin);
			bin = octalToBinary(oct);
			dec = binaryToDecimal(bin);
			hex = binaryToHexadecimal(bin);
		break;
		case 10: dec = atoi(bin);
			bin = decimalToBinary(dec);
			oct = binaryToOctal(bin);
			hex = binaryToHexadecimal(bin);
		break;
		case 16: strcpy(hex, bin);
			bin = hexadecimalToBinary(hex);
			dec = binaryToDecimal(bin);
			oct = binaryToOctal(bin);
		break;
		}
		
		printf("Binary: %s\n", bin);
		printf("Octal: %ld\n", oct);
		printf("Decimal: %ld\n", dec);
		printf("Hexadecimal: %s\n", hex);
		
		free(bin);
		free(hex);
		
		char choice;
		printf("Do you want to continue?(Y/N): ");
		scanf("\n%c", &choice);
		if (choice == 'Y' || choice == 'y') {
			continue;
		} else {
			printf("Exiting Program......\n\n");
			return 0;
		}
		
	}

	return 0;
}