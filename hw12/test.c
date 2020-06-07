// EE3980 HW12 Travelling Salesperson Problem
// 106000147, 沈永聖
// 2020/06/06

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/time.h>

#define inf 999

typedef struct sCost {
    int des;
    int lb;
    struct sCost *next;
} Cost;


char **city_name;
int city_num;
int **cost;
int *path;
int *sol;
int *red_row;
int *red_col;
int fc = inf;

double GetTime(void); // get local time in seconds
void read(void); // read texta and textb
void DynamicString(char **s, int *size, int str_ind, char item);
void LCS_drive(void);
void LCS(int **cost, int k, int s, int cr);
Cost* find_lb(int **cost, int s, int cr);
int keep(int **cost);
void recover(int **cost);
void print_result(void);

int main(void)
{
    double t;

    
    read();
    t = GetTime(); // initialize time counter
    LCS_drive();
    t = (GetTime() - t);
    printf("CPU time %g\n", t);
    print_result();
	return 0;
}


double GetTime(void)  // get local time in seconds
{   
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return tv.tv_sec + 1e-6 * tv.tv_usec;
    
    
}

void read(void) 
{
    char c;
    int s_size, s_ind;
    int i, j;

    scanf("%d", &city_num); // read number of cities
    city_name = malloc(city_num * sizeof(char *));
    cost = malloc(city_num * sizeof(int *));
    path = malloc(city_num * sizeof(int));
    sol = malloc(city_num * sizeof(int));
    red_row = malloc(city_num * sizeof(int));
    red_col = malloc(city_num * sizeof(int));

    getchar();
    for (i = 0; i < city_num; i++) {
        s_size = 0; s_ind = 0;
        while ((c = getchar()) != '\n') {
            DynamicString(&city_name[i], &s_size, s_ind, c); // store char
            s_ind++; // next char
        }
        // DynamicString(&city_name[i], &s_size, s_ind++, '\n');
        DynamicString(&city_name[i], &s_size, s_ind, '\0');
    }
    for (i = 0; i < city_num; i++) {
        cost[i] = malloc(city_num * sizeof(int));
        for (j = 0; j < city_num; j++) {
            scanf("%d \n", &cost[i][j]);
            if (i == j) {
                cost[i][j] = inf;
            }
        }
    }
}

void DynamicString(char **s, int *size, int str_ind, char item)
{
    int i = 0;		// Using for loop
	char *temp;		// Store data of array temporarily

	if ((*size) == 0) {
		*size = 8;	// Init call
		*s = malloc(*size * sizeof(char)); // Allocate memory
	}
	else if (str_ind >= *size) {	// If full, double array
		*size = 2 * (*size);
		temp = malloc(*size * sizeof(char));
		
		for (i = 0; i < str_ind; i++)
			temp[i] = (*s)[i];
		free(*s);			// Free memory
		*s = temp;
	}
	(*s)[str_ind] = item;	// Store data
}

void print_result(void)
{
    int i, j;
    int c = 0;

    printf("Minimum distance route:\n");
    for (i = 0, j = 0; i < city_num; i++) {
        printf(" %s -> %s\n", city_name[j], city_name[sol[i]]);
        j = sol[i];
    }
    printf("Total travelling distance : %d\n", fc);
}

void LCS(int **cost, int k, int s, int cr)
{
    int i, j;
    int min;
	int temp;
    int **temp_cost;
    Cost *cost_list, *p_min, *prev_min, *head, *prev;

    if (k == city_num -1) { // termination condition
		path[k] = 0;
        if (fc > (cr + cost[s][0])) {
            fc = cr + cost[s][0];
            for (i = 0; i < city_num; i++) {
                sol[i] = path[i];
            }
        }
    } else {
        temp_cost = malloc(city_num * sizeof(int *));
        for (i = 0; i < city_num; i++) {
            temp_cost[i] = malloc(city_num * sizeof(int));
        }

        cost_list = find_lb(cost, s, cr);

        while (cost_list != NULL) {
            min = inf;
            // find least cost
            for (head = cost_list; head != NULL; prev = head, head = head->next) {
                if (min > head->lb) {
                    min = head->lb;
                    p_min = head;
                    prev_min = prev;
                }
            }
            if (p_min == cost_list) // cut list
                cost_list = cost_list->next;
            else {
                prev_min->next = p_min->next;
            }

            if (p_min->lb > fc) {
				for (i = 0; i < city_num; i++) {
                    free(temp_cost[i]);
                }
                free(temp_cost);
                return; // need no more test
				
            } else {
                for (i = 0; i < city_num; i++) {
                    for (j = 0; j < city_num; j++) {
                        if (i == s || j == p_min->des)
                            temp_cost[i][j] = cost[i][j] + inf;
                        else
                        {
                            temp_cost[i][j] = cost[i][j];
                        }
                        
                    }
                }
                if (k != city_num - 2) {
                    temp_cost[p_min->des][0] += inf;
                }
				temp = keep(temp_cost);	
                path[k] = p_min->des; // record path
                LCS(temp_cost, k + 1, p_min->des, p_min->lb);
            }
            
        }
        for (i = 0; i < city_num; i++) {
            free(temp_cost[i]);
        }
        free(temp_cost);
    }
}

void LCS_drive(void)
{
    int cr = 0;
    Cost *temp;

    cr = keep(cost);
    LCS(cost, 0, 0, cr);
}


Cost* find_lb(int **cost, int s, int cr)
{
    int i, j;
    int cmp_value = 2 * inf;
    Cost *temp, *result;
    
    for (i = 0; i < city_num; i++) {
        cost[s][i] += inf;
    }

    result = NULL;
    for (i = 0; i < city_num; i++) {
        if (cost[s][i] < cmp_value) {
            temp = malloc(sizeof(Cost));
            temp->des = i;
            for (j = 0; j < city_num; j++) {
                cost[j][i] += inf;
            }
            cost[i][0] += inf;
            temp->lb = keep(cost) + cost[s][i] - 2 * inf + cr;
            temp->next = result;
            result = temp;
            recover(cost);
            cost[i][0] -= inf;
            for (j = 0; j < city_num; j++) {
                cost[j][i] -= inf;
            }
        }
    }
    for (i = 0; i < city_num; i++) {
        cost[s][i] -= inf;
    }
    return result;
}



int keep(int **cost)
{
    int i, j;
    int min, cr = 0;
    // check row
    for (i = 0; i < city_num; i++) {
        min = inf;
        for (j = 0; j < city_num; j++) {
            if (cost[i][j] < inf && min > cost[i][j])
                min = cost[i][j];
        }
        if (min >= inf)
            min = 0;
        cr += min;
        red_row[i] = min;
        if (min != 0) {
            for (j = 0; j < city_num; j++) {
                if (cost[i][j] < inf) {
                    cost[i][j] = cost[i][j] - min;
                }
            }
        }
    }

    // check column
    for (i = 0; i < city_num; i++) {
        min = inf;
        for (j = 0; j < city_num; j++) {
            if (cost[j][i] < inf && min > cost[j][i])
                min = cost[j][i];
        }
        if (min >= inf)
            min = 0;
        cr += min;
        red_col[i] = min;
        if (min != 0) {
            for (j = 0; j < city_num; j++) {
                if (cost[j][i] < inf) {
                    cost[j][i] = cost[j][i] - min;
                }
            }
        }
    }
    return cr;
}

void recover(int **cost)
{
    int i, j;

    for (i = 0; i < city_num; i++) {
        if (red_row[i] != 0) {
            for (j = 0; j < city_num; j++) {
                if (cost[i][j] < inf)
                    cost[i][j] += red_row[i];
            }
        }
    }

    for (i = 0; i < city_num; i++) {
        if (red_col[i] != 0) {
            for (j = 0; j < city_num; j++) {
                if (cost[j][i] < inf)
                    cost[j][i] += red_col[i];
            }
        }
    }
}

