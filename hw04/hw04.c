// EE3980 HW04 Network Connectivity Problem
// 106061146, Jhao-Ting, Chen
// 2020/04/03

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

int V, E;                          			// input size
int **data;                    				// input data
int *S;                                     // store disjoint sets
int *R_table;                               // store Roots
int R = 100;                    			// number of repetitions
int NS;                                     // number of Sets

void readGraph(void);           			// read all inputs
void printRtable(void);                     // print Root table (for testing)
double GetTime(void);           			// get local time in seconds
int SetFind(int i);             // Find the set that element i is in
int CollapsingFind(int i);      // Find the root of i, collapsing the elements
void SetUnion(int i, int j);    // Form union of two sets with roots, i and j
void WeightedUnion(int i, int j);   // Form union using the weighting rule
void Connect1(void);                // Find connectivity method 1
void Connect2(void);                // Find connectivity method 2
void Connect3(void);                // Find connectivity method 3

int main(void)
{
	int i;             			// loop index
	double t0, t1, t2, t3;                  // for CPU time tracking
    int NS1, NS2, NS3;                      // for recording NSs

	readGraph();                			// read input graph

	t0 = GetTime();              			// initialize time counter
	for (i = 0; i < R; i++) {               // Connect1 testing
        Connect1();
	}

	NS1 = NS;
	t1 = GetTime();              			// initialize time counter
	for (i = 0; i < R; i++) {               // Connect2 testing
        Connect2();
	}
	
	NS2 = NS;
    t2 = GetTime();              			// initialize time counter
	for (i = 0; i < R; i++) {               // Connect3 testing
        Connect3();
	}

	NS3 = NS;
    t3 = GetTime();              			// initialize time counter
	printf("%e, %e, %e, ", (t1 - t0) / R, (t2 - t1) / R, (t3 - t2) / R);   // print result
	printf("%d, %d, %d\n", NS1, NS2, NS3);   // print result
	
	return 0;
}

void readGraph(void)            			    // read all inputs
{
	int i;							            // for looping and dynamic store

	scanf("%d %d\n", &V, &E);					// read # of Vertices and Edges

	data = (int **)calloc(E, sizeof(int *));	// initialize data size E * 2
    S = (int *)calloc(V, sizeof(int *));        // initialize S size 
    R_table = (int *)calloc(V, sizeof(int *));  // initialize R table size

	for (i = 0; i < E; i++) {                   // Store edges in table
		data[i] = (int *)calloc(2, sizeof(int));
	    scanf("%d %d\n", &data[i][0], &data[i][1]);		// read each edge
	}
}           

void printRtable(void)                          // print the R table result
{
    int i;
    for (i = 0; i < V; i++) {
        printf("%d ", R_table[i]);
    }
    printf("\n");
}

double GetTime(void)					// demonstration code from 1.1.3
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return tv.tv_sec + 1e-6 * tv.tv_usec;
}

int SetFind(int i)
{
    while (S[i] >= 0){
        i = S[i];
    }
    return i;
}

int CollapsingFind(int i)
{
    int r, temp;
    r = i;                  // initiallized r to i
    while (S[r] >= 0) {     // Find the root
        r = S[r];
    }
    while (i != r) {        // Collapse the elements on the path
        temp = S[i];
        S[i] = r;
        i = temp;
    }
    return r;
}

void SetUnion(int i, int j)
{
    S[i] = j;
}

void WeightedUnion(int i, int j)
{
    int temp;           // note that temp < 0
    temp = S[i] + S[j]; 
    if (S[i] > S[j]) {  // i has fewer elements
        S[i] = j;
        S[j] = temp;
    } else {            // j has fewer elements
        S[j] = i;
        S[i] = temp;
    }
}

void Connect1(void)
{
    int i, s0, s1;
    for (i = 0; i < V; i++) {
        S[i] = -1;
    }
    NS = V;
    for (i = 0; i < E; i++) {
        s0 = SetFind(data[i][0] - 1);
        s1 = SetFind(data[i][1] - 1);
        if (s0 != s1) {
            NS--;
            SetUnion(s0, s1);
        }
    }
    for (i = 0; i < V; i++) {
        R_table[i] = SetFind(i) + 1;
    }
}

void Connect2(void)
{
    int i, s0, s1;
    for (i = 0; i < V; i++) {
        S[i] = -1;
    }
    NS = V;
    for (i = 0; i < E; i++) {
        s0 = SetFind(data[i][0] - 1);
        s1 = SetFind(data[i][1] - 1);
        if (s0 != s1) {
            NS--;
            WeightedUnion(s0, s1);
        }
    }
    for (i = 0; i < V; i++) {
        R_table[i] = SetFind(i) + 1;
    }
}

void Connect3(void)
{
    int i, s0, s1;
    for (i = 0; i < V; i++) {
        S[i] = -1;
    }
    NS = V;
    for (i = 0; i < E; i++) {
        s0 = CollapsingFind(data[i][0] - 1);
        s1 = CollapsingFind(data[i][1] - 1);
        if (s0 != s1) {
            NS--;
            WeightedUnion(s0, s1);
        }
    }
    for (i = 0; i < V; i++) {
        R_table[i] = SetFind(i) + 1;
    }
}

