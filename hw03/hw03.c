// EE3980 HW03 Heap Sorts
// 106061146, Jhao-Ting, Chen
// 2020/03/27

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

int N;                          				// input size
char **data;                    				// input data
char **A;                       				// array to be sorted
int R = 500;                    				// number of repetitions

void readInput(void);           				// read all inputs
void printArray(char **A);      				// print the content of A
void copyArray(char **data, char **A); 			// copy data to array A
double GetTime(void);           				// get local time in seconds
void SelectionSort(char **list, int n); 		// in-place selection sort
void InvSelectionSort(char **list, int n); 		// in-place selection sort
void InsertionSort(char **list, int n); 		// in-place insertion sort
void BubbleSort(char **list, int n); 			// in-place bubble sort
void ShakerSort(char **list, int n); 			// in-place shaker sort
void Heapify(char **list, int i, int n);
void HeapSort(char **list, int n);

int main(void)
{
	int i;                      			// loop index
	double t;                   			// for CPU time tracking

	readInput();                			// read input data

//	InvSelectionSort(data, N);    			// execute inversed selection sort
//	printArray(data);
	t = GetTime();              			// initialize time counter
	for (i = 0; i < R; i++) {
		copyArray(data, A);     			// initialize array for sorting
//		SelectionSort(A, N);    			// execute selection sort
//		InsertionSort(A, N);    			// execute insertion sort
//		BubbleSort(A, N);    				// execute bubble sort
//		ShakerSort(A, N);    				// execute shaker sort
		HeapSort(A, N);    					// execute Heap sort
		if (i == 0) printArray(A); 			// print sorted results
	}
	t = (GetTime() - t) / R;    			// calculate CPU time / iteration
//	printf("Selection sort:\n  N = %d\n  CPU time = %e\n", N, t);	// result
//	printf("Insertion sort:\n  N = %d\n  CPU time = %e\n", N, t);	// result
//	printf("Bubble sort:\n  N = %d\n  CPU time = %e\n", N, t); 	// result
//	printf("Shaker sort:\n  N = %d\n  CPU time = %e\n", N, t); 	// result
	printf("Heap sort:\n  N = %d\n  CPU time = %e\n", N, t); 	// result

	return 0;
}

void readInput(void)                        // read all inputs
{
    int i, j, len;                          // for looping and dynamic store
    char str[50];                           // for dynamic store

    scanf("%d", &N);                        // read N for # of words
    data = (char **)calloc(N, sizeof(char *));  // initialize data size (N)
    A = (char **)calloc(N, sizeof(char *));     // initialize A size (N)

    for (i = 0; i < N; i++) {
        scanf("%s", str);                   // store each word in data
        len = strlen(str);
        data[i] = (char *)calloc(len, sizeof(char));
        A[i] = (char *)calloc(len, sizeof(char));
        for (j = 0; j < len; j++)  {
            data[i][j] = str[j];
            A[i][j] = str[j];
		}
    }
}

void printArray(char **A)       // print the content of array A
{
	int i;								// for looping 
	for (i = 0; i < N; i++) {				
		printf("%s\n", A[i]);			// print each string 
	}

}

void copyArray(char **data, char **A)    // copy data to array A
{
	int i;								// for looping
    
	for (i = 0; i < N; i++) {
		A[i] = data[i];					// copy from data to A
	}
}

double GetTime(void)					// demonstration code from 1.1.3
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return tv.tv_sec + 1e-6 * tv.tv_usec;
}

void SelectionSort(char **list, int n) // in-place selection sort
{
	int i, j, k;						// for looping
	char *tmp = (char *)calloc(50, sizeof(char));	// initialize tmp size
	for (i = 0; i < N; i++) {				// for every A[i]
		j = i;								// Initialize j to i
		for (k = i + 1; k < N; k++) {	// search for smallest in A[i+1 : n]
			if (strcmp(list[k], list[j]) < 0) {	// found, store in j
				j = k;
			}
		}
		tmp = list[i];						// swap A[i] & A[j]
		list[i] = list[j];
		list[j] = tmp;
	}
}

void InsertionSort(char **list, int n) // in-place insertion sort
{
	int i, j;
	char *tmp = (char *)calloc(50, sizeof(char));	// initialize tmp size
	for (j = 1; j < N; j++) {				// Assume A[0: j-1] already sorted
		tmp = list[j];						// Move A[j] to its proper place
		i = j - 1;							// Init i to be j-1
		while ((i >= 0) && (strcmp(tmp, list[i]) < 0)) {//Find i for A[i]<=A[j]
			list[i + 1] = list[i];			// Move A[i] up by one position
			i = i - 1;
		}
		A[i + 1] = tmp;						// Move A[j] to A[i+1]
	}
}

void BubbleSort(char **list, int n) // in-place bubble sort
{
	int i, j;
	char *tmp = (char *)calloc(50, sizeof(char)); 	// Initialize tmp size
	for (i = 0; i < N - 1; i++) {			// Find the smallest item for A[i]
		for (j = N - 1; j > i; j--) {		
			if (strcmp(list[j], list[j - 1]) < 0) {	// swap A[j] & A[j-1]
				tmp = list[j];
				list[j] = list[j - 1];
				list[j - 1] = tmp;
			}
		}
	}
}

void ShakerSort(char **list, int n)  // in-place shaker sort
{
	int l = 0, r = N-1, j;
	char *tmp = (char *)calloc(50, sizeof(char)); 	// Initialize tmp size
	while (l <= r) {
		for (j = r; j >= l+1; j--) {		// Element exchange from r down to l
			if (strcmp(list[j], list[j - 1]) < 0) {	// swap A[j] & A[j-1]
				tmp = list[j];
				list[j] = list[j - 1];
				list[j - 1] = tmp;
			}
		}
		l++;
		for (j = l; j <= r-1; j++) {		// Element exchange from l to r
			if (strcmp(list[j], list[j + 1]) > 0) {	// swap A[j] and A[j+1]
				tmp = list[j];
				list[j] = list[j + 1];
				list[j + 1] = tmp;
			}
		}
		r--;
	}
}

void Heapify(char **list, int i, int n)
{
	int j, done;
	char *tmp = (char *)calloc(50, sizeof(char)); 	// Initialize tmp size
	
	j = 2 * i;				// list[j] is the lchild
	tmp = list[i - 1];
	done = 0;

	while (((j - 1) < n) && (!done)) {		// list[j] is the rchild
		if (((j - 1) < n - 1) && (strcmp(list[j - 1], list[j]) < 0)) {
			j = j + 1;						// list[j - 1] is the larger child
		}
		if (strcmp(tmp, list[j - 1]) > 0) {	// If larger than children, done
			done = 1;
		} else {							// Otherwise, continue
			list[(j / 2) - 1] = list[j - 1];
			j = 2 * j;
		}
	}
	list[(j / 2) - 1] = tmp;				
}

void HeapSort(char **list, int n)
{
	int i;											// for looping
	char *tmp = (char *)calloc(50, sizeof(char)); 	// Initialize tmp size
	
	for (i = (n / 2); i >= 1; i--) {	// Initialize **list to be a max heap
		Heapify(list, i, n);
	}
	for (i = n; i >= 2; i--) {		// Repeat n-1 times
		tmp = list[i - 1];			// Move maximum to the end
		list[i - 1] = list[0]; 
		list[0] = tmp;
		Heapify(list, 1, i - 1);	// Then make list[1: i-1] a max heap
	}
}

void InvSelectionSort(char **list, int n) // in-place selection sort
{
	int i, j, k;						// for looping
	char *tmp = (char *)calloc(50, sizeof(char));	// initialize tmp size
	for (i = 0; i < N; i++) {				// for every A[i]
		j = i;								// Initialize j to i
		for (k = i + 1; k < N; k++) {	// search for biggest in A[i+1 : n]
			if (strcmp(list[k], list[j]) > 0){	// found, store in j
				j = k;
			}
		}
		tmp = list[i];						// swap A[i] & A[j]
		list[i] = list[j];
		list[j] = tmp;
	}
}
