#include "numberConverter.h"

long reverseNumber(long n) {
	long rev = 0;
	while(n > 0) {
		rev = rev * 10 + (n % 10);
		n /= 10;
	}
	return rev;
}

/* Binary <---> Decimal */
long binaryToDecimal(char *bin) {
	long n = 0;
	int i;
	for (i = 0; i < strlen(bin); i++) {
		if (bin[i] == '1') {
			n += (1 << (strlen(bin)-i-1));
		}
	}
	
	return n;
}

char* decimalToBinary(long n) {
	char *num = (char *)calloc(32, sizeof(char));
	int i, j;
	for (i = 0; i < 32; i++) {
		num[i] = '0';
		num[i] = ((n >> (31-i) & 1) == 1)? '1': '0';
	}
	int flag = false;
	char *bin = (char *)calloc(32, sizeof(char));
	for (i = 0, j = 0; i < 32; i++) {
		if (num[i] == '0' && flag == false) {
			continue;
		} else {
			flag = true;
			bin[j++] = num[i];
		}
	}
	return bin;
}

/* Binary <---> Hexadecimal */

char* binaryToHexadecimal(char *bin) {

	char *n = (char *)calloc(8, sizeof(char));
	int i, j, k = 7;
	for (i = 0; i < 8; i++) {
		n[i] = '0';
	}
	
	for (i = strlen(bin)-1; i >= 0; i-=4) {
		char *temp = (char *)calloc(4, sizeof(char));
		for (j = 3; j >= 0; j--) {			
			if(i-j >= 0) temp[3-j] = bin[i-j];
			else temp[3-j] = '0';			
		}
		
		int tempdec = (int)binaryToDecimal(temp);
		
		switch (tempdec) {
		case 0: n[k] = '0'; break;
		case 1: n[k] = '1'; break;
		case 2: n[k] = '2'; break;
		case 3: n[k] = '3'; break;
		case 4: n[k] = '4'; break;
		case 5: n[k] = '5'; break;
		case 6: n[k] = '6'; break;
		case 7:	n[k] = '7'; break;	
		case 8: n[k] = '8'; break;
		case 9: n[k] = '9'; break;
		case 10: n[k] = 'A'; break;
		case 11: n[k] = 'B'; break;
		case 12: n[k] = 'C'; break;
		case 13: n[k] = 'D'; break;
		case 14: n[k] = 'E'; break;
		case 15: n[k] = 'F'; break;		
		}
		free(temp);
		k--;
	}
	int flag = false;
	char *hex = (char *)calloc(8, sizeof(char));
	for (i = 0, j = 0; i < 8; i++) {
		if (n[i] == '0' && flag == false) {
			continue;
		} else {
			flag = true;
			hex[j++] = n[i];
		}
	}
	return hex;
}

char* hexadecimalToBinary(char *n) {
	char *num = (char *)calloc(32, sizeof(char));
	
	int i, j;
	for (i = 0; i < strlen(n); i++) {
		int digit = 0;
		if (n[i] >= 'A') {
			digit = 10 + (n[i] - 'A');
		} else {
			digit = n[i] - '0';
		}
		
		char *temp = decimalToBinary(digit);
		char *digBin = (char *)calloc(4, sizeof(char));
		for (j = 0; j < 4-strlen(temp); j++) {
			digBin[j] = '0';
		}
		strcat(digBin, temp);
		strcat(num, digBin);

		free(temp);
		free(digBin);
	}

	int flag = false;
	char *bin = (char *)calloc(32, sizeof(char));
	for (i = 0, j = 0; i < 32; i++) {
		if (num[i] == '0' && flag == false) {
			continue;
		} else {
			flag = true;
			bin[j++] = num[i];
		}
	}
	return bin;
}

/* Binary <---> Octal */

long binaryToOctal(char *bin) {
	long n = 0;
	int i, j, k = 0;
	
	for (i = strlen(bin)-1; i >= 0; i-=3) {
		char *temp = (char *)calloc(3, sizeof(char));
		for (j = 2; j >= 0; j--) {			
			if(i-j >= 0) temp[2-j] = bin[i-j];
			else temp[2-j] = '0';			
		}
		
		int tempdec = (int)binaryToDecimal(temp);
		n = pow(10, k) * tempdec + n;
		
		free(temp);
		k++;
	}
	return n;
} 


char* octalToBinary(long n) {
	char *bin = (char *)malloc(33 * sizeof(char));
	n = reverseNumber(n);
	int i;
	while (n > 0) {
		char *temp = decimalToBinary(n % 10);
		char *digBin = (char *)calloc(3, sizeof(char));
		for (i = 0; i < 3-strlen(temp); i++) {
			digBin[i] = '0';
		}
		strcat(digBin, temp);
		strcat(bin, digBin);

		n /= 10;
		free(temp);
		free(digBin);
	}
	return bin;
}

/* Decimal <---> Octal */
long decimalToOctal(long n) {
	char *bin = decimalToBinary(n);
	long num = binaryToOctal(bin);
	return num;
}

long octalToDecimal(long n) {
	char *bin = octalToBinary(n);
	long num = binaryToDecimal(bin);
	return num;
}

/* Decimal <---> Hexadecimal */
char* decimalToHexadecimal(long n) {
	char *bin = decimalToBinary(n);
	char *num = binaryToHexadecimal(bin);
	return num;
}
long hexadecimalToDecimal(char *n) {
	char *bin = hexadecimalToBinary(n);
	long num = binaryToDecimal(bin);
	return num;
}

/* Octal <---> Hexadecimal */
char* octalToHexadecimal(long n) {
	char *bin = octalToBinary(n);
	char *num = binaryToHexadecimal(bin);
	return num;
}
long hexadecimalToOctal(char *n) {
	char *bin = hexadecimalToBinary(n);
	long num = binaryToOctal(bin);
	return num;
}



