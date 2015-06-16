#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//
//
//
typedef int ElementType;
/*typedef struct QuickSort {
	int Capacity;
	int Size;
	ElementType *Elements;
}QuickSort;*/
//
//
//

void Qsort(ElementType A[], int N, char* str);
void Swap(int* A, int* B);
void Quicksort(ElementType A[], int l, int r, char* str);
int Partition(ElementType A[] ,int l, int r, char* str);

int main(int argc, char** argv)
{
	FILE* input = fopen(argv[1], "r");
	char str[40];
	char cond;
	int t = 0, i = 0;
	int data[40], data2[40], data3[40];
	fscanf(input, "%s", str);
	printf("%s ", str);

	do
	{
		fscanf(input, "%d", &data[i]);
		cond = fgetc(input);
		i++;
	} while (cond != '\n');

	for (t = 0; t < i; t++)
	{
		printf("%d ", data[t]);
	}	printf("\n");

	Qsort(data, i, str);

	printf("Result of QuickSort:");
	for (t = 0; t < i; t++)
		printf("%d ", data[t]);
	printf("\n");

	printf("---------------------------------------------------------------\n");
	
	fscanf(input, "%s", str);
	printf("%s ", str);
	i = 0;
	do
	{
		fscanf(input, "%d", &data2[i]);
		cond = fgetc(input);
		i++;
	} while (cond != '\n');
	for (t = 0; t < i; t++)
	{
		printf("%d ", data2[t]);
	}	printf("\n");

	Qsort(data2, i, str);
	
	printf("Result of QuickSort:");
	for (t = 0; t < i; t++)
	{
		printf("%d ", data2[t]);
	}	printf("\n");


	printf("---------------------------------------------------------------\n");

	fscanf(input, "%s", str);
	printf("%s ", str);
	i = 0;
	
	while (1)
	{
		fscanf(input, "%d", &data3[i]);
		if (fgetc(input) == EOF)
		{
			break;
		}
		i++;
	}
	for (t = 0; t < i; t++)
	{
		printf("%d ", data3[t]);
	}	printf("\n");

	Qsort(data3, i, str);

	printf("Result of QuickSort:");
	for (t = 0; t < i; t++)
	{
		printf("%d ", data3[t]);
	}	printf("\n");

}

void Qsort(ElementType A[], int N, char* str)
{
	Quicksort(A, 0, N - 1, str);
}


void Swap(int* A, int* B)
{
	int Temp;

	Temp = *A;
	*A = *B;
	*B = Temp;
}

void Quicksort(ElementType A[], int l,int r, char* str)
{
	
	if (l < r)
	{

		ElementType p, i;

		p = Partition(A, l, r, str);
		printf("<");
		for (i = l; i < p; i++)
		{
			printf("%d ", A[i]);
		}	printf(">");
		printf("<pviot:%d index=%d>", A[p],p);

		printf("<");
		for (i = p + 1; i <= r; i++)
		{
			printf("%d ", A[i]);
		}	printf(">");
		printf("\n");
		Quicksort(A, l, p-1 , str);
		
		Quicksort(A, p+1, r, str);		
		
	}
}

int Partition(ElementType A[], int l, int r, char* str)
{
	int i, j, k;
	ElementType pivot;
	
	if (strcmp(str, "leftmost") == 0)
	{
		pivot = A[l];// pviot=A[l]
		Swap(&A[r], &A[l]);
		i = l-1;
		j = r;
	}
	else if (strcmp(str, "rightmost") == 0)
	{
		pivot =A[r];// pviot=A[r]
		i = l-1;
		j = r;
	}
	else
	{
		pivot = A[(l+r)/2];// pviot=A[l+r/2]
		Swap(&A[(l+r)/2], &A[r]);
		i = l-1;
		j = r;

	}

	while(1)
	{
		while (A[++i] < pivot && l < r){}
		while (A[--j] > pivot && l < r){}

		if (i < j )
		{
			Swap(&A[i], &A[j]);
		}
		else
				break;
	} 
	//in case of (j<i)
	Swap(&A[r], &A[i]);

	return i;
		
}
