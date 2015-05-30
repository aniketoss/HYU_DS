#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 100

//------------------------------------------------------------------------
//							TYPE RE-CASTING
//------------------------------------------------------------------------


typedef int ElementType;
typedef struct Vertex* ptr_to_Vertex;
typedef struct Graph* ptr_to_Graph;
typedef struct Queue* ptrQueue;
typedef struct Vertex{

	ElementType Edge;
	ElementType In_Degree;// Define In-degree of vertex V.
	int index;

}Vertex;

typedef struct Graph{

	Vertex* List_of_Vertex;
	Vertex**  Map_Of_Vertex;
	ElementType Num_of_Vertex;// All numbers of v's.

}Graph;

typedef struct Queue{

	int Capacity;
	int Q_Size;
	int front;
	int rear;
	Vertex* vertex;

}Queue;

//------------------------------------------------------------------------
//							FUNCTION LISTS
//------------------------------------------------------------------------

static int counter = 0;
ptrQueue CreateQueue(ElementType Size);
Graph Initailze_Graph(ElementType number_of_vertex);
Graph Set_Graph(int row, int column, Graph G);
Vertex Find_NewVertex_of_IndegreeZero(Graph G); //
void Topological_Sort(Graph G);// not possible if graph has a cycle.
void EnQueue(Vertex element, ptrQueue Q);
void DisposeQueue(ptrQueue Q);
void Dispose_all_graph(Graph G);
Vertex DeQueue(ptrQueue Q);
int IsEmpty(ptrQueue Q);
int Succ(int Value, ptrQueue Q);// to implement Circular Queue

//------------------------------------------------------------------------
//						Static Variable For Counting	
//------------------------------------------------------------------------


void main(int argc, char** argv)
{
	// FILE* input = fopen(argv[1], "r");
	FILE* input = fopen("input.txt", "r");
	Graph new_Graph;
	char Buffer[BUFFER_SIZE];
	char str[40][40];
	char str2[40][40];
	char* token;
	int j;
	int line_FeedCounter = 0, Counter_for_vertex = 0, i = 0;

	while (1)
	{
		if (line_FeedCounter == 0)
		{
			fgets(Buffer, 100, input);
			printf("Buffer = %s\n", Buffer);
			token = strtok(Buffer, " ");
			printf("Token = %s\n", token);
			while (token != NULL)
			{
				strcpy(str[i++], token);
				token = strtok(NULL, " ");
				printf("Token = %s\n", token);
				Counter_for_vertex++;
			}

			new_Graph = Initailze_Graph(Counter_for_vertex);

			for (i = 0; i < Counter_for_vertex; i++)
			{
				for (j = 0; j < Counter_for_vertex; j++)
				{
					printf("%2d", new_Graph.Map_Of_Vertex[i][j].Edge);
				}
				printf("\n");
			}
			printf("------------------------------------------ \n");
			++line_FeedCounter;
		}

		if (line_FeedCounter == 1)
		{
			fgets(Buffer, 100, input);

			printf("Buffer = %s\n", Buffer);
			
			token = strtok(Buffer, " -");			
			
			printf("Token = %s\n", token);

			while (token != NULL)
			{
				strcpy(str2[0], token);

				token = strtok(NULL, " -");
				
				printf("Token = %s\n", token);
				
				strcpy(str2[1], token);

				Set_Graph(atoi(str2[0]), atoi(str2[1]), new_Graph);

				token = strtok(NULL, " -");

				printf("Token = %s\n", token);

				for (i = 0; i < Counter_for_vertex; i++)
				{
					for (j = 0; j < Counter_for_vertex; j++)
					{
						printf("%2d", new_Graph.Map_Of_Vertex[i][j].Edge);
					}
					printf("\n");
				}
				printf("------------------------------------------ \n");

			}
			line_FeedCounter += 1;
		}
		else
		{
			printf("Index of V:");
			for (i = 0; i < Counter_for_vertex; i++)
			{

				printf("%2d", new_Graph.List_of_Vertex[i].index);
			}
			printf("\n");
			printf("In-Degree: ");
			for (i = 0; i < Counter_for_vertex; i++)
			{

				printf("%2d", new_Graph.List_of_Vertex[i].In_Degree);
			}
			printf("\n");
			break;
		}
	}

	Topological_Sort(new_Graph);

	//---------------------------------------------------------------------------------------------
	//			FREE ALL DYNAMIC ALLOCATIONS & CLOSE FILE STREAM
	//---------------------------------------------------------------------------------------------
	Dispose_all_graph(new_Graph);
	fclose(input); 

}

Graph Initailze_Graph(ElementType NumberOfVertex)
{
	int i, j;
	Graph G;

	G.Map_Of_Vertex = (Vertex**)malloc(sizeof(Vertex*)*NumberOfVertex);

	if (G.Map_Of_Vertex == NULL)
	{
		printf("ERROR: OUT OF SPACE!!\n");
	}

	else
	{
		for (i = 0; i < NumberOfVertex; i++)
		{
			G.Map_Of_Vertex[i] = (Vertex*)malloc(sizeof(Vertex)*NumberOfVertex);
			if (G.Map_Of_Vertex[i] == NULL)
			{
				printf("ERROR: OUT OF SPACE!!\n");
			}
		}
	}

	G.List_of_Vertex = (Vertex*)malloc(sizeof(Vertex)*NumberOfVertex);

	if (G.List_of_Vertex == NULL)
	{
		printf("ERROROUT OF SPACE!\n");
	}

	else
	{
		G.Num_of_Vertex = NumberOfVertex;

		for (i = 0; i < NumberOfVertex; i++)
		{
			G.List_of_Vertex[i].index = i + 1;
			
			G.List_of_Vertex[i].In_Degree = 0;

			for (j = 0; j < NumberOfVertex; j++)
			{
				G.Map_Of_Vertex[i][j].Edge = 0;
			}
		}
	}
	return G;
}

Graph Set_Graph(int row, int column, Graph G)
{

	int i, j;
	int all_edge = 0;

	G.Map_Of_Vertex[row - 1][column - 1].Edge = 1; // Set the relation btw V_row -> V_column ex> 1->2 

	for (i = 0; i < G.Num_of_Vertex; i++)
	{
		all_edge = 0;// reset for all-Degree
		for (j = 0; j < G.Num_of_Vertex; j++)
		{
			all_edge += G.Map_Of_Vertex[j][i].Edge; // For calcaulating the in-degree of Vertex v.
			G.List_of_Vertex[i].In_Degree = all_edge;

		}
	}

	return G;
}

void Topological_Sort(Graph G)
{
	ptrQueue myQueue;
	Vertex V;
	int Counter = 0, i = 0, j;
	int* TopNum;
	int Size_for_Queue;

	Size_for_Queue = G.Num_of_Vertex;

	TopNum = (int*)malloc(sizeof(int)* G.Num_of_Vertex);

	myQueue = CreateQueue(Size_for_Queue);

	for (i = 0; i < G.Num_of_Vertex; i++)
	{
		if (G.List_of_Vertex[i].In_Degree == 0)
		{
			EnQueue(G.List_of_Vertex[i], myQueue);
		}

		else { continue; }

	}
	printf("Sequence of Vertices After Topological Sort:");
	while (!IsEmpty(myQueue))
	{
		V = DeQueue(myQueue);
		TopNum[Counter++] = V.index;
		for (i = 0; i < G.Num_of_Vertex; i++)
		{
			if (i == V.index - 1)//|| G.List_of_Vertex[j].In_Degree == 0
			{
				continue;
			}
			
			for (j = 0; j < G.Num_of_Vertex; j++)
			{
				if (G.List_of_Vertex[i].In_Degree != 0 && G.Map_Of_Vertex[V.index-1][i].Edge == 1)
				{	
					G.Map_Of_Vertex[V.index-1][i].Edge = 0;

					--G.List_of_Vertex[i].In_Degree;

					if (G.List_of_Vertex[i].In_Degree == 0)
					{
						EnQueue(G.List_of_Vertex[i], myQueue);
					}
				}

			}
			
		}
		
	}
	
	printf("\n"); 
	printf("--------------------------------------------\n");
	printf("Elements in 2D Matrix After Topological Sort\n");
	for (i = 0; i < G.Num_of_Vertex; i++)
	{


		for (j = 0; j < G.Num_of_Vertex; j++)
		{
			printf("%2d", G.Map_Of_Vertex[i][j].Edge);
		}
		printf("\n");
	}

	if (Counter != G.Num_of_Vertex)
	{
		printf("GRAPH HAS A CYCLE!!\n");
	}
	DisposeQueue(myQueue);
} // after


void Dispose_all_graph(Graph G)
{
	int i;
	int counter;
	counter = G.Num_of_Vertex;
	for (i = 0; i < counter; i++)
	{
		free(G.Map_Of_Vertex[i]);

	}
		free(G.Map_Of_Vertex);

		free(G.List_of_Vertex);
		
}
//---------------------------------------------------------------------------------------
//							FUNCTION FOR SIMPLE CURCULAR QUEUE
//---------------------------------------------------------------------------------------

ptrQueue CreateQueue(ElementType Size) // Creation And Initailzation of QUEUE
{
	ptrQueue Q = (ptrQueue)malloc(sizeof(ptrQueue)*Size);

	if (Q == NULL) { printf("Error: OUT OF SPACE ! \n"); }


	Q->vertex = (Vertex*)malloc(sizeof(Vertex)*Size);

	if (Q->vertex == NULL) { printf("Error: OUT OF SPACE ! \n"); }

	else
	{
		Q->Q_Size = 0;
		Q->front = 1;
		Q->rear = 0;
		Q->Capacity = Size;
	}

	return Q;
}

void EnQueue(Vertex V, ptrQueue Q) // Insertion
{
	if (Q->Q_Size == Q->Capacity)
	{
		printf("QUEUE IS FULL");
	}
	else
	{
		Q->rear = Succ(Q->rear, Q);
		Q->vertex[Q->rear] = V;
		Q->Q_Size += 1;
	}


}

Vertex DeQueue(ptrQueue Q) // Deletion
{
	Vertex temp;

	if (Q->Q_Size == 0)
	{
		printf("Queue is Empty!");

	}
	else

	{
		Q->Q_Size -= 1;
		temp = Q->vertex[Q->front];
		Q->front = Succ(Q->front, Q);

	}
	
	printf("%2d", temp.index);

	return temp;
}

void DisposeQueue(ptrQueue Q)
{
	free(Q->vertex);
	free(Q);
}

int Succ(int Value, ptrQueue Q)
{
	if (++Value == Q->Capacity)
	{
		Value = 0;
	}

	return Value;
}

int IsEmpty(ptrQueue Q)
{
	return Q->Q_Size == 0;
}
