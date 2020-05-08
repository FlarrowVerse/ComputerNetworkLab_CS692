#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

	long num;
	printf("Decimal: ");
	scanf("%ld", &num);

	char *binary = (char *)calloc(33, sizeof(char));
	sprintf(binary, "%lx", num);

	printf("Binary: %s\n", binary);

	return 0;
}