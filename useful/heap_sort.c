#include <stdio.h>
#include <stdlib.h>

void heapify(int arr[], int n, int i);
void heapSort(int arr[], int n);
void printArray(int arr[], int n);
void swap(int *a, int *b);
int extractMin(int arr[], int n);
void insertMinHeap(int arr[], int n, int a);

// Driver program
int main()
{
    int arr[] = {12, 11, 13, 5, 6, 7, 8, -3};
    int n = sizeof(arr)/sizeof(arr[0]);

	printf("Original: ");
    printArray(arr, n);
    
	heapSort(arr, n);

	printf("sorted: ");
    printArray(arr, n);

	printf("extract Min: %d, " , extractMin(arr, n));
	printArray(arr, n - 1);

	printf("add 10: ");
	insertMinHeap(arr, n - 1, 10);
    printArray(arr, n);


}

void swap(int *a, int *b)
{   
    int t = *a; 
    *a = *b; 
    *b = t; 
} 

void heapify(int arr[], int n, int i)
{
    int largest = i; // Initialize largest as root
    int l = 2*i + 1; // left = 2*i + 1
    int r = 2*i + 2; // right = 2*i + 2

    // If left child is larger than root
    if (l < n && arr[l] > arr[largest])
        largest = l;

    // If right child is larger than largest so far
    if (r < n && arr[r] > arr[largest])
        largest = r;

    // If largest is not root
    if (largest != i)
    {
        swap(&arr[i], &arr[largest]);

        // Recursively heapify the affected sub-tree
        heapify(arr, n, largest);
    }
}

// main function to do heap sort
void heapSort(int arr[], int n)
{
    // Build heap (rearrange array)
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    // One by one extract an element from heap
    for (int i=n-1; i>0; i--)
    {
        // Move current root to end
        swap(&arr[0], &arr[i]);

        // call max heapify on the reduced heap
        heapify(arr, i, 0);
    }
}

// minimum value node from heap 
int extractMin(int arr[], int n)   
{ 
  
    int temp = arr[0]; 
    arr[0] = arr[n - 1]; 
    arr[n - 1] = -1000; 

    heapSort(arr, n - 1); 
  
    return temp; 
} 

void insertMinHeap(int arr[], int n, int a)  
{  
	n++;
	int i = n - 1;
    while (i >= 0 && a < arr[(i - 1) / 2]) { 
        arr[i] = arr[(i - 1) / 2]; 
        i = (i - 1) / 2; 
    } 
	
  
    arr[i] = a; 
	heapSort(arr, n); 
} 

/* A utility function to print array of size n */
void printArray(int arr[], int n)
{
    for (int i=0; i<n; ++i)
        printf("%d ", arr[i]);
    printf("\n");
}

