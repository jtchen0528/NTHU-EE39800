// EE3980 HW01 Quadratic Sorts
// ID, 姓名
// 2020/03/10

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

int N;                          // input size
char **data;                    // input data
char **A;                       // array to be sorted
int R = 500;                    // number of repetitions

void readInput(void);           // read all inputs
void printArray(char **A);      // print the content of array A
void copyArray(char **data, char **A); // copy data to array A
double GetTime(void);           // get local time in seconds
void SelectionSort(char **list, int n); // in-place selection sort
void InsertionSort(char **list, int n); // in-place insertion sort
void BubbleSort(char **list, int n); // in-place bubble sort
void ShakerSort(char **list, int n); // in-place shaker sort

int main(void)
{
    int i;                      // loop index
    double t;                   // for CPU time tracking

    readInput();                // read input data
//    printArray(data);

    t = GetTime();              // initialize time counter
    for (i = 0; i < R; i++) {
        copyArray(data, A);     // initialize array for sorting
        SelectionSort(A, N);    // execute selection sort
        if (i == 0) printArray(A); // print sorted results
    }
    t = (GetTime() - t) / R;    // calculate CPU time per iteration
    printf("Selection sort:\n  N = %d\n  CPU time = %f\n", N, t);        // print out CPU time

    t = GetTime();              // initialize time counter
    for (i = 0; i < R; i++) {
        copyArray(data, A);     // initialize array for sorting
        InsertionSort(A, N);    // execute selection sort
        if (i == 0) printArray(A); // print sorted results
    }
    t = (GetTime() - t) / R;    // calculate CPU time per iteration
    printf("Insertion sort:\n  N = %d\n  CPU time = %f\n", N, t);        // print out CPU time

    t = GetTime();              // initialize time counter
    for (i = 0; i < R; i++) {
        copyArray(data, A);     // initialize array for sorting
        BubbleSort(A, N);    // execute selection sort
        if (i == 0) printArray(A); // print sorted results
    }
    t = (GetTime() - t) / R;    // calculate CPU time per iteration
    printf("Bubble sort:\n  N = %d\n  CPU time = %f\n", N, t);        // print out CPU time

    t = GetTime();              // initialize time counter
    for (i = 0; i < R; i++) {
        copyArray(data, A);     // initialize array for sorting
        ShakerSort(A, N);    // execute selection sort
        if (i == 0) printArray(A); // print sorted results
    }
    t = (GetTime() - t) / R;    // calculate CPU time per iteration
    printf("Shaker sort:\n  N = %d\n  CPU time = %f\n", N, t);        // print out CPU time

    return 0;
}

void readInput(void)            // read all inputs
{
    scanf("%d", &N);
    //printf("%d\n", N);
    int i;
    
    data = (char **)calloc(N, sizeof(char *));
    A = (char **)calloc(N, sizeof(char *));

    for (i = 0; i < N; i++){
        data[i] = (char *)calloc(50, sizeof(char));
        A[i] = (char *)calloc(50, sizeof(char));
        scanf("%s", data[i]);
    }

}           

void printArray(char **A)       // print the content of array A
{
    int i, j;
    for (i = 0; i < N; i++){
        printf("%s\n", A[i]);
    }

}

void copyArray(char **data, char **A)    // copy data to array A
{
    int i;
    
    for (i = 0; i < N; i++){
        A[i] = data[i];
        //printf("%s\n", A[i]);
    }
}

double GetTime(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return tv.tv_sec + 1e-6 * tv.tv_usec;
}

void SelectionSort(char **list, int n) // in-place selection sort
{
    int i, j, k;
    char *tmp = (char *)calloc(50, sizeof(char));
    for (i = 0; i < N; i++){
        j = i;
        for (k = i + 1; k < N; k++){
            if (strcmp(list[k], list[j]) < 0){
                j = k;
            }
        }
        tmp = list[i];
        list[i] = list[j];
        list[j] = tmp;
    }
}

void InsertionSort(char **list, int n) // in-place insertion sort
{
    int i, j;
    char *tmp = (char *)calloc(50, sizeof(char));
    for (j = 1; j < N; j++){
        tmp = list[j];
        i = j - 1;
        while((i >= 0) && (strcmp(tmp, list[i]) < 0)){
            list[i + 1] = list[i];
            i = i - 1;
        }
        A[i + 1] = tmp;
    }
}

void BubbleSort(char **list, int n) // in-place bubble sort
{
    int i, j;
    char *tmp = (char *)calloc(50, sizeof(char));
    for (i = 0; i < N - 1; i++){
        for (j = N - 1; j > i; j--){
            if (strcmp(list[j], list[j-1]) < 0){
                tmp = list[j];
                list[j] = list[j-1];
                list[j-1] = tmp;
            }
        }
    }
}

void ShakerSort(char **list, int n)  // in-place shaker sort
{
    int l = 0, r = N-1, j;
    char *tmp = (char *)calloc(50, sizeof(char));
    while (l <= r) {
        for (j = r; j >= l+1; j--){
            if (strcmp(list[j], list[j-1]) < 0){
                tmp = list[j];
                list[j] = list[j-1];
                list[j-1] = tmp;
            }
        }
        l++;
        for (j = l; j <= r-1; j++){
            if (strcmp(list[j], list[j+1]) > 0){
                tmp = list[j];
                list[j] = list[j+1];
                list[j+1] = tmp;
            }
        }
        r--;
    }
}