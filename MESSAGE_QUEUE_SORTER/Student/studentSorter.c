#include "studentSorter.h"

char* toString(student *record) {
	char *recStr = (char *)calloc(MAX_LEN * 3, sizeof(char));
	sprintf(recStr, "%ld\t", record->roll);
	strcat(recStr, record->name[0]);
	strcat(recStr, " ");
	strcat(recStr, record->name[1]);
}

void sortByRoll(student *records[], int size) {
	int i, j;

	for (i = 0; i < size - 1; i++) {
		for (j = 0; j < size - i - 1; j++) {
			if (records[j]->roll > records[j + 1]->roll) {				
				student *temp = records[j];
				records[j] = records[j + 1];
				records[j + 1] = temp;
			}
		}
	}	
}

void sortByName(student *records[], int size) {
	int i, j;
	char *firstName = (char *)calloc(MAX_LEN * 2, sizeof(char));
	char *secondName = (char *)calloc(MAX_LEN * 2, sizeof(char));
	for (i = 0; i < size - 1; i++) {
		for (j = 0; j < size - i -1; j++) {
			sprintf(firstName, "%s %s", records[j]->name[0], records[j]->name[1]);
			sprintf(secondName, "%s %s", records[j + 1]->name[0], records[j + 1]->name[1]);
			if (strcmp(firstName, secondName) > 0) {
				student *temp = records[j];
				records[j] = records[j + 1];
				records[j + 1] = temp;
			}
		}
	}
}

void sortStudents(student **records, int size, char *field) {
	if (strcmp(field, "name") == 0) {
		sortByName(records, size);
	}
	else if (strcmp(field, "roll") == 0) {
		sortByRoll(records, size);
	} else {
		printf("ERROR: No such field as %s in student record.\n", field);
		return;
	}
}

student* createNew(int i) {
	student *record = (student *)malloc(sizeof(student));
	record->name[0] = (char *)calloc(MAX_LEN, sizeof(char));
	record->name[1] = (char *)calloc(MAX_LEN, sizeof(char));

	printf("Enter the record for Student %d::\n", i);
	getchar(); // reading out the \n from existing buffer
	printf("First Name: "); fgets(record->name[0], MAX_LEN, stdin);
	record->name[0][strlen(record->name[0]) - 1] = '\0';
	printf("Last Name: "); fgets(record->name[1], MAX_LEN, stdin);
	record->name[1][strlen(record->name[1]) - 1] = '\0';
	printf("Roll No.: "); scanf("%ld", &(record->roll));

	return record;
}