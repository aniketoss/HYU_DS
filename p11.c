#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX_BUFF_SIZE 100
#define TRUE 1
#define FALSE 0
//----------------------------------------------------------------------------------------------
//         TYPE RE_CASTING BELOW
//----------------------------------------------------------------------------------------------
typedef struct Bit{
	unsigned int bit : 1;
} Bit;
// Hash function pointer 
typedef unsigned int(*hashfunc_t)(const char *);
typedef hashfunc_t Hash_Func;
// Bloom Filter
typedef struct BloomFilters{
	int FILTER_SIZE;//m
	int NUM_HASHFUNC;//k
	Bit *FilterArray;
	hashfunc_t *funcs; // function pointer array for hash functions
} BloomFilters;
//----------------------------------------------------------------------------------------------
//          FUNCTION LISTS
//----------------------------------------------------------------------------------------------
void insert(BloomFilters* B, char *Key);
int lookup(BloomFilters* B, char *Key);
int Hash(BloomFilters* B, unsigned int Key);
BloomFilters* CreateBloomFilter(int m, int b, int k);
//
//   LISTS OF HASH FUNCTION
//
unsigned int Hash1(const char* key);
unsigned int Hash2(const char* key);
unsigned int Hash3(const char* key);
unsigned int Hash4(const char* key);
unsigned int Hash5(const char* key);
unsigned int Hash6(const char* key);
unsigned int Hash7(const char* key);
unsigned int Hash8(const char* key);
unsigned int Hash9(const char* key);
unsigned int Hash10(const char* key);

int main(int argc, char** argv)
{
	FILE* input = fopen(argv[1], "r");
	BloomFilters* my_Bloom_Filter=NULL;
	char Buff[MAX_BUFF_SIZE];
	char* str=NULL;
	int b, k, m, i;
	int counter=0;
	char* n;
	int result;
	n = fgets(Buff, 100, input);
	b = (int)(1.44*(log2(1 /0.01)));
	k = (int)(0.693*b);
	m = atoi(n)*b;

	str = (char*)malloc(sizeof(char)* 100);

	printf("b=%2d k=%2d m=%2d\n", b, k, m);

	my_Bloom_Filter = CreateBloomFilter(m, b, k);

	while (counter<7)
	{
	
		//str = fgets(Buff, 100, input);
		

		fscanf(input, "%[^ ]s", str);

		printf("Counter::%d String::%s\n", counter ,str);

		insert(my_Bloom_Filter, str);
		
		counter++;


	}	printf("\n");

	printf("------------------------------------------------------------------------\n");

	while (1)
	{
		if (feof(input))
		{
			break;
		}

		fscanf(input, "%s", str);
		printf("%s\n", str);

		result=lookup(my_Bloom_Filter, str);

		if (result == 1)
		{
			printf("Result::true\n");
		}
		else
		{
			printf("Result::false\n");
		}
	}		printf("\n");

	fclose(input);
	return 0;
}
//----------------------------------------------------------------------------------------------
//         FUNCTION DEFINITION BELOW
//------------------ ----------------------------------------------------------------------------
void insert(BloomFilters* B, char *Key)
{
	int Mapping;
	int i;
	for (i = 0; i< B->NUM_HASHFUNC; i++)
	{
		Mapping = Hash(B, (*B->funcs[i])(Key));
		printf("index::%d\n", Mapping);
		B->FilterArray[Mapping].bit = 1;
		printf("Filter_Array[%2d] =%2d\n", Mapping, B->FilterArray[Mapping].bit);
	}
	
	
}
int lookup(BloomFilters* B, char *Key)
{
	int i;
	int Mapping, Comparator=1, result;

	printf("%s\n", Key);
	for (i = 0; i < B->NUM_HASHFUNC; i++)
	{
		Mapping = Hash(B, (*B->funcs[i])(Key));
		result =  Comparator & B->FilterArray[Mapping].bit;
		printf("comparator::%d bit::%d index::[%2d]\n", Comparator, B->FilterArray[Mapping].bit, Mapping);

		
		if (Comparator!= B->FilterArray[Mapping].bit)
		{
			return FALSE;
		}
	}
	return TRUE; //  true == 1 , the other numbers false
}
int Hash(BloomFilters* B, unsigned int Key)
{
	printf("Key:%d\n", Key);
	return (Key % B->FILTER_SIZE);
}
BloomFilters* CreateBloomFilter(int m, int b, int k)
{
	BloomFilters* bloom_Filter;
	int i;

	bloom_Filter = (BloomFilters*)malloc(sizeof(BloomFilters));
	if (bloom_Filter == NULL)
	{
		printf("FATAL ERROR: OUT OF SPACE!");
		return NULL;
	}
	else
	{
		bloom_Filter->FILTER_SIZE = m;
		bloom_Filter->FilterArray = (Bit*)malloc(sizeof(Bit)*m);
		if (bloom_Filter->FilterArray == NULL)
		{
			printf("OUT OF SPACE!");
		}
		for (i = 0; i < bloom_Filter->FILTER_SIZE; i++)
		{
			bloom_Filter->FilterArray[i].bit = 0;
		}
		
		bloom_Filter->NUM_HASHFUNC = k;
		
		bloom_Filter->funcs = (hashfunc_t*)malloc(sizeof(hashfunc_t)* k);
		
		if (bloom_Filter->funcs == NULL)
		{
			printf("OUT OF SPACE!");
		}
		//--------------------------------------------------------------------------------------
		//       USING Pointer with respect to Function 
		//--------------------------------------------------------------------------------------
		bloom_Filter->funcs[0] = &Hash1;
		bloom_Filter->funcs[1] = &Hash2;
		bloom_Filter->funcs[2] = &Hash3;
		bloom_Filter->funcs[3] = &Hash4;
		bloom_Filter->funcs[4] = &Hash5;
		bloom_Filter->funcs[5] = &Hash6;
	}
	return bloom_Filter;
}

unsigned int Hash1(const char *Key){
	int HashVal = 0;
	for (; *Key != '\0'; Key++)
		HashVal += *Key + 31;
	return HashVal;
}
unsigned int Hash2(const char *Key){
	int HashVal = 0;
	for (; *Key != '\0'; Key++)
		HashVal += *Key * 31;
	return HashVal;
}
unsigned int Hash3(const char *Key){
	int HashVal = 0;
	for (; *Key != '\0'; Key++)
		HashVal += (*Key + 31) * 11;
	return HashVal;
}
unsigned int Hash4(const char *Key){
	int HashVal = 0;
	for (; *Key != '\0'; Key++)
		HashVal = (HashVal >> 5) + *Key++;
	return HashVal;
}
unsigned int Hash5(const char *Key){
	int HashVal = 0;
	for (; *Key != '\0'; Key++)
		HashVal = (HashVal << 5) + *Key++;
	return HashVal;
}
unsigned int Hash6(const char *Key){
	int HashVal = 0;
	for (; *Key != '\0'; Key++)
		HashVal = (HashVal << 5) + (HashVal >> 5) + *Key++;
	return HashVal;
}
unsigned int Hash7(const char *Key){
	int HashVal = 0;
	for (; *Key != '\0'; Key++)
		HashVal += (*Key * 11) + 31;
	return HashVal;
}
unsigned int Hash8(const char *Key){
	int HashVal = 0;
	for (; *Key != '\0'; Key++)
		HashVal = ((HashVal << 5) + (HashVal >> 5)) * 11 + *Key++;
	return HashVal;
}
unsigned int Hash9(const char *Key){
	int HashVal = 0;
	for (; *Key != '\0'; Key++)
		HashVal = (HashVal >> 5) * 11 + *Key++;
	return HashVal;
}
unsigned int Hash10(const char *Key){
	int HashVal = 0;
	for (; *Key != '\0'; Key++)
		HashVal = (HashVal << 5) * 11 + *Key++;
	return HashVal;
}
