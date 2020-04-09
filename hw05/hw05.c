// EE3980 HW05 Trading Stock
// 106061146, Jhao-Ting, Chen
// 2020/04/08

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

typedef struct sSTKprice {
    int year, month, day;
    double price, change;
} STKprice;

typedef struct sMaxArray {
    int low, high;
    double change;
}MaxArray;

int R = 1000;
int N;
STKprice *data;

void readInput(void);           			// read all inputs
void printInput(void);                     // print Input
double GetTime(void);           			// get local time in seconds
MaxArray MaxSubArrayBF(STKprice *A, int N);
MaxArray MaxSubArray(STKprice *A, int begin, int end);
MaxArray MaxSubArrayXB(STKprice *A, int begin, int mid, int end);


int main(void)
{
	int i;             			// loop index
    double t;
    MaxArray ans;

	readInput();                			// read input graph
//    printInput();
    printf("N = %d\n", N);

    t = GetTime();
    ans = MaxSubArrayBF(data, N);
   
    printf("Brute-force approach: time %e s\n", (GetTime() - t));
    printf("  Buy: %d/%d/%d at %g\n", data[ans.low].year, data[ans.low].month, data[ans.low].day, data[ans.low].price);
    printf("  Sell: %d/%d/%d at %g\n", data[ans.high].year, data[ans.high].month, data[ans.high].day, data[ans.high].price);
    printf("  Earning: %g per share.\n", ans.change);

    t = GetTime();              			// initialize time counter
	for (i = 0; i < R; i++) {               // Connect1 testing
        ans = MaxSubArray(data, 0, N-1);
    }
//    if (ans.low != 0){
//        ans.low--;
//    }
    printf("Divide and Conquer: time %e s\n", (GetTime() - t) / R);
    printf("  Buy: %d/%d/%d at %g\n", data[ans.low].year, data[ans.low].month, data[ans.low].day, data[ans.low].price);
    printf("  Sell: %d/%d/%d at %g\n", data[ans.high].year, data[ans.high].month, data[ans.high].day, data[ans.high].price);
    printf("  Earning: %g per share.\n", ans.change);

	
	return 0;
}

void readInput(void)            			    // read all inputs
{
	int i;							            // for looping and dynamic store   

	scanf("%d\n", &N);					// read # of Vertices and Edges

    data = (STKprice *)calloc(N, sizeof(STKprice));

	for (i = 0; i < N; i++) {                   // Store edges in table
        scanf("%d %d %d %lf\n", &data[i].year, &data[i].month, &data[i].day, &data[i].price);		// read each edge
    }
    data[0].change = 0.0;
    for (i = 1; i < N; i++) {
        data[i].change = data[i].price - data[i - 1].price;
    }
}           

void printInput(void)                          // print the R table result
{
    int i;
    for (i = 0; i < N; i++) {
        printf("%d %d %d %lf %lf\n", data[i].year, data[i].month, data[i].day, data[i].price, data[i].change);		// read each edge
    }
}

double GetTime(void)					// demonstration code from 1.1.3
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return tv.tv_sec + 1e-6 * tv.tv_usec;
}

MaxArray MaxSubArrayBF(STKprice *A, int N)
{
    double max = 0;
    double sum;
    int low = 0;
    int high = N - 1;
    int i, j, k;

    MaxArray ans;

    for (j = 0; j < N; j++) {
        for (k = j; k < N; k++) {
            sum = 0;
            sum = sum - A[j].change;
            for (i = j; i <= k; i++) {
                sum = sum + A[i].change;
            }
            if (sum > max) {
                max = sum;
                low = j;
                high = k;
            }
        }
    }
    ans.low = low;
    ans.high = high;
    ans.change = max;
    return ans;
}

MaxArray MaxSubArray(STKprice *A, int begin, int end)
{
    int mid;
    MaxArray ans;
    if (begin == end) {
        ans.low = begin;
        ans.high = end;
        ans.change = 0;
        return ans;
    }
    mid = (begin + end) / 2;
    MaxArray lsum, rsum, xsum; 
    lsum = MaxSubArray(A, begin, mid);
    rsum = MaxSubArray(A, mid + 1, end);
    xsum = MaxSubArrayXB(A, begin, mid, end);
    if ((lsum.change >= rsum.change) && (lsum.change >= xsum.change)) {
        ans.low = lsum.low;
        ans.high = lsum.high;
        ans.change = lsum.change;
        return ans;
    } else if ((rsum.change >= lsum.change) && (rsum.change >= xsum.change)) {
        ans.low = rsum.low;
        ans.high = rsum.high;
        ans.change = rsum.change;
        return ans;
    }
    ans.low = xsum.low;
    ans.high = xsum.high;
    ans.change = xsum.change;
    return ans;
}

MaxArray MaxSubArrayXB(STKprice *A, int begin, int mid, int end)
{
    double lsum, sum, rsum;
    int i;
    int low, high;
    MaxArray ans;

    low = mid;    
    lsum = 0.0;
    sum = 0.0;
    
    for (i = mid; i > begin; i--) {
        sum = sum + A[i].change;
        if (sum > lsum) {
            lsum = sum;
            low = i;
        }
    }
    
    rsum = 0.0;
    high = mid + 1;
    sum = 0.0;

    for (i = mid + 1; i <= end; i++) {
        sum = sum + A[i].change;
        if (sum > rsum) {
            rsum = sum;
            high = i;
        }
    }

    ans.low = low - 1;
    ans.high = high;
    ans.change = lsum + rsum;
    return ans;

}
