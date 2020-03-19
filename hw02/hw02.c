// EE3980 HW02 Random Data Searches
// 106061146, Jhao-Ting, Chen
// 2020/03/18

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

int N;                          				// input size
char **data;                    				// input data
char **A;                       				// array to be sorted
int R = 500;                    				// number of repetitions

void readInput(void);           				// read all inputs
double GetTime(void);           				// get local time in seconds
int Search(char *word, char **list, int n); 	// Linear Search
int BDSearch(char *word, char **list, int n); 	// Bidirection Search
int RDSearch(char *word, char **list, int n); 	// Random-direction Search

int main(void)
{
	int i, result = 0, random_num;             			// loop index
	double t;                   			// for CPU time tracking

	readInput();                			// read input data
	printf("n: %d\n", N);					// print N 

	t = GetTime();              			// initialize time counter
	for (i = 0; i < R; i++) {
		random_num = (rand() % N);			// generate a random number
		result = Search(data[random_num], data, N);	// search item accordingly
	}
	t = (GetTime() - t) / R;    			// calculate CPU time / iteration
	printf("Linear search average CPU time: %e\n", t);// print result

	t = GetTime();              			// initialize time counter
	for (i = 0; i < R; i++) {
		random_num = (rand() % N);			// generate a random number
		result = BDSearch(data[random_num], data, N);	// search item
	}
	t = (GetTime() - t) / R;    			// calculate CPU time / iteration
	printf("Bidirection search average CPU time: %e\n", t);// print result

	t = GetTime();              			// initialize time counter
	for (i = 0; i < R; i++) {
		random_num = (rand() % N);			// generate a random number
		result = RDSearch(data[random_num], data, N);	// search item
	}
	t = (GetTime() - t) / R;    			// calculate CPU time / iteration
	printf("Random-direction search average CPU time: %e\n", t);// result

	t = GetTime();              			// initialize time counter
	for (i = 0; i < R; i++) {
		random_num = (rand() % N);			// generate a random number
		result = Search(data[N-1], data, N);	// search for the middle one
	}
	t = (GetTime() - t) / R;    			// calculate CPU time / iteration
	printf("Linear search worse-case CPU time: %e\n", t);// result
	
	t = GetTime();              			// initialize time counter
	for (i = 0; i < R; i++) {
		random_num = (rand() % N);			// generate a random number
		result = BDSearch(data[N/2], data, N);	// search for the middle one
	}
	t = (GetTime() - t) / R;    			// calculate CPU time / iteration
	printf("Bidirection search worse-case CPU time: %e\n", t);// result
	
	t = GetTime();              			// initialize time counter
	for (i = 0; i < R; i++) {
		random_num = (rand() % N);			// generate a random number
		result = RDSearch(data[N/2], data, N); // search for the middle one
	}
	t = (GetTime() - t) / R;    			// calculate CPU time / iteration
	printf("Random search worse-case CPU time: %e\n", t);// result
	
	return 0;
}

void readInput(void)            			// read all inputs
{
	int i, j, len;							// for looping and dynamic store
	char str[50];							// for dynamic store

	scanf("%d", &N);						// read N for # of words

	data = (char **)calloc(N, sizeof(char *));	// initialize data size (N)
	A = (char **)calloc(N, sizeof(char *));		// initialize A size (N)

	for (i = 0; i < N; i++) {
		scanf("%s", str);					// store each word in data
		len = strlen(str);
		data[i] = (char *)calloc(len, sizeof(char));	
		A[i] = (char *)calloc(len, sizeof(char));
		for (j = 0; j < len; j++)  {
			data[i][j] = str[j];
			A[i][j] = str[j];
		}
	}
}           

double GetTime(void)					// demonstration code from 1.1.3
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return tv.tv_sec + 1e-6 * tv.tv_usec;
}

int Search(char *word, char **list, int n) 		// Linear Search
{
	int i;										// for looping
	for (i = 0; i < n; i++) {				// compare all possible entries
		if (list[i] == word) {
			return i;
		}
	}
	return -1;								// unsuccessful search
}

int BDSearch(char *word, char **list, int n) 	// Bidirection Search
{
	int i;										// for looping
	for (i = 0; i <= n/2; i++) {	// compare all entries from both ends
		if (list[i] == word) {
			return i;
		}
		if (list[n - i - 1] == word) {
			return n - i - 1;
		}
	}
	return -1;								// unsuccessful search
}

int RDSearch(char *word, char **list, int n) 	// Random-direction Search
{
	int i, j;
	j = rand() % 2;
	if (j == 1) {
//		word = list[n-1];					// for worst-case scenario
		for (i = 1; i < n; i++) {				// forward search
			if (list[i] == word) {
				return i;
			}
		}
	}else{
//		word = list[0];						// for worst-case scenario
		for (i == n - 1; i >= 0; i--) {			// backward search
			if (list[i] == word) {
				return i;
			}
		}
	}
	return -1;							// unsuccessful search
}
