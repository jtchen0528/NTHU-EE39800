// EE3980 HW12 Travelling Salesperson Problem
// 106061146, 陳兆廷
// 2020/06/07

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define INF 10000

typedef struct sNode {				// List to store costs
    int city;
    int cost;
    struct sNode *next;
} Node;

int N;								// initialization
char **city;
int **route, *path, *final_path, *row, *col;
int distance, start;

double GetTime(void);				// get local time in seconds
void readInput(void);				// read route and city
void printInput(void);				// print input
void printMat(int **r);				// print current matrix r
void TravelingSalesman(int src);				// main function 
void LCSearch(int **r, int level, int src, int curr_cost);	// LCSearch
Node *FindAllCosts(int **r, int src, int last_cost);	// find a list with all costs
int Cost(int **r);					// calculate cost

int main(void)
{
    double t;
    readInput();
//	printInput();
    t = GetTime();					// initialize time counter
    TravelingSalesman(1);
    t = (GetTime() - t);
	printf("CPU time %g\n", t);
	return 0;
}

void readInput(void)				// read all inputs
{
	char str[100];					// initialize parameters
	int i, j;
	scanf("%d\n", &N);
	city = (char **)malloc(N * sizeof(char *));	// initialize space
	route = (int **)malloc(N * sizeof(int *));
	path = (int *)malloc(N * sizeof(int));
	final_path = (int *)malloc(N * sizeof(int));
	row = (int *)malloc(N * sizeof(int));
	col = (int *)malloc(N * sizeof(int));
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
			scanf("%d", &route[i][j]);		// if 0 set to INF
			if (route[i][j] == 0) route[i][j] = INF;
		}
	}
}           

void printInput(void)       // print input
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

double GetTime(void)  // get local time in seconds
{   
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return tv.tv_sec + 1e-6 * tv.tv_usec;
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

void TravelingSalesman(int src)				// main function
{
    int i, curr_cost;
	distance = INF;
	start = src;							// starts at city 0
    curr_cost = Cost(route);
    LCSearch(route, 0, src, curr_cost);
    printf("Minimum distance route:\n %s -> ", city[src]);
    for (i = 0; i < N - 1; i++) {
        printf("%s\n %s -> ", city[final_path[i]], city[final_path[i]]);
    }
    printf("%s\nTotal travelling distance: %d\n", city[src], distance);
}

void LCSearch(int **r, int level, int src, int curr_cost)	// LCSearch for TSP
{
    int i, j, min, **new_r;							// initialization
    Node *child, *min_node, *last_min_node, *node, *last_node;

    if (level == N - 1) {						// if it is the last stop
		path[level] = 0;
        if (distance > (curr_cost + r[src][start])) {
            distance = curr_cost + r[src][start];
			for (i = 0; i < N; i++) {
				final_path[i] = path[i];		// store current paths
			}
        }
		return;									// end iteration
    } 
									// initialize route table for this level
    new_r = (int **)malloc(N * sizeof(int *));	
    for (i = 0; i < N; i++) {
        new_r[i] = (int *)malloc(N * sizeof(int));
    }

	child = FindAllCosts(r, src, curr_cost);	// find all costs for this level

    while (child != NULL) {				// end when all the childs are compared
        min = INF;
		node = child;
        while (node != NULL) {				// find minimum cost child
            if (min > node->cost) {
                min = node->cost;
                min_node = node;
                last_min_node = last_node;
            }
			last_node = node;
			node = node->next;
        }
        if (min_node == child)				// delete child
            child = child->next;
        else {
            last_min_node->next = min_node->next;
        }
							// terminate when it already exceed current distance
        if (min_node->cost > distance) return;	
		
        for (i = 0; i < N; i++) {			// set up next route table
            for (j = 0; j < N; j++) {
                if ((j == min_node->city) || (i == src)) {
                    new_r[i][j] = r[i][j] + INF;
				}
                else {
                    new_r[i][j] = r[i][j];
                }   
            }
        }
        new_r[min_node->city][src] += INF;

		Cost(new_r);						// calculate next cost
        path[level] = min_node->city;		// store path
        LCSearch(new_r, level + 1, min_node->city, min_node->cost);	// next level
    }
}

Node *FindAllCosts(int **r, int src, int last_cost)	// find all costs of the level
{
    int i, j, k, *path_cost;				
    Node *tmp, *root;
    
	path_cost = (int *)malloc(N * sizeof(int));		// store this level distances
    for (i = 0; i < N; i++) {
		path_cost[i] = r[src][i];
        r[src][i] += INF;						// init route table
    }

    root = NULL;
    for (i = 0; i < N; i++) {
        if (r[src][i] < 2 * INF) {
            tmp = (Node *)malloc(sizeof(Node));
            tmp->city = i;
            for (j = 0; j < N; j++) {
                r[j][i] += INF;
            }
            r[i][src] += INF;					// calculate costs
            tmp->cost = Cost(r) + last_cost + path_cost[i];
            tmp->next = root;
            root = tmp;
			for (j = 0; j < N; j++) {			// restore route table
				for (k = 0; k < N; k++) {
					if (r[j][k] < INF) r[j][k] += row[j];
				}
			}
			for (j = 0; j < N; j++) {
				for (k = 0; k < N; k++) {
					if (r[k][j] < INF) r[k][j] += col[j];
				}
			}
            r[i][src] -= INF;
            for (j = 0; j < N; j++) {
                r[j][i] -= INF;
            }
        }
    }
    for (i = 0; i < N; i++) {
        r[src][i] -= INF;
    }
    return root;						// return list
}

int Cost(int **r)						// calculate cost
{
    int i, j;
    int min = INF, cost = 0;
    
	for (i = 0; i < N; i++) {			// find min in row
        for (j = 0; j < N; j++) {
            if (r[i][j] < INF && min > r[i][j]) min = r[i][j];
        }
        if (min < INF) {
			cost += min;
			row[i] = min;
		}
		for (j = 0; j < N; j++) {
			if (r[i][j] < INF && min != 0) r[i][j] = r[i][j] - min;
		}
		min = INF;
    }

    for (i = 0; i < N; i++) {			// find min in col
        for (j = 0; j < N; j++) {
            if (r[j][i] < INF && min > r[j][i])
                min = r[j][i];
        }
        if (min < INF) {
			cost += min;
			col[i] = min;
		}
		for (j = 0; j < N; j++) {
			if (r[j][i] < INF && min != 0) r[j][i] = r[j][i] - min;
		}
		min = INF;
    }	
    return cost;
}
