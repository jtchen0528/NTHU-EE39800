// EE3980 HW07 Grouping Friends
// 106061146, Jhao-Ting, Chen
// 2020/04/23

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

struct node{
	int data;
	struct node *next;
};
typedef struct node Node;

char **NameList;
Node **Connect;
Node **ConnectT;
Node *steps;

int N, M, time, sub = 0;					// # of stock shares

int *v, *f, *l;

void readInput(void);           			// read all inputs
void printInput(void);                     	// print Input
double GetTime(void);           			// get local time in seconds
void DFS_call(Node **G);			// call function for DFS
void DFS_d(Node **G, int i);				// Depth First search
void SCC(void);								// Strongly Connected Components

int main(void)
{
	int i, j, k;             				// loop index
	double t;								// record CPU time
	Node *n;								// node to go through subgroups

	readInput();                			// read input graph

//	printInput();
	
	printf("N = %d M = %d", N, M);			// print V & E num	
	t = GetTime();							// Get CPU time
	SCC();									// find SCC groups
	t = GetTime() - t;						// Calculate CPU runtime
	
	n = steps;								// Initialize 
	i = 0;
	k = 1;
	printf(" Subgroup = %d CPU time = %g\n", sub, t);	// print result
	
	while(n->next != NULL){					// go through f[i] according to travel order
		printf("  Subgroup %d: ", k);		// each n is a subgroup
		for(j = 0; j < (n->data - n->next->data - 1); j++){
			printf("%s " , NameList[f[i]]);
			i++;
		}
		printf("%s" , NameList[f[i]]);
		i++;
		printf("\n");
		k++;
		n = n->next;
	}
	
	return 0;
}

void readInput(void)            			    // read all inputs
{
	int i, j, len, end;								// for looping and dynamic store   
	char str[15], str2[15];						// storing Chinese names

	scanf("%d %d\n", &N, &M);					// read # of Vertices and Edges

	Connect = (Node **)calloc(N, sizeof(Node *));	// open space for all needed arrays
	ConnectT = (Node **)calloc(N, sizeof(Node *));
	NameList = (char **)calloc(N, sizeof(char *));
	v = (int *)calloc(M, sizeof(int));
	f = (int *)calloc(M, sizeof(int));
	l = (int *)calloc(M, sizeof(int));

	for (i = 0; i < N; i++) {                   // Store Chinese names in NameList
		scanf("%s", str);						// read Chinese names
		len = strlen(str);
		Connect[i] = (Node *)calloc(1, sizeof(Node));			// store adjacent list
		ConnectT[i] = (Node *)calloc(1, sizeof(Node));
		NameList[i] = (char *)calloc(len + 1, sizeof(char));	// store names
		for (j = 0; j < len; j++){				
			NameList[i][j] = str[j];
		}
		Connect[i]->next = NULL;
		Connect[i]->data = i;
		ConnectT[i]->next = NULL;
		ConnectT[i]->data = i;
		NameList[i][j] = '\0';
	}
	
	for (i = 0; i < M; i++) {                   // Store connections
		Node *newnode, *newnode2, *p;			// Initialize connection nodes		
		end = 0;								// iteration terminator

		scanf("%s -> %s", str, str2);			// read connection pairs
		newnode = (Node *)calloc(1, sizeof(Node));		// open spaces
		newnode->next = NULL;
		newnode2 = (Node *)calloc(1, sizeof(Node));
		newnode2->next = NULL;
		for (j = 0; end == 0; j++) {			// search for orders in NameList
			if (strcmp(NameList[j], str2) == 0) {
				newnode->data = j;				// store index
                p = Connect[j];
				newnode2->next = p->next;
                p->next = newnode2;
				end = 1;
			}
		}
		end = 0;
		for (j = 0; end == 0; j++) {
            if (strcmp(NameList[j], str) == 0) {
                newnode2->data = j;
                p = ConnectT[j];
				newnode->next = p->next;
				p->next = newnode;
				end = 1;
            }
        }
		end = 0;
    }
}

void printInput(void)                          // print adjacent list
{
    int i;
    Node *p;

	for (i = 0; i < N; i++) {
        p = Connect[i];
        printf("%d", p->data);
        while (p->next != NULL) {
            p = p->next;
            printf(" -> %d", p->data);
        }
        printf("\n");
    }
}

double GetTime(void)					// demonstration code from 1.1.3
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return tv.tv_sec + 1e-6 * tv.tv_usec;
}

void DFS_call(Node **G)
{
    int i;							// Initialize loop index
    Node *newtime;					// store travel orders
    
	sub = 0;						// initialize subgroup num
	steps = NULL;					// initialize steps
    newtime = (Node *)calloc(1, sizeof(Node));
    newtime->data = 0;
    newtime->next = steps;
    steps = newtime;
    
	for (i = 0; i < N; i++) {
        v[i] = 0;
        f[i] = 0;
    }
    time = 0;
    for (i = 0; i < N; i++) {
        if (v[l[i]] == 0) {			// if z[i] is not visited
            DFS_d(G, l[i]);			// DFS z[i] in G
            Node *newtime;			// store travel times
            newtime = (Node *)calloc(1, sizeof(Node));
            newtime->data = time;
            newtime->next = steps;
			steps = newtime;
			sub++;					// subgroup num + 1
        }
    }
}

void DFS_d(Node **G, int i)
{
    Node *n;						// travel node
    
    v[i] = 1;						// v[i] is traveling
    n = G[i]->next;					// initialize n
    while (n != NULL) {
        if (v[n->data] == 0) {		// if vertice adjacents to i is not visited
            DFS_d(G, n->data);		// DFS that vertice
        }
        n = n->next;				// next vertice adjacents to i
    }

    v[i] = 2;						// i visited
    f[N - 1 - time] = i;			// store travel orders
    time++;							// travel order ++
}

void SCC(void)
{
    int i;
    for (i = 0; i < N; i++) {		// initialize graph's travel order
        l[i] = i;
    }
    DFS_call(Connect);			// do DFS on Connect
    for (i = 0; i < N; i++){		// initialize travel order
        l[i] = f[i];
    }
    DFS_call(ConnectT);			// do DFS on ConnectT
}
