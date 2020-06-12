#include "studentSorter.h"

int main() {

	int n, i;
	printf("Enter the number of records: ");
	scanf("%d", &n); getchar();

	student **records = createNewList(n);

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

	printf("Enter a record in string to decode: \n");
	char input[1024];
	fgets(input, 1024, stdin);
	input[strlen(input)-1] = '\0';
	student *record = parseStudent(input);

	printf("FirstName: %s\n", record->name[0]);
	printf("LastName: %s\n", record->name[1]);
	printf("Roll: %ld\n", record->roll);


	return 0;
}