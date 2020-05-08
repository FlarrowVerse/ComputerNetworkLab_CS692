#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define true 1
#define false 0

long reverseNumber(long);
char* toString(long);


/* Conversion:
	Binary --> Decimal, Hexadecimal, Octal
	Decimal, Hexadecimal, Octal --> Binary
*/
long binaryToDecimal(char *);
char* decimalToBinary(long);

char* binaryToHexadecimal(char *);
char* hexadecimalToBinary(char *);

long binaryToOctal(char *);
char* octalToBinary(long);

/* Conversion:
	Decimal --> Hexadecimal, Octal
	Hexadecimal, Octal --> Decimal	
*/
long decimalToOctal(long);
long octalToDecimal(long);

char* decimalToHexadecimal(long);
long hexadecimalToDecimal(char *);

/* Conversion:
	Octal --> Hexadecimal
	Hexadecimal --> Octal
*/
char* octalToHexadecimal(long);
long hexadecimalToOctal(char *);