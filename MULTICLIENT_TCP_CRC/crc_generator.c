#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1024

char* getCRCData(char *data, char* div) {
	char *transData = (char *)calloc();
}

int main() {

	printf("Enter a dataword: ");
	char *dataword = (char *)calloc(MAX, sizeof(char));
	fgets(dataword, 1024, stdin);

	printf("Enter a divisor: ");
	char *divisor = (char *)calloc(MAX, sizeof(char));
	fgets(divisor, 1024, stdin);

	char *transmission_data = getCRCData(dataword, divisor);
	printf("Data to be transmitted according to CRC: %s\n", transmission_data);

	return 0;
}