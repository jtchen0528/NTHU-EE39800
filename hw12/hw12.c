// EE3980 HW12 Travelling Salesperson Problem
// 106061146, 陳兆廷
// 2020/06/07

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define INF 10000

typedef struct sNode {
    int city;
    int cost;
    struct sNode *next;
} Node;

int N;
char **city;
int **route;
int *path;
int *row;
int *col;
int distance = INF;

double GetTime(void); // get local time in seconds
void readInput(void); // read texta and textb
void printInput();       // print the content of array A
void initLCS(void);
void LCSearch(int **r, int level, int src, int curr_cost);
int Cost(int **r);
void unCost(int **r);
Node *FindAllCosts(int **r, int src, int last_cost);
void printMat(int **r);

int main(void)
{
    double t;

    readInput();
	//printInput();
    t = GetTime(); // initialize time counter
    initLCS();
    t = (GetTime() - t);
    printf("CPU time %g\n", t);
	return 0;
}

void readInput(void)				// read all inputs
{
	char str[100];					// initialize parameters
	int i, j;
	scanf("%d\n", &N);
	city = (char **)malloc(N * sizeof(char *));
	route = (int **)malloc(N * sizeof(int *));
	path = (int *)malloc(N * sizeof(int));
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

double GetTime(void)  // get local time in seconds
{   
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return tv.tv_sec + 1e-6 * tv.tv_usec;
}

void LCSearch(int **cost, int level, int src, int curr_cost)
{
    int i, j, min;
    int **routes;
    Node *child, *min_node, *last_min_node, *node, *last_node;

    if (level == N - 1) {
		path[level] = 0;
        if (distance > (curr_cost + cost[src][0])) {
            distance = curr_cost + cost[src][0];
        }
		return;
    } 

    routes = (int **)malloc(N * sizeof(int *));
    for (i = 0; i < N; i++) {
        routes[i] = (int *)malloc(N * sizeof(int));
    }

	child = FindAllCosts(cost, src, curr_cost);
    while (child != NULL) {
        min = INF;
		node = child;
        while (node != NULL) {
            if (min > node->cost) {
                min = node->cost;
                min_node = node;
                last_min_node = last_node;
            }
			last_node = node;
			node = node->next;
        }
        if (min_node == child)
            child = child->next;
        else {
            last_min_node->next = min_node->next;
        }

        if (min_node->cost > distance) {
            return;
		}

        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                if (i == src || j == min_node->city)
                    routes[i][j] = cost[i][j] + INF;
                else
                {
                    routes[i][j] = cost[i][j];
                }   
            }
        }
        if (level != N - 2) {
            routes[min_node->city][0] += INF;
        }
		Cost(routes);	
        path[level] = min_node->city; // record path
        LCSearch(routes, level + 1, min_node->city, min_node->cost);
    }
}

void initLCS(void)
{
    int cr = 0, i, j;

    cr = Cost(route);
    LCSearch(route, 0, 0, cr);
    printf("Minimum distance route:\n");
    for (i = 0, j = 0; i < N; i++) {
        printf(" %s -> %s\n", city[j], city[path[i]]);
        j = path[i];
    }
    printf("Total travelling distance : %d\n", distance);
}

Node *FindAllCosts(int **r, int src, int last_cost)
{
    int i, j;
    Node *tmp, *root;
    
    for (i = 0; i < N; i++) {
        r[src][i] += INF;
    }

    root = NULL;
    for (i = 0; i < N; i++) {
        if (r[src][i] < 2 * INF) {
            tmp = (Node *)malloc(sizeof(Node));
            tmp->city = i;
            for (j = 0; j < N; j++) {
                r[j][i] += INF;
            }
            r[i][0] += INF;
            tmp->cost = Cost(r) + r[src][i] - 2 * INF + last_cost;
            tmp->next = root;
            root = tmp;
            unCost(r);
            r[i][0] -= INF;
            for (j = 0; j < N; j++) {
                r[j][i] -= INF;
            }
        }
    }
    for (i = 0; i < N; i++) {
        r[src][i] -= INF;
    }
    return root;
}

int Cost(int **r)
{
    int i, j;
    int min = INF, cost = 0;
    
	for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (r[i][j] < INF && min > r[i][j]) min = r[i][j];
        }
        if (min < INF) {
			cost += min;
			row[i] = min;
		}
        if (min != 0) {
            for (j = 0; j < N; j++) {
                if (r[i][j] < INF) r[i][j] = r[i][j] - min;
            }
        }
		min = INF;
    }

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (r[j][i] < INF && min > r[j][i])
                min = r[j][i];
        }
        if (min < INF) {
			cost += min;
			col[i] = min;
		}
        if (min != 0) {
            for (j = 0; j < N; j++) {
                if (r[j][i] < INF) r[j][i] = r[j][i] - min;
            }
        }
		min = INF;
    }	
    return cost;
}

void unCost(int **r)
{
    int i, j;

    for (i = 0; i < N; i++) {
        if (row[i] != 0) {
            for (j = 0; j < N; j++) {
                if (r[i][j] < INF) r[i][j] += row[i];
            }
        }
    }

    for (i = 0; i < N; i++) {
        if (col[i] != 0) {
            for (j = 0; j < N; j++) {
                if (r[j][i] < INF) r[j][i] += col[i];
            }
        }
    }
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

