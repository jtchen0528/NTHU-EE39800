// EE3980 HW09 Encoding ASCII Texts
// 106061146, Jhao-Ting, Chen
// 2020/05/14

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define MAX_TREE_HT 1000 

struct MinHeapNode { 
    char data; 
    unsigned freq; 
    struct MinHeapNode *left, *right; 
}; 
  
struct MinHeap { 
    unsigned size; 
    unsigned capacity; 
    struct MinHeapNode** array; 
}; 

void readInput(void);           			// read all inputs
void printInput(void);                     	// print Input
struct MinHeapNode* newNode(char data, unsigned freq);
struct MinHeap* createMinHeap(unsigned capacity);
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b);
void minHeapify(struct MinHeap* minHeap, int idx);
int isSizeOne(struct MinHeap* minHeap);
struct MinHeapNode* extractMin(struct MinHeap* minHeap);
void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode);
void buildMinHeap(struct MinHeap* minHeap);
int printArr(int arr[], int n);
int isLeaf(struct MinHeapNode* root);
struct MinHeap* createAndBuildMinHeap(char data[], int freq[], int size);  
struct MinHeapNode* buildHuffmanTree(char data[], int freq[], int size);
void printCodes(struct MinHeapNode* root, int arr[], int top);
void HuffmanCodes(char data[], int freq[], int size); 

char *data;
int N, charnum, bitnum = 0;
int *freq;

int main() 
{ 
	readInput();
	//printInput();
    HuffmanCodes(data, freq, N); 
  
    return 0; 
} 

void readInput(void)            			    // read all inputs
{
	char c;
	int f = 0, i, found = 0;
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
		//printf("%c", c);
	}
}

void printInput(void)                          // print adjacent list
{
	int i;
	
	for (i = 0; i < N; i++) {
		printf("%c: %d times\n", data[i], freq[i]);
	}
} 
  
struct MinHeapNode* newNode(char data, unsigned freq) 
{ 
    struct MinHeapNode* temp  = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode)); 
  
    temp->left = NULL;
	temp->right = NULL; 
    temp->data = data; 
    temp->freq = freq; 
  
    return temp; 
} 
  
struct MinHeap* createMinHeap(unsigned capacity) 
{ 
    struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap)); 
  
    minHeap->size = 0; 
    minHeap->capacity = capacity; 
    minHeap->array = (struct MinHeapNode**)malloc(minHeap->capacity * sizeof(struct MinHeapNode*)); 
    
	return minHeap; 
} 
  
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b) 
{ 
    struct MinHeapNode* t = *a; 
    *a = *b; 
    *b = t; 
} 
  
void minHeapify(struct MinHeap* minHeap, int idx) 
{ 
    int smallest = idx; 
    int left = 2 * idx + 1; 
    int right = 2 * idx + 2; 
  
    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq) 
        smallest = left; 
  
    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq) 
        smallest = right; 
  
    if (smallest != idx) { 
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]); 
        minHeapify(minHeap, smallest); 
    } 
} 
  
int isSizeOne(struct MinHeap* minHeap) 
{ 
    return (minHeap->size == 1); 
} 
  
struct MinHeapNode* extractMin(struct MinHeap* minHeap) 
{ 
    struct MinHeapNode* temp = minHeap->array[0]; 
    minHeap->array[0] = minHeap->array[minHeap->size - 1]; 
  
    --minHeap->size; 
    minHeapify(minHeap, 0); 
  
    return temp; 
} 
  
void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode) 
{ 
    ++minHeap->size; 
    int i = minHeap->size - 1; 
  
	while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) { 
        minHeap->array[i] = minHeap->array[(i - 1) / 2]; 
        i = (i - 1) / 2; 
    } 
  
    minHeap->array[i] = minHeapNode; 
} 
  
void buildMinHeap(struct MinHeap* minHeap) 
{ 
  
    int n = minHeap->size - 1; 
    int i; 
  
    for (i = (n - 1) / 2; i >= 0; --i) 
        minHeapify(minHeap, i); 
} 
  
int printArr(int arr[], int n) 
{ 
    int i, b = 0; 
    for (i = 0; i < n; ++i) { 
        printf("%d", arr[i]); 
		b++;
	}
    printf("\n");
	return b;	
} 
  
int isLeaf(struct MinHeapNode* root) 
{ 
    return !(root->left) && !(root->right); 
} 
  
struct MinHeap* createAndBuildMinHeap(char data[], int freq[], int size)  
{ 
  
    struct MinHeap* minHeap = createMinHeap(size); 
  
    for (int i = 0; i < size; ++i) 
        minHeap->array[i] = newNode(data[i], freq[i]); 

    minHeap->size = size; 
    buildMinHeap(minHeap); 
  
    return minHeap; 
} 
  
struct MinHeapNode* buildHuffmanTree(char data[], int freq[], int size) 
  
{ 
    struct MinHeapNode *left, *right, *top; 
    struct MinHeap* minHeap = createAndBuildMinHeap(data, freq, size); 
    
	while (!isSizeOne(minHeap)) { 
  
        left = extractMin(minHeap); 
        right = extractMin(minHeap); 
  
        top = newNode('$', left->freq + right->freq); 
  
        top->left = left; 
        top->right = right; 
  
        insertMinHeap(minHeap, top); 
    } 
  
    return extractMin(minHeap); 
} 
  
void printCodes(struct MinHeapNode* root, int arr[], int top) 
{ 
	int i, b;
    if (root->left) { 
        arr[top] = 0; 
        printCodes(root->left, arr, top + 1); 
    } 
  
    if (root->right) { 
        arr[top] = 1; 
        printCodes(root->right, arr, top + 1); 
    } 
  
    if (isLeaf(root)) { 
		if (root->data == '\n') {
			printf("  '\\n': "); 	
		} else if (root->data == ' ') {
			printf("  ' ': "); 	
		} else {
			printf("  %c: ", root->data); 
		}
        b = printArr(arr, top); 
		for (i = 0; i < N; i++) {
			if (data[i] == root->data) {
				bitnum = bitnum + b * freq[i];	
			}
		}
    } 
} 
  
void HuffmanCodes(char data[], int freq[], int size) 
{ 
    struct MinHeapNode* root = buildHuffmanTree(data, freq, size); 
  
    int arr[MAX_TREE_HT], top = 0, mod; 
  
	printf("Huffman coding:\n"); 
    printCodes(root, arr, top); 
	printf("Number of Chars read: %d\n", charnum);
    mod = bitnum % 8;
    if (mod) mod = 1;
	printf("  Huffman Coding needs %d bits, %d bytes\n", bitnum, bitnum / 8 + mod);
	printf("  Ratio = %f\n", (float)bitnum / 8 / (float)charnum);
} 
  
