// EE3980 HW09 Encoding ASCII Texts
// 106061146, Jhao-Ting, Chen
// 2020/05/14

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <locale.h>
#include <wchar.h>

typedef struct sNode { 						// store new node
	char *data; 
	int freq; 
	struct sNode *left, *right; 
} Node;
  
typedef struct sHeap { 						// store new heap
	int size; 
	Node** array; 
} Heap;

void readInput(void);           			// read all inputs
void printInput(void);                     	// print Input
Node* newNode(unsigned char *data, int freq);			// initialize a new node
void Heapify(Heap* Heap, int idx);			// Heapify function
Heap* HeapSort(Heap* Heap, int n);			// Heap Sort function
Node* GetMin(Heap* Heap);					// get min from heap and remove it
Node* BMT(Heap* HeapTree);					// Binary Merge Tree
void printCodes(Node* root, int *code, int top);	// Print Huff code
void Huff(unsigned char **data, int *freq, int size); 	// Main Huff function

unsigned char **data;									// charactor array
int *freq;									// frequency array
int N, charnum, bitnum;			// calculate # of characters and bits

#define INVALID (-2)

char *fgetutf8c(void)
{
	unsigned char c;
	unsigned char *str;
	int len = 1;
	c = getchar();
	if (c <= 0x7F) {
		len = 2;
		str = (char *)malloc(len * sizeof(char));
		str[0] = c;
		str[1] = '\0';
	} else if ((c <= 0xDF) && (c >= 0xC0)) {
		len = 3;
		str = (char *)malloc(len * sizeof(char));
		str[0] = c;
		str[1] = getchar();
		str[2] = '\0';
	} else if ((c <= 0xEF) && (c >= 0xE0)) {
		len = 4;
		str = (char *)malloc(len * sizeof(char));
		str[0] = c;
		str[1] = getchar();
		str[2] = getchar();
		str[3] = '\0';
	} else if ((c <= 0xF7) && (c >= 0xF0)) {
		len = 5;
		str = (char *)malloc(len * sizeof(char));
		str[0] = c;
		str[1] = getchar();
		str[2] = getchar();
		str[3] = getchar();
		str[4] = '\0';
	}
	return str;
}

int main() 
{ 
	readInput();
	printInput();
    //Huff(data, freq, N); 
  
    return 0; 
} 

void readInput(void)            			    // read all inputs
{
	unsigned char *c;										// initialize
	int i, j, found = 0;
	N = 0;
	charnum = 0;
	bitnum = 0;
	c = fgetutf8c();
	// while (c[0] != EOF) {			// before text ends
	for (int k = 0; k < 500; k++){
		if (N == 0) {							// first text
			data =	(unsigned char **)malloc(sizeof(unsigned char *));
			freq =	(int *)malloc(1);
			data[0] = (unsigned char *)malloc(strlen(c) * sizeof(unsigned char));
			for (j = 0; j < strlen(c); j++) {
				data[0][j] = c[j];
			}
			freq[0] = 1;
			N++;
		} else {								// if existed
			for (i = 0; i < N; i++) {
				if (strcmp(data[i], c) == 0) {				// store and add frequency
					found = 1;
					freq[i]++;
				} 
			}
			if (found == 0) {					// if new character
				data = (unsigned char **)realloc((data), (N + 1)* sizeof(unsigned char *));
				freq = (int *)realloc((freq), (N + 1)* sizeof(int));
				data[N] = (unsigned char *)malloc(strlen(c) * sizeof(unsigned char));
				for (j = 0; j < strlen(c); j++) {
					data[N][j] = c[j];
				}
				freq[N] = 1;
				// printf("%s", data[N]);
				N++;
			}
			found = 0;
		}
		charnum++;
		c = fgetutf8c();
	}
}

void printInput(void)               // print character and frequency list
{
	int i;
	
	for (i = 0; i < N; i++) {
		printf("%s: %d times\n", data[i], freq[i]);
	}
} 
  
Node* newNode(unsigned char *data, int freq) 			// initialize a new node
{
	int i;
	Node* tmp  = (Node*)malloc(sizeof(Node)); 
	tmp->data = (char *)malloc(strlen(data) * sizeof(char));
	tmp->left = NULL;
	tmp->right = NULL; 
	for (i = 0; i < strlen(data); i++) {
		tmp->data[i] = data[i];
	}
	tmp->data = data; 
	tmp->freq = freq; 
  
	return tmp; 
} 
  
void Heapify(Heap* Heap, int i) 			// Heapify function
{ 
	int s = i; 
	int l = 2 * i + 1; 					// left child
	int r = 2 * i + 2; 					// right child
	Node* t; 
  
	if (l < Heap->size && Heap->array[l]->freq < Heap->array[s]->freq) {
		s = l; 							// left smaller
	}
	if (r < Heap->size && Heap->array[r]->freq < Heap->array[s]->freq) {
		s = r; 							// right smaller
	}
	if (s != i) { 						// swap with smaller one
		t = Heap->array[s];
		Heap->array[s] = Heap->array[i];
		Heap->array[i] = t;
		Heapify(Heap, s); 				// heapify children
	} 
} 
  
Heap* HeapSort(Heap* Heap, int n)			// calling Heapify
{	
	int i;
	for (i = (n - 1) / 2; i >= 0; i--) {
		Heapify(Heap, i); 
	}
	return Heap;
}

Node* GetMin(Heap* Heap) 				// get the minimum node and remove it
{ 
	Node* temp = Heap->array[0]; 		// get minimum

	Heap->array[0] = Heap->array[Heap->size - 1];  // get the biggest
	Heap->size--; 						// remove one
	Heapify(Heap, 0); 					// Heapify agian
  
	return temp; 						// return min
} 

Node* BMT(Heap* HeapTree)				// Binary Merge Tree function
{
	Node *left, *right, *mid; 			// Initialize
	int m;
	
	while (HeapTree->size != 1) { 
	
		right = GetMin(HeapTree); 		// get left child
		left = GetMin(HeapTree); 		// get right child
  
		mid = newNode('\0', left->freq + right->freq); 	// create merged node
		
		mid->left = left; 				// assign child to merged node
		mid->right = right; 
  
		HeapTree->size++; 
		m = HeapTree->size - 1; 		// remove 2 element and add 1
  
		while (m && mid->freq < HeapTree->array[(m - 1) / 2]->freq) { 
			HeapTree->array[m] = HeapTree->array[(m - 1) / 2]; 
			m = (m - 1) / 2;		// find position and insert merged node
		} 
		HeapTree->array[m] = mid; 
	} 
  
	return GetMin(HeapTree); 			// return the root of HeapTree
}
  
void printCodes(Node* node, int *code, int top) 	// Print Huff Codes
{ 
	int i, b = 0;
	if (!(node->left) && !(node->right)) { 		// if it is a leaf
		printf("  %s: ", node->data); 

		for (i = 0; i < top; i++) { 
			printf("%d", code[i]); 
			b++;
		}
		printf("\n");

		for (i = 0; i < N; i++) {				// calculate # of bits
			if (strcmp(data[i], node->data) == 0) {
				bitnum = bitnum + b * freq[i];	
			}
		}
    } 
    
	if (node->left) { 							// if traverse to left child
		code[top] = 0; 							// code add 0
		printCodes(node->left, code, top + 1); 
	} 
  
	if (node->right) { 							// if traverse to right child
		code[top] = 1; 							// code add 1
		printCodes(node->right, code, top + 1); 
	}   
} 
  
void Huff(unsigned char **data, int *freq, int size) 			// main Huff function
{ 
	Heap* HeapTree; 								// Initialize a Heap tree
	Node *root; 									// initialize a root node
	int i;						
	int code[100], top = 0, mod; 				// code to store huff codes

	HeapTree = (Heap*)malloc(sizeof(Heap)); 		// open space
	HeapTree->size = size; 
	HeapTree->array = (Node**)malloc(size * sizeof(Node*)); 
  
	for (i = 0; i < size; ++i) {					// assigned original array
		HeapTree->array[i] = newNode(data[i], freq[i]); 
	}
	
	HeapTree = HeapSort(HeapTree, size - 1);		// sort heap array
 	
	root = BMT(HeapTree);							// create binary merge tree

	printf("Huffman coding:\n"); 

	printCodes(root, code, top); 					// print huffman codes
	
	printf("Number of Chars read: %d\n", charnum);
    mod = bitnum % 8;
    if (mod) mod = 1;
	printf("  Huffman Coding needs %d bits, %d bytes\n", 
				bitnum, bitnum / 8 + mod);
	printf("  Ratio = %g%%\n", 
				(float)(bitnum / 8 + mod) / (float)charnum * 100);
	
} 
