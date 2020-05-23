// EE3980 HW10 Coin Set Design
// 106061146, Jhao-Ting, Chen
// 2020/05/21

#include <stdio.h>
#include <stdlib.h>

void getCoin(int n);					// Getting # of coins needed
void printCoin(int n);					// print coins' combination
double CoinSol(int n);					// return average of Coins

int N;
int p[4] = {1, 5, 10, 50};				// initialize Coin values
int *CoinNum, *s;

int main(void) 
{ 
	int i, j, mindd, mindd2;			// initialize
	double minCoin = 100.0, sum;
	N = 100;
	CoinNum = (int *)malloc(N * sizeof(int));	// open spaces
	s = (int *)malloc(N * sizeof(int));

	getCoin(N);									// get coins for 1 ~ 100
//	printCoin(N);								// print result
	printf("For coin set {1, 5, 10, 50} the average is %g\n", CoinSol(N));

	for(i = p[2]; i <= 100; i++) {					// test from 1 ~ 100
		p[3] = i;							// give value to 3rd coin
		getCoin(N);							// get all coins 
		sum = CoinSol(N);
		if (sum < minCoin) {				// get the minimum average
			minCoin = sum;
			mindd = i;
		}
	}
    
	p[3] = mindd;							// print result
	getCoin(N);
//	printCoin(N);
	printf("Coin set {1, 5, 10, %d} has ", mindd); 
	printf("the minimum average of %g\n", CoinSol(N));
	
	minCoin = 100.0;
	p[3] = 50;
	for(i = p[1]; i <= p[3]; i++) {				// test from 1 ~ 100
		p[2] = i;							// give value to 2nd coin
		getCoin(N);							// get all coins
		sum = CoinSol(N);
		if (sum < minCoin) {				// get the minimum average
			minCoin = sum;
			mindd = i;
		}
	}

	p[2] = mindd;							// print result
	getCoin(N);
//	printCoin(N);
	printf("Coin set {1, 5, %d, 50} has ", mindd); 
	printf("the minimum average of %g\n", CoinSol(N));
	
	minCoin = 100.0;
	
	for(i = p[1]; i <= 100; i++) {				// test from 1 ~ 100
		p[2] = i;							// give value to 2nd coin
		for(j = i; j <= 100; j++) {			// test from 1 ~ 100
			p[3] = j;						// give value to 3rd coin
			getCoin(N);						// get all coins
			sum = CoinSol(N);
			if (sum < minCoin) {			// get the minimum average
				minCoin = sum;
				mindd = i;
				mindd2 = j;
			}
		}
	}
	
	p[2] = mindd;							// print result
	p[3] = mindd2;
	getCoin(N);
//	printCoin(N);
	printf("Coin set {1, 5, %d, %d} has ", mindd, mindd2); 
	printf("the minimum average of %g\n", CoinSol(N));
		
	return 0; 
} 

void getCoin(int n)				// get all Coin num and save solution
{
	int i, j, min;							// initialize
	CoinNum[0] = 1;							// one coin for 1 dollar
	s[0] = p[0];
	for (i = 1; i < n; i++) {				// 1 ~ n coin combinations
		min = n;
		for (j = 3; j >=0; j--) {			// test through coin value 1 ~ 4
			if ((i + 1 - p[j]) >= 0) {				// if can use
				if ((1 + CoinNum[i - p[j]]) < min) {	// if lesser coin #
					min = 1 + CoinNum[i - p[j]];		// store result
					s[i] = p[j];			// store the largest 
				}
			}
		}
		CoinNum[i] = min;					// store result
	}
}

void printCoin(int n)
{
	int i, j, sum = 0; 

	for (i = 0; i < N; i++) {
		j = i;
		printf("%d: %d coins, with:", i + 1, CoinNum[i]);
		while (j >= 0) {
			printf(" %d", s[j]);		// print all coins
			sum++;
			j = j - s[j];
		}
		printf("\n");
	}
	printf("sum = %d\n", sum);
}

double CoinSol(int n) {				// count average
	int i;
	double sum = 0.0;
	for (i = 0; i < n; i++) {
		sum = sum + (double)CoinNum[i];
	}
	sum = sum / n;
	return sum;
}
