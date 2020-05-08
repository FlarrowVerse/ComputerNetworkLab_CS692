#include "studentSorter.h"

int main() {

	int n, i;
	printf("Enter the number of records: ");
	scanf("%d", &n);

	student **records = (student **)calloc(n, sizeof(student *));
	for (i = 0; i < n; i++) {
		records[i] = createNew(i + 1);
	}

	printf("Original Records....\n");
	for (i = 0; i < n; i++) {
		char *buff = toString(records[i]);
		printf("%s\n", buff);
		free(buff);
	}

	sortStudents(records, n, "roll");
	printf("Records sorted by Roll Number....\n");
	for (i = 0; i < n; i++) {
		char *buff = toString(records[i]);
		printf("%s\n", buff);
		free(buff);
	}

	sortStudents(records, n, "name");
	printf("Records sorted by Name....\n");
	for (i = 0; i < n; i++) {
		char *buff = toString(records[i]);
		printf("%s\n", buff);
		free(buff);
	}

	return 0;
}