#include "numberConverter.h"

char* toString(long n) {
	char *str = (char *)calloc(64, sizeof(char));
	sprintf(str, "%ld", n);
	return str;
}

char* convert(int inp_base, char *inp_num, int out_base) {
	char *out_num = (char *)calloc(64, sizeof(char));
	if (inp_base == 2) {
		switch(out_base) {
		case 8: out_num = toString(binaryToOctal(inp_num)); break;
		case 10: out_num = toString(binaryToDecimal(inp_num)); break;
		case 16: out_num = binaryToHexadecimal(inp_num); break;
		}
	}
	else if (inp_base == 8) {
		long oct = atoi(inp_num);
		switch(out_base) {
		case 2: out_num = octalToBinary(oct); break;
		case 10: out_num = toString(octalToDecimal(oct)); break;
		case 16: out_num = octalToHexadecimal(oct); break;
		}
	}
	else if (inp_base == 10) {
		long dec = atoi(inp_num);
		switch(out_base) {
		case 2: out_num = decimalToBinary(dec); break;
		case 8: out_num = toString(decimalToOctal(dec)); break;
		case 16: out_num = decimalToHexadecimal(dec); break;
		}
	}
	else if (inp_base == 16) {
		switch(out_base) {
		case 2: out_num = hexadecimalToBinary(inp_num); break;
		case 8: out_num = toString(hexadecimalToOctal(inp_num)); break;
		case 10: out_num = toString(hexadecimalToDecimal(inp_num)); break;
		}
	}
	return out_num;
}

int main() {

	printf("----------------------------Number Converter Menu Driven Test----------------------------------\n");
	printf("-----------------------------------------------------------------------------------------------\n");
	while(true) {
		int input_base, output_base;
		char *input_num = (char *)calloc(64, sizeof(char));
		
		printf("Enter the base for input number(2/8/10/16): ");
		scanf("%d", &input_base);
		printf("Enter the number for base %d: ", input_base);
		scanf("%s", input_num);

		printf("Enter the base for output number(2/8/10/16):");
		scanf("%d", &output_base);

		char *output_num = convert(input_base, input_num, output_base);

		printf("(%s)%d ==> (%s)%d\n", input_num, input_base, output_num, output_base);

		free(input_num);
		free(output_num);

		printf("Do you want to continue?(Y/N): ");
		char choice;
		scanf("\n%c", &choice);
		if (choice == 'Y' || choice == 'y') {
			continue;
		}
		break;

	}

	return 0;
}