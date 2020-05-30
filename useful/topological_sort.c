#include <stdio.h>
#include <stdlib.h>

int **matrix, *flag;
int N;

void top_sort(int v);
void top_call(void);

int main(){

	int i,j;
 
	printf("Enter the no of vertices:\n");
	scanf("%d",&N);
	
	flag = (int *)malloc(N * sizeof(int));
	matrix = (int **)malloc(N * sizeof(int *));

	for(i=0;i<N;i++){
		matrix[i] = (int *)malloc(N * sizeof(int));
		for(j=0;j<N;j++)
			scanf("%d",&matrix[i][j]);
	}
 
	top_call();
 
    return 0;
}

void top_sort(int v)
{
	int i;

	flag[v] = 1;
	
	for (i = 0; i < N; i++) {
		if(matrix[v][i] == 1) {
			if(flag[i] == 0) {
				top_sort(i);
			}
		}
	}
	printf(" %d", v + 1);
}

void top_call(void)
{
	int i;
	for (i = 0; i < N; i++) {
		flag[i] = 0;
	}
	for (i = 0; i < N; i++) {
		if(flag[i] == 0){
			top_sort(i);
		}
	}
}
