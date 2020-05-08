#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 50

typedef struct Student {
	char *name[2];
	long roll;
} student;

char* toString(student *);

void sortByRoll(student* [], int);

void sortByName(student* [], int);

void sortStudents(student**, int, char *);

student* createNew(int);