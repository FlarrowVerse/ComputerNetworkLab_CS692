#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX 1024

int* getIndices(int pos, int max_bits, int *len) {
	// printf("Position recieved: %d\n", pos);
	int i, count = 0;
	for (i = 1; i <= max_bits; i++) {
		// printf("%d ", (pos & i));
		if ((pos & i) == pos && pos != i) {
			count++;
		}
	}
	// printf("\n");
	int *nums = (int *)calloc(count, sizeof(int)), index = 0;
	*len = count;
	for (i = 1; i <= max_bits; i++) {
		if ((pos & i) == pos && pos != i) {
			nums[index] = i;
			// printf("%d ", nums[index]);
			index++;
		}
	}
	// printf(" ---> %d\n", *len);
	return nums;
}

int getRedundantBits(int dataBits) {
	int redBits = 0;
	while((1 << redBits) < (dataBits + redBits + 1)) {
		redBits++;
	}
	return redBits;
}

int getParityBit(int *nums, int len, char *bitStream) {
	int parity = 0, i, total = strlen(bitStream);
	// printf("Length recieved: %d %d\n", len, total);

	for (i = 0; i < len; i++) {
		// printf("%d: %c\t", total - nums[i], bitStream[total - nums[i]]);
		if (bitStream[total - nums[i]] == '1') {
			parity = (parity == 0)? 1: 0;
		}
	}
	// printf(" ------> Parity: %d\n", parity);
	// free(nums);
	return parity;
}

char* hamming_code(char *data) {
	// printf("Data original: %s\n", data);
	int dataBits = strlen(data) - 1, redBits = getRedundantBits(dataBits), i = 0;
	int total = dataBits + redBits;
	char *codedData = (char *)calloc(total, sizeof(char));

	for (i = 0; i < redBits; i++) {
		int currPos = 1 << i;
		codedData[total - currPos] = 'R';
	}
	
	int k = 0;
	for (i = 0; i < total; i++) {
		if (codedData[i] != 'R') {
			codedData[i] = data[k];
			k++;
		}
	}
	// printf("%s\n", codedData);

	for (i = 0; i < redBits; i++) {
		int currPos = 1 << i, len = 0;
		int *arr = getIndices(currPos, total, &len);
		int res = getParityBit(arr, len, codedData);
		// printf("total - currPos: %d\n", total - currPos);
		codedData[total - currPos] = (res == 1)? '1': '0';
	}

	return codedData;
}

int main() {
	char *data = (char *)calloc(MAX, sizeof(char));
	printf("Enter the data stream to be transmitted: ");
	fgets(data, MAX, stdin);

	printf("Data coded by Hamming Code: %s\n", hamming_code(data));
	return 0;
}