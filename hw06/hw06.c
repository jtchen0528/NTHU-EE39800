// EE3980 HW05 Trading Stock, II
// 106061146, Jhao-Ting, Chen
// 2020/04/08

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

typedef struct sSTKprice {					// Storing stock shares
    int year, month, day;
    double price, change;
} STKprice;

typedef struct sMaxArray {					// return found value
    int low, high;
    double change;
}MaxArray;

int R = 1000;								// # of test cycles
int N;										// # of stock shares
STKprice *data;								// Stock list

void readInput(void);           			// read all inputs
void printInput(void);                     	// print Input
double GetTime(void);           			// get local time in seconds
MaxArray MaxSubArrayBF(STKprice *A, int N);				// 3 test algorithms
MaxArray MaxSubArray(STKprice *A, int begin, int end);
MaxArray MaxSubArrayXB(STKprice *A, int begin, int mid, int end);
MaxArray MaxSubArrayBF2(STKprice *A, int N);				// 3 test algorithms
MaxArray MaxSubArrayN(STKprice *A, int N);

int main(void)
{
	int i;             						// loop index
	double t;								// record CPU time
	MaxArray ans;							// returned value
	
	readInput();                			// read input graph
//    printInput();
	printf("N = %d\n", N);

	t = GetTime();							// initialize timer			
	ans = MaxSubArrayBF(data, N);
   
	printf("Brute-force approach: time %e s\n", (GetTime() - t));	// result
	printf("  Buy: %d/%d/%d at %g\n", data[ans.low].year, data[ans.low].month,
			data[ans.low].day, data[ans.low].price);
	printf("  Sell: %d/%d/%d at %g\n", data[ans.high].year, 
			data[ans.high].month, data[ans.high].day, data[ans.high].price);
	printf("  Earning: %g per share.\n", ans.change);

	t = GetTime();              			// initialize time counter
	for (i = 0; i < R; i++) {               // Connect1 testing
        ans = MaxSubArray(data, 0, N-1);
    }

	printf("Divide and Conquer: time %e s\n", (GetTime() - t) / R);
	printf("  Buy: %d/%d/%d at %g\n", data[ans.low].year, data[ans.low].month,
			data[ans.low].day, data[ans.low].price);
	printf("  Sell: %d/%d/%d at %g\n", data[ans.high].year, 
			data[ans.high].month, data[ans.high].day, data[ans.high].price);
	printf("  Earning: %g per share.\n", ans.change);

	t = GetTime();							// initialize timer			
	ans = MaxSubArrayBF2(data, N);
   
	printf("Brute-force N^2 approach: time %e s\n", (GetTime() - t));	// result
	printf("  Buy: %d/%d/%d at %g\n", data[ans.low].year, data[ans.low].month,
			data[ans.low].day, data[ans.low].price);
	printf("  Sell: %d/%d/%d at %g\n", data[ans.high].year, 
			data[ans.high].month, data[ans.high].day, data[ans.high].price);
	printf("  Earning: %g per share.\n", ans.change);
	
    t = GetTime();              			// initialize time counter
	for (i = 0; i < R; i++) {               // Connect1 testing
        ans = MaxSubArrayN(data, N);
    }

	printf("N: time %e s\n", (GetTime() - t) / R);
	printf("  Buy: %d/%d/%d at %g\n", data[ans.low].year, data[ans.low].month,
			data[ans.low].day, data[ans.low].price);
	printf("  Sell: %d/%d/%d at %g\n", data[ans.high].year, 
			data[ans.high].month, data[ans.high].day, data[ans.high].price);
	printf("  Earning: %g per share.\n", ans.change);


	
	return 0;
}

void readInput(void)            			    // read all inputs
{
	int i;								// for looping and dynamic store   

	scanf("%d\n", &N);					// read # of Vertices and Edges

	data = (STKprice *)calloc(N, sizeof(STKprice));


	for (i = 0; i < N; i++) {                   // Store shares in data
		scanf("%d %d %d %lf\n", &data[i].year, &data[i].month, 
								&data[i].day, &data[i].price);	// read shares
    }

	data[0].change = 0.0;						// store each share's change
	for (i = 1; i < N; i++) {
		data[i].change = data[i].price - data[i - 1].price;
    }
}

void printInput(void)                          // print stock list
{
    int i;
	for (i = 0; i < N; i++) {
		printf("%d %d %d %lf %lf\n", data[i].year, data[i].month, data[i].day,
									data[i].price, data[i].change);
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
	double max = 0;					// initialize
	double sum;
	int low = 0;
	int high = N - 1;
	int i, j, k;

    MaxArray ans;

	for (j = 0; j < N; j++) {		// Try all possible ranges: A[j : k].
		for (k = j; k < N; k++) {
			sum = 0;
			for (i = j + 1; i <= k; i++) {	// Summation for A[j : k]
				sum = sum + A[i].change;
			}
			if (sum > max) {			// Record the maximum value and range
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
	int mid;					// terminate condition
	MaxArray ans;
	if (begin == end) {
		ans.low = begin;
		ans.high = end;
		ans.change = 0;
		return ans;
	}
	mid = (begin + end) / 2;
	MaxArray lsum, rsum, xsum; 
	lsum = MaxSubArray(A, begin, mid);		// left region
	rsum = MaxSubArray(A, mid + 1, end);	// right region
	xsum = MaxSubArrayXB(A, begin, mid, end);	// cross obundary
	if ((lsum.change >= rsum.change) && (lsum.change >= xsum.change)) {
		ans.low = lsum.low;						// lsum is the largest
		ans.high = lsum.high;
		ans.change = lsum.change;
		return ans;
	} else if ((rsum.change >= lsum.change) && (rsum.change >= xsum.change)) {
		ans.low = rsum.low;						// rsum is the largest
		ans.high = rsum.high;
		ans.change = rsum.change;
		return ans;
    }
	ans.low = xsum.low;						// cross-boundary is the largest
	ans.high = xsum.high;
	ans.change = xsum.change;
	return ans;
}

MaxArray MaxSubArrayXB(STKprice *A, int begin, int mid, int end)
{
	double lsum, sum, rsum;			// initialize
	int i;
	int low, high;
	MaxArray ans;

	low = mid;    					// initialize for lower half
	lsum = 0.0;
	sum = 0.0;
    
	for (i = mid; i > begin; i--) {		// find low to maximize A[low : mid]
		sum = sum + A[i].change;		// continue to add
		if (sum > lsum) {				// record if larger
			lsum = sum;
			low = i;
		}
	}

	rsum = 0.0;							// Initiallize for higher half
	high = mid + 1;
	sum = 0.0;

	for (i = mid + 1; i <= end; i++) {	// find end 
		sum = sum + A[i].change;		// continue to add
		if (sum > rsum) {				// record if larger
			rsum = sum;
			high = i;
		}
	}

	ans.low = low - 1;
	ans.high = high;
	ans.change = lsum + rsum;			// overall sum
	return ans;
}

MaxArray MaxSubArrayBF2(STKprice *A, int N)
{
	double max = 0;					// initialize
	double sum;
	int low = 0;
	int high = N - 1;
	int j, k;

    MaxArray ans;

	for (j = 0; j < N; j++) {		// Try all possible ranges: A[j : k].
		for (k = j; k < N; k++) {
			sum = A[k].price - A[j].price;
			if (sum > max) {			// Record the maximum value and range
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

MaxArray MaxSubArrayN(STKprice *A, int N)
{
    double minprice = 1000000.0;
    double maxprofit = 0.0;
    MaxArray ans;
    int i, low, high;
    for (i = 0; i < N; i++){
        if (A[i].price < minprice) {
            minprice = A[i].price;
            low = i;
        } else if (A[i].price - minprice > maxprofit) {
            maxprofit = A[i].price - minprice;
            high = i;
        }
    } 
    ans.low = low;
    ans.high = high;
    ans.change = maxprofit;
    return ans;
}
