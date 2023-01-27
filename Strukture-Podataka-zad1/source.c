#include <stdio.h>
#include <stdlib.h>

#define STRING_LEN 64
#define BUFFER_SIZE 1024

typedef struct _student{
	char fname[STRING_LEN], lname[STRING_LEN];
	int pts;
}student;

int studentCount(char *filename) {
	int count = 0;
	FILE* file = NULL;
	char niz[BUFFER_SIZE] = {0};

	file = fopen(filename, "r");
	if (!file) {
		puts("Neuspjesno otvaranje datoteke!");
		return -1;
	}

	while (!feof(file)) {
		fgets(niz, BUFFER_SIZE, file);
		count++;
	}

	fclose(file);

	return count;
}

student* readStudents(int studentcount, char* filename) {
	int i;
	FILE* file = NULL;
	student* list = NULL;
	
	file = fopen(filename, "r");
	if (!file) {
		puts("Neuspjesno otvaranje datoteke!");
		free(file);
		return NULL;
	}

	list = (student *)malloc(studentcount * sizeof(student));
	if (!list) {
		puts("Neuspjesna alokacija memorije!");
		free(list);
		fclose(file);
		return NULL;
	}
	
	

	for(i=0; i<studentcount; i++){
		fscanf(file, " %s\t%s\t%d",
		list[i].fname,
		list[i].lname,
		&list[i].pts);
	}

	fclose(file);

	return list;
}

int findMaxPts(student *list, int count) {
	
	int maxid = 0, i;

	for (i = 1; i < count; i++) {
		if (list[i].pts > list[maxid].pts) {
			maxid = i;
		}
	}
	return list[maxid].pts;
}

void printResults(student *list, int count) {
	
	int i;
	int maxpts = findMaxPts(list, count);

	for (i = 0; i < count; i++) {
		printf("%s %s %d %.2lf\n",
		list[i].fname,
		list[i].lname,
		list[i].pts,
		(double)list[i].pts/maxpts *100);
	}
}

int main() {
	student *list = NULL;
	char* filename = "studenti.txt";
	int count = 0;

	count = studentCount(filename);
	if(count == -1)
		return 1;
	list= readStudents(count, filename);
	if(!list)
		return 1;

	//ne znam error codeove pa vracam 1 u slucaju greske :(

	printResults(list, count);

	return 0;
}