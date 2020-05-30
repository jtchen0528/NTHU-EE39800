// EE3980 HW11 Transforming Text Files
// 106061146, 陳兆廷
// 2020/05/31

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

int N1, N2;                     // input size
char **data1;					// input data
char **data2;					// input data
int R = 500;                    // number of repetitions
int **M;

void readInput(char **argv);			// read all inputs
void printInput(char **A, int N);		// print the content of array A
double GetTime(void);					// get local time in seconds
void WagnerFisher(int n, int m, char **X, char **Y);	// find transformation
void Trace(int n, int m, char **X, char **Y);	// print transformation

int main(int argc, char **argv)
{
    int i;							// loop index
    double t;						// for CPU time tracking

    readInput(argv);                // read input dat
//	printInput(data1, N1);
//	printInput(data2, N2);

	WagnerFisher(N1, N2, data1, data2);

    t = GetTime();              // initialize time counter
    for (i = 0; i < R; i++) {
		WagnerFisher(N1, N2, data1, data2);	// find transform
		if (i == 1) Trace(N1, N2, data1, data2);
    }
    t = (GetTime() - t) / R;    // calculate CPU time per iteration
    printf("CPU time = %g\n", t);        // print out CPU time

    return 0;
}

void readInput(char **argv)				// read all inputs
{
	char str[100], c;					// initialize parameters
	int i, strnum = 0, end;				
	FILE *file1 = fopen(argv[1],"r");	// read file from arguments
	FILE *file2 = fopen(argv[2],"r");

	fscanf(file1, "%c", &c);				// read first file
	end = fscanf(file1, "%[^\n]", str);	
	while (end != EOF) {
		data1 = (char **)realloc(data1, (strnum + 1) * sizeof(char *));
		data1[strnum] = (char *)malloc((strlen(str) + 3) * sizeof(char));
		data1[strnum][0] = c;
		for (i = 1; i <= strlen(str); i++) {	// store string
			data1[strnum][i] = str[i - 1];
		}
		data1[strnum][i] = '\n';
		data1[strnum][i + 1] = '\0';
		strnum++;
		fscanf(file1, "%c", &c);
		fscanf(file1, "%c", &c);		// get first char (check \n)
		if ((c == 13) || (c == 10)) {
			data1 = (char **)realloc(data1, (strnum + 1) * sizeof(char *));
			data1[strnum] = (char *)malloc(2 * sizeof(char));
			data1[strnum][0] = c;
			data1[strnum][1] = '\0';
			strnum++;
			fscanf(file1, "\n%c", &c);
		}
		end = fscanf(file1, "%[^\n]", str);
	}
	N1 = strnum - 1;
	strnum = 0;	
	fscanf(file2, "%c", &c);				// second file
	end = fscanf(file2, "%[^\n]", str);	
	while (end != EOF) {
		data2 = (char **)realloc(data2, (strnum + 1) * sizeof(char *));
		data2[strnum] = (char *)malloc((strlen(str) + 3) * sizeof(char));
		data2[strnum][0] = c;
		for (i = 1; i <= strlen(str); i++) {	// store string
			data2[strnum][i] = str[i - 1];
		}
		data2[strnum][i] = '\n';
		data2[strnum][i + 1] = '\0';
		strnum++;
		fscanf(file2, "%c", &c);
		fscanf(file2, "%c", &c);		// get first char (check \n)
		if ((c == 13) || (c == 10)) {
			data2 = (char **)realloc(data2, (strnum + 1) * sizeof(char *));
			data2[strnum] = (char *)malloc(2 * sizeof(char));
			data2[strnum][0] = c;
			data2[strnum][1] = '\0';
			strnum++;
			fscanf(file2, "\n%c", &c);
		}
		end = fscanf(file2, "%[^\n]", str);
	}
	N2 = strnum - 1;
}           

void printInput(char **A, int N)       // print the content of array A
{
    int i;
	printf("N = %d\n", N);
    for (i = 0; i < N; i++){
        printf("%d: %s", i + 1, A[i]);
    }

}

double GetTime(void)				// get CPU time
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return tv.tv_sec + 1e-6 * tv.tv_usec;
}

void WagnerFisher(int n, int m, char **X, char **Y)	// find transformation
{
	int i, j, m1, m2, m3;
	M = (int **)malloc((n + 1) * sizeof(int *));
	for (i = 0; i < (n + 1); i++) {
		M[i] = (int *)malloc((m + 1) * sizeof(int));
	}
	M[0][0] = 0;						// initialize

	for (i = 1; i <= n; i++) {
		M[i][0] = M[i - 1][0] + 1;
	}

	for (j = 1; j <= m; j++) {
		M[0][j] = M[0][j - 1] + 1;
	}

	for (i = 1; i <= n; i++) {
		for (j = 1; j <= m; j++) {
			if (strcmp(X[i - 1], Y[j - 1]) == 0) {	
				m1 = M[i - 1][j - 1];	// do nothing if string same
			} else {
				m1 = M[i - 1][j - 1] + 1;	// Change
			}
			m2 = M[i - 1][j] + 1;		// add
			m3 = M[i][j - 1] + 1;		// delete
			if ((m1 <= m2) && (m1 <= m3)) M[i][j] = m1;	// find min
			if ((m2 <= m1) && (m2 <= m3)) M[i][j] = m2;
			if ((m3 <= m1) && (m3 <= m2)) M[i][j] = m3;
		}
	}
}

void Trace(int n, int m, char **X, char **Y)	// print result
{
	int i = n, j = m, k = 0;
	int **T;
	T = (int **)malloc(sizeof(int *));		// initialize table to store steps
	while ((i > 0) && (j > 0)) {
		if ((i > 0) && (j > 0) && (M[i][j] == M[i - 1][j - 1] + 1)) {
			i--;							// it is change
			j--;
			k++;
			T = (int **)realloc(T, k * sizeof(int *));
			T[k - 1] = (int *)malloc(2 * sizeof(int));
			T[k - 1][0] = 2;
			T[k - 1][1] = i;
		} else if ((j == 0) || (M[i][j] == M[i][j - 1] + 1)) {
			j--;							// it is insert
			k++;
			T = (int **)realloc(T, k * sizeof(int *));
			T[k - 1] = (int *)malloc(2 * sizeof(int));
			T[k - 1][0] = 1;
			T[k - 1][1] = j;
		} else if ((i == 0) || (M[i][j] == M[i - 1][j] + 1)) {
			i--;							// it is delete
			k++;
			T = (int **)realloc(T, k * sizeof(int *));
			T[k - 1] = (int *)malloc(2 * sizeof(int));
			T[k - 1][0] = 0;
			T[k - 1][1] = i;
		} else {
			i--; 
			j--;
		}
	}

	printf("%d lines with %d changes:\n", n, k);

	for (i = k - 1; i >= 0; i--) {		// print steps
		if(T[i][0] == 0) {
			printf("Delete line %d:\n%s", T[i][1] + 1, X[T[i][1]]);
		} else if(T[i][0] == 1) {
			printf("Insert line %d:\n%s", T[i][1] + 1, Y[T[i][1]]);
		} else {
			printf("Change line %d:\n%s", T[i][1] + 1, X[T[i][1]]);
		}
	}
}
