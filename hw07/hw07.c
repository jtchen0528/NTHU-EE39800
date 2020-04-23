// EE3980 HW07 Grouping Friends
// 106061146, Jhao-Ting, Chen
// 2020/04/23

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

struct node{
	char *data;
    int root;
	struct node *next;
};
typedef struct node Node;

Node **Connect;
Node **ConnectT;
Node *steps;

int N, M, time, sub = 0;										// # of stock shares

int *p, *v, *d, *f, *l;

void readInput(void);           			// read all inputs
void printInput(void);                     	// print Input
double GetTime(void);           			// get local time in seconds
void DFS_call(Node **G, int *z);
void DFS_d(Node **G, int i);
void SCC(void);

int main(void)
{
	int i, j, k;             						// loop index
	double t;								// record CPU time
	
	readInput();                			// read input graph
	printf("N = %d M = %d", N, M);
    //printInput();
    t = GetTime();
    SCC();
    t = GetTime() - t;

    Node *n;
    n = steps;
    i = 0;
    k = 1;
    printf(" Subgroup = %d CPU time = %e\n", sub, t);
    while(n->next != NULL){
        printf("  Subgroup %d: ", k);
        for(j = 0; j < (n->root - n->next->root - 1); j++){
            printf("%s " , Connect[f[i]]->data);
            i++;
        }
        printf("%s" , Connect[f[i]]->data);
        i++;
        printf("\n");
        k++;
        n = n->next;
    }
    return 0;
}

void readInput(void)            			    // read all inputs
{
	int i, j, len, len2;								// for looping and dynamic store   
    char str[15], str2[15];

	scanf("%d %d\n", &N, &M);					// read # of Vertices and Edges

    Connect = (Node **)calloc(N, sizeof(Node *));
    ConnectT = (Node **)calloc(N, sizeof(Node *));
	v = (int *)calloc(M, sizeof(int));
	f = (int *)calloc(M, sizeof(int));
	l = (int *)calloc(M, sizeof(int));

	for (i = 0; i < N; i++) {                   // Store shares in data
        scanf("%s", str);	// read shares
        len = strlen(str);
        Connect[i] = (Node *)calloc(1, sizeof(Node));
        Connect[i]->data = (char *)calloc(len + 1, sizeof(char));
        ConnectT[i] = (Node *)calloc(1, sizeof(Node));
        ConnectT[i]->data = (char *)calloc(len + 1, sizeof(char));
        for (j = 0; j < len; j++){
            Connect[i]->data[j] = str[j];
            ConnectT[i]->data[j] = str[j];
        }
        Connect[i]->next = NULL;
        Connect[i]->data[j] = '\0';
        Connect[i]->root = i;
        ConnectT[i]->next = NULL;
        ConnectT[i]->data[j] = '\0';
        ConnectT[i]->root = i;
    }
	
    for (i = 0; i < M; i++) {                   // Store shares in data
        
        Node *newnode, *newnode2, *p;

        scanf("%s -> %s", str, str2);	// read shares
        len = strlen(str);
        len2 = strlen(str2);
        newnode = (Node *)calloc(1, sizeof(Node));
        newnode->data = (char *)calloc(len2 + 1, sizeof(char));
        newnode->next = NULL;
        newnode2 = (Node *)calloc(1, sizeof(Node));
        newnode2->data = (char *)calloc(len + 1, sizeof(char));
        newnode2->next = NULL;
        for (j = 0; j < len; j++) {
            newnode->data[j] = str2[j];
        }
        newnode->data[j] = '\0';
        for (j = 0; j < len2; j++) {
            newnode2->data[j] = str[j];
        }
        newnode2->data[j] = '\0';
        for (j = 0; j < N; j++) {
            if (strcmp(Connect[j]->data, str2) == 0) {
                newnode->root = j;
            }
            if (strcmp(ConnectT[j]->data, str) == 0) {
                newnode2->root = j;
            }
        }
        for (j = 0; j < N; j++) {
            if (strcmp(Connect[j]->data, str) == 0) {
                p = Connect[j];
                while (p->next != NULL) {
                    p = p->next;
                }
                p->next = newnode;
            }
            if (strcmp(ConnectT[j]->data, str2) == 0) {
                p = ConnectT[j];
                while (p->next != NULL) {
                    p = p->next;
                }
                p->next = newnode2;
            }
        }
    }
}

void printInput(void)                          // print stock list
{
    int i;
    Node *p;

	for (i = 0; i < N; i++) {
        p = Connect[i];
        printf("%s%d", p->data, p->root);
        while (p->next != NULL) {
            p = p->next;
            printf(" -> %s%d", p->data, p->root);
        }
        printf("\n");
    }
	for (i = 0; i < N; i++) {
        p = ConnectT[i];
        printf("%s%d", p->data, p->root);
        while (p->next != NULL) {
            p = p->next;
            printf(" -> %s%d", p->data, p->root);
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

void DFS_call(Node **G, int *z)
{
    int i;
    steps = NULL;
    Node *newtime;
    newtime = (Node *)calloc(1, sizeof(Node));
    newtime->root = 0;
    newtime->next = steps;
    steps = newtime;
    for (i = 0; i < N; i++) {
        v[i] = 0;
        f[i] = 0;
    }
    time = 0;
    for (i = 0; i < N; i++) {
        if (v[z[i]] == 0) {
            DFS_d(G, z[i]);
            Node *newtime;
            newtime = (Node *)calloc(1, sizeof(Node));
            newtime->root = time;
            newtime->next = steps;
            steps = newtime;
            sub++;
        }
    }
}

void DFS_d(Node **G, int i)
{
    Node *n;
    int w;
    
    v[i] = 1;
    n = G[i]->next;
    while (n != NULL) {
        w = n->root;
        if (v[w] == 0) {
            DFS_d(G, w);
        }
        n = n->next;
    }

    v[i] = 2;
    f[N - 1 - time] = i;
    time++;
}

void SCC(void)
{
    int i;
    for (i = 0; i < N; i++) {
        l[i] = i;
    }
    DFS_call(Connect, l);
    for (i = 0; i < N; i++){
        l[i] = f[i];
    }
    DFS_call(ConnectT, l);
    sub--;
}
