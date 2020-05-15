// EE3980 HW09 Encoding ASCII Texts
// 106061146, Jhao-Ting, Chen
// 2020/05/14

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

typedef struct sNode { 
    char data; 
    int freq; 
    struct sNode *left, *right; 
} Node;
  
typedef struct sHeap { 
    int size; 
    Node** array; 
} Heap;

void readInput(void);           			// read all inputs
void printInput(void);                     	// print Input
Node* newNode(char data, int freq);
void Heapify(Heap* Heap, int idx);
Node* GetMin(Heap* Heap);
int printCode(int *code, int n);
void printCodes(Node* root, int *code, int top);
void Huff(char *data, int *freq, int size); 

char *data;
int N, charnum, bitnum = 0;
int *freq;

int main() 
{ 
	readInput();
	//printInput();
    Huff(data, freq, N); 
  
    return 0; 
} 

void readInput(void)            			    // read all inputs
{
	char c;
	int i, found = 0;
	N = 0;
	charnum = 0;
	while ((c = getchar()) != EOF) {
		if (N == 0) {
			data =	(char *)malloc(1);
			freq =	(int *)malloc(1);
			data[0] = c;
			freq[0] = 1;
			N++;
		} else {
			for (i = 0; (i < N); i++) {
				if (data[i] == c) {
					found = 1;
					freq[i]++;
				} 
			}
			if (found == 0) {
				data = (char *)realloc((data), (N + 1)* sizeof(char));
				freq = (int *)realloc((freq), (N + 1)* sizeof(int));
				data[N] = c;
				freq[N] = 1;
				N++;
			}
			found = 0;
		}
		charnum++;
	}
}

void printInput(void)                          // print adjacent list
{
	int i;
	
	for (i = 0; i < N; i++) {
		printf("%c: %d times\n", data[i], freq[i]);
	}
} 
  
Node* newNode(char data, int freq) 
{ 
	Node* tmp  = (Node*)malloc(sizeof(Node)); 
  
    tmp->left = NULL;
	tmp->right = NULL; 
    tmp->data = data; 
    tmp->freq = freq; 
  
    return tmp; 
} 
  
void Heapify(Heap* Heap, int i) 
{ 
    int s = i; 
    int left = 2 * i + 1; 
    int right = 2 * i + 2; 
    Node* t; 
  
    if (left < Heap->size && Heap->array[left]->freq < Heap->array[s]->freq) {
        s = left; 
	}
    if (right < Heap->size && Heap->array[right]->freq < Heap->array[s]->freq) {
        s = right; 
	}
    if (s != i) { 
		t = Heap->array[s];
		Heap->array[s] = Heap->array[i];
		Heap->array[i] = t;
        Heapify(Heap, s); 
    } 
} 
  
Node* GetMin(Heap* Heap) 
{ 
    Node* temp = Heap->array[0]; 

    Heap->array[0] = Heap->array[Heap->size - 1]; 
    Heap->size--; 
    Heapify(Heap, 0); 
  
    return temp; 
} 
  
int printCode(int *code, int n) 
{ 
    int i, b = 0; 
    for (i = 0; i < n; ++i) { 
        printf("%d", code[i]); 
		b++;
	}
    printf("\n");
	return b;	
} 

void printCodes(Node* node, int *code, int top) 
{ 
	int i, b;
    if (!(node->left) && !(node->right)) { 
		if (node->data == '\n') {
			printf("  '\\n': "); 	
		} else if (node->data == ' ') {
			printf("  ' ': "); 	
		} else {
			printf("  %c: ", node->data); 
		}
        b = printCode(code, top); 
		for (i = 0; i < N; i++) {
			if (data[i] == node->data) {
				bitnum = bitnum + b * freq[i];	
			}
		}
    } 
    
	if (node->left) { 
        code[top] = 0; 
        printCodes(node->left, code, top + 1); 
    } 
  
    if (node->right) { 
        code[top] = 1; 
        printCodes(node->right, code, top + 1); 
    }   
} 
  
void Huff(char *data, int *freq, int size) 
{ 
    Heap* HeapTree; 
    Node *left, *right, *mid, *root; 
	int n, i, m;
	int code[100], topNode = 0, mod; 

    HeapTree = (Heap*)malloc(sizeof(Heap)); 
    HeapTree->size = size; 
    HeapTree->array = (Node**)malloc(size * sizeof(Node*)); 
  
    for (i = 0; i < size; ++i) {
        HeapTree->array[i] = newNode(data[i], freq[i]); 
	}

	n = HeapTree->size - 1; 
  
    for (i = (n - 1) / 2; i >= 0; i--) {
        Heapify(HeapTree, i); 
	}
    
	while (HeapTree->size != 1) { 
        
		right = GetMin(HeapTree); 
        left = GetMin(HeapTree); 
  
        mid = newNode('\0', left->freq + right->freq); 
  
        mid->left = left; 
        mid->right = right; 
  
		HeapTree->size++; 
		m = HeapTree->size - 1; 
  
		while (m && mid->freq < HeapTree->array[(m - 1) / 2]->freq) { 
			HeapTree->array[m] = HeapTree->array[(m - 1) / 2]; 
			m = (m - 1) / 2;
		} 
		HeapTree->array[m] = mid; 
    } 
  
    root = GetMin(HeapTree); 
  
	printf("Huffman coding:\n"); 
    printCodes(root, code, topNode); 
	printf("Number of Chars read: %d\n", charnum);
    mod = bitnum % 8;
    if (mod) mod = 1;
	printf("  Huffman Coding needs %d bits, %d bytes\n", bitnum, bitnum / 8 + mod);
	printf("  Ratio = %g%%\n", (float)(bitnum / 8 + mod) / (float)charnum * 100);
	
} 
