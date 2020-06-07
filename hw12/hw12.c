// EE3980 HW12 Traveling Salesperson Problem
// 106061146, 陳兆廷
// 2020/06/04

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define INF 10000

typedef struct sNode {
	int **path;
	int **reducedRoute;
	int cost;
	int city;
	int level;
} Node;

int N;                     // input size
int **route;					// input route
char **city; 
int src;

void readInput(void);			// read all inputs
void printInput();		// print the content of array A
double GetTime(void);					// get local time in seconds
int TSP(void);
Node *newNode(int **parent, int **path, int level, int s, int d);
int Cost(int **r);
void printMat(int **r); 
Node *copyNode(Node *ori);

int main(void)
{
    int i;							// loop index
    double t;						// for CPU time tracking

    readInput();                // read input dat
	printInput();
    t = GetTime();              // initialize time counter
	TSP();
    t = GetTime() - t;    // calculate CPU time per iteration
    printf("CPU time = %g\n", t);        // print out CPU time

    return 0;
}

void readInput(void)				// read all inputs
{
	char str[100];					// initialize parameters
	int i, j;
	scanf("%d\n", &N);
	city = (char **)malloc(N * sizeof(char *));
	route = (int **)malloc(N * sizeof(int *));
	for (i = 0; i < N; i++) {
		route[i] = (int *)malloc(N * sizeof(int));
		scanf("%[^\n]\n", str);
		city[i] = (char *)malloc((strlen(str) + 1) * sizeof(char));
		for (j = 0; j < strlen(str); j++) {
			city[i][j] = str[j];
		}
		city[i][j] = '\0';
	}
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			scanf("%d", &route[i][j]);
			if (route[i][j] == 0) route[i][j] = INF;
		}
	}
}           

void printInput()       // print the content of array A
{
    int i, j;
	printf("N = %d\n", N);
    for (i = 0; i < N; i++){
        printf("%s\n", city[i]);
    }
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			printf(" %d", route[i][j]);
		}
		printf("\n");
	}
}

double GetTime(void)				// get CPU time
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return tv.tv_sec + 1e-6 * tv.tv_usec;
}

Node *newNode(int **parent, int **path, int level, int s, int d)
{
	int i, j;
	Node *newnode;
	
	newnode = (Node *)malloc(sizeof(Node));
	
	newnode->path = (int **)malloc(N * sizeof(int *));
	for (i = 0; i < N; i++) {
		newnode->path[i] = (int *)malloc(2 * sizeof(int));
		newnode->path[i][0] = path[i][0];
		newnode->path[i][1] = path[i][1];
//		printf("path %d %d\n", path[i][0], path[i][1]);
	}

	newnode->reducedRoute = (int **)malloc(N * sizeof(int *));
	for (i = 0; i < N; i++) {
		newnode->reducedRoute[i] = (int *)malloc(N * sizeof(int));
		for (j = 0; j < N; j++) {
			newnode->reducedRoute[i][j] = parent[i][j];
		}
	}

	for (i = 0; (level != 0) && (i < N); i++) {
		newnode->reducedRoute[s][i] = INF;
		newnode->reducedRoute[i][d] = INF;
	}
	newnode->reducedRoute[d][0] = INF;

	newnode->level = level;
	newnode->city = d;

	return newnode;
}

int Cost(int **r)
{
	int cost = 0;
	int i, j;
	int *row, *col;

	row = (int *)malloc(N * sizeof(int));
	col = (int *)malloc(N * sizeof(int));

	for (i = 0; i < N; i++) {
		row[i] = INF;
	}

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (r[i][j] < row[i]) row[i] = r[i][j];
		}
	}
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if ((r[i][j] !=  INF) && (row[i] != INF)) r[i][j] -= row[i];
		}
	}

	for (i = 0; i < N; i++) {
		col[i] = INF;
	}

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (r[i][j] < col[j]) col[j] = r[i][j];
		}
	}

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if ((r[i][j] !=  INF) && (col[j] != INF)) r[i][j] -= col[j];
		}
	}
	for (i = 0; i < N; i++) {
		if (row[i] != INF) cost += row[i];
		if (col[i] != INF) cost += col[i];
	}
	
	

	return cost;
}

void printMat(int **r) 
{
	int i, j;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			printf(" %d", r[i][j]);
		}
		printf("\n");
	}
}

Node *copyNode(Node *ori)
{
	int i, j;
	Node *n;
	n = (Node *)malloc(sizeof(Node));
	n->reducedRoute = (int **)malloc(N * sizeof(int *));
	n->path = (int **)malloc(N * sizeof(int *));
	n->level = ori->level;
	n->city = ori->city;
	n->cost = ori->cost;
	for (i = 0; i < N; i++) {
		n->reducedRoute[i] = (int *)malloc(N * sizeof(int));
		for (j = 0; j < N; j++) {
			n->reducedRoute[i][j] = ori->reducedRoute[i][j];
		}
	}
	for (i = 0; i < N; i++) {
		n->path[i] = (int *)malloc(2 * sizeof(int));
		n->path[i][0] = ori->path[i][0];
		n->path[i][1] = ori->path[i][1];
	}
	return n;
}

int TSP(void)
{
	int i, j, min = INF, d, pqnum = 0;
	int **path;
	Node **pq;
	Node *current;

	path = (int **)malloc(N * sizeof(int *));
	pq = (Node **)malloc(sizeof(Node *));

	for (i = 0; i < N; i++) {
		path[i] = (int *)malloc(2 * sizeof(int));
		path[i][0] = -1;
		path[i][1] = -1;
	}
	
	Node *root = newNode(route, path, 0, -1, 0);

	root->cost = Cost(root->reducedRoute);
	printf("%d\n", root->cost);
	
	current = root;
	while(current->level != N)
//	for (j = 0; j < 3; j++)
	{
		int city = current->city;
		if (current->level == N - 1) {
			current->path[current->level][0] = city;
			current->path[current->level][1] = src;
			current->level++;
			return current->cost;
		}
		printMat(current->reducedRoute);
		printf("current cost %d\n", current->cost);
		for (i = 0; i < N; i++) {
			if (current->reducedRoute[city][i] != INF) {
				Node *child = newNode(current->reducedRoute, current->path, current->level + 1, city, i);
				printMat(child->reducedRoute);
				child->cost = current->cost + current->reducedRoute[city][i] + Cost(child->reducedRoute);
				pq = (Node **)realloc(pq, (pqnum + 1) * sizeof(Node *));
				pq[pqnum] = child;
				pqnum++;
			}
		}
		Node *tmp;
		for (i = 0; i < pqnum; i++) {
			if (pq[i]->cost < min) {
				min = pq[i]->cost;
				tmp = pq[i];
				d = i;
			}
		}
		min = INF;
		current = copyNode(tmp);
		for (i = d; i < pqnum - 1; i++) {
			pq[i] = pq[i] + 1;
		}
		pqnum--;
		printf("%d to %d cost %d level %d\n", city, current->city, current->cost, current->level);	
	}

}
