// EE3980 HW08 Selecting Course
// 106061146, Jhao-Ting, Chen
// 2020/05/09

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

typedef struct sCourse{						// Data Structure for courses
	int credict, student;
	char *id, *time, *name;
} Course;

int N;										// # of courses
int *order;									// sorting order
Course *Clist;								// Course List
int table[5][13];						// Weekly Schedule

void readInput(void);           			// read all inputs
void printInput(void);                     	// print Input
void printTable(void);						// print Weekly Schedule
double priority(Course a);					// Course's weight
void insertionSort(int arr[]);				// Insertion Sort
void getCourseTime(char *d, char *n);		// convert M, T, W, R, F to int
int addCourse(Course c);					// check if time slot is taken
void GreedyCourse(void);			// Greedy method to find optimal schedule

int main(void)
{
	readInput();
//	printInput();
	GreedyCourse();
	return 0;
}

void readInput(void)            			    // read all inputs
{
	int i, j, Cred, Stu;						// Initialize
	char ID[15], Time[15], Name[100];

	scanf("%d\n", &N);
	Clist = (Course *)calloc(N, sizeof(Course));	// open space for Course
	order = (int *)calloc(N, sizeof(int));
	for (i = 0; i < N; i++) {
		scanf("%s %d %d %s %[^\n]", ID, &Cred, &Stu, Time, Name);
		Clist[i].credict = Cred;				// store each course element
		Clist[i].student = Stu;
		Clist[i].id = (char *)calloc(strlen(ID) + 1, sizeof(char));
		Clist[i].time = (char *)calloc(strlen(Time) + 1, sizeof(char));
		Clist[i].name = (char *)calloc(strlen(Name) + 1, sizeof(char));
		for(j = 0; j < strlen(ID); j++) {
			Clist[i].id[j] = ID[j];
		}
		Clist[i].id[j] = '\0';
		for(j = 0; j < strlen(Time); j++) {
			Clist[i].time[j] = Time[j];
		}
		Clist[i].time[j] = '\0';
		for(j = 0; j < strlen(Name); j++) {
			Clist[i].name[j] = Name[j];
		}
		Clist[i].name[j] = '\0';
		order[i] = i;
	}
	for (i = 0; i < 5; i++) {
		for (j = 0; j < 5; j++) {
			table[i][j] = 0;
		}
	}
}

void printInput(void)                          // print adjacent list
{
    int i;
	for (i = 0; i < N; i++) {
		printf("%d %s %d %d %s %s\n", i, Clist[i].id, Clist[i].credict, 
			Clist[i].student, Clist[i].time, Clist[i].name);
	}
}

void printTable(void)							// print weekly schedule
{
	int i, j;
	char day[5] = {'M', 'T', 'W', 'R', 'F'};
	printf("Weekly schedule:\n");
	printf("    1 2 3 4 n 5 6 7 8 9 a b c\n");
	for (i = 0; i < 5; i++) {
		printf("  %c", day[i]);
		for (j = 0; j < 13; j++) {
			if (table[i][j]) printf(" V");
			else printf(" .");
		}
		printf("\n");
	}
}

void getCourseTime(char *d, char *n)		// convert course time to integers
{
	char day, num;
	if (*d == 'M') day = '0';
	if (*d == 'T') day = '1';
	if (*d == 'W') day = '2';
	if (*d == 'R') day = '3';
	if (*d == 'F') day = '4';
	if (*n == '1') num = '0';
	if (*n == '2') num = '1';
	if (*n == '3') num = '2';
	if (*n == '4') num = '3';
	if (*n == 'n') num = '4';
	if (*n == '5') num = '5';
	if (*n == '6') num = '6';
	if (*n == '7') num = '7';
	if (*n == '8') num = '8';
	if (*n == '9') num = '9';
	if (*n == 'a') num = ':';
	if (*n == 'b') num = ';';
	if (*n == 'c') num = '<';
	*d = day;
	*n = num;
}

double priority(Course a)	// generate priority for each course for sorting
{
	int i, day, num;
	char d, n;

	for (i = 0; i < strlen(a.time) / 2; i++) {
		d = a.time[i * 2];
		n = a.time[i * 2 + 1];
		getCourseTime(&d, &n);
		day = 5 - d - '0';
		num = 13 - n - '0';
	}
							// course ratio * 5 + credict + time priority
	return (double)a.credict / ((double)strlen(a.time) / 2) * 5 
		+ (double)a.credict + (double) day / 5 + (double) num / 13;
}

void insertionSort(int arr[])	// simple insertion sort with priority
{ 
    int i, j, pos;
	Course a;
    for (i = 1; i < N; i++) {
		a = Clist[arr[i]];
		pos = arr[i];
        j = i - 1;											// apply priority
        while ((j >= 0) && priority(Clist[arr[j]]) < priority(a)) {	
            arr[j + 1] = arr[j];
            j = j - 1; 
        } 
        arr[j + 1] = pos; 
    } 
} 

int addCourse(Course c){		// add course and return if success or fail
	int i, j, day, num, failed = 0;
	int set[5][2];
	char d, n;
	for (i = 0; i < 5; i++) {						// store changed time slot
		for (j = 0; j < 2; j++) {
			set[i][j] = -1;
		}
	}
	for (i = 0; i < strlen(c.time) / 2; i++) {
		d = c.time[i * 2];
		n = c.time[i * 2 + 1];
		getCourseTime(&d, &n);
		day = d - '0';
		num = n - '0';
		if (table[day][num] == 0) {					// if time slot not taken
			table[day][num] = 1;					// take it
			set[i][0] = day;
			set[i][1] = num;
		} else {
			failed = 1;
		}
	}
	if (failed) {									// if there's overlap
		for (i = 0; i < 5; i++) {
			if (set[i][0] != -1) {			// remove slots taken this time
				table[set[i][0]][set[i][1]] = 0;
			}
		}		
		return 1;									// and return failed
	} else {
		return 0;									// or return succeed
	}
}

void GreedyCourse(void)								// fi  nd optimal schedule
{
	int i, cred = 0, Cnum = 0, j;					// Initialize
	int Selected[30];

	insertionSort(order);							// sort course with weight

	for (i = 0; i < N; i++) {						// goes through courses
		if (!addCourse(Clist[order[i]])) {			// if add success
			Selected[Cnum] = i;						// add course
			cred = cred + Clist[order[i]].credict;	// calculate credit
			Cnum++;									// add selected course num
		}
	}
	printf("Total credicts: %d\n", cred);			// print result
	printf("Number of courses selected: %d\n", Cnum);
	for (i = 0; i < Cnum; i++) {
		j = order[Selected[i]];
		printf("%d: %s %d %d %s %s\n", i + 1, Clist[j].id, Clist[j].credict, 
			Clist[j].student, Clist[j].time, Clist[j].name);
	}
	printTable();
}
