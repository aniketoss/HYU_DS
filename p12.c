#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_BUFF_SIZE 100
#define Infinity 999
#define Not_A_vertex -1
#define TRUE 1
#define FALSE 0 
#define SENTINAL_NODE -1
//
//
//
typedef struct List_Node* List; // ptr with respect to Singly Linked List
typedef struct List_Node* Position;
typedef struct Graph_adjList* Graph;
typedef struct Table_Entry* Table;
typedef int ElementType;
typedef int Vertex;
typedef struct Graph_adjList{
	List* Linked_List;
	int Number_of_Vertices;
}Graph_adjList;
typedef struct List_Node
{
	ElementType Key;
	ElementType Weight;
	List next;
}List_Node;
typedef struct Table_Entry{
	List Header;
	int Index;
	int Know;
	int Distance;
	Vertex Path;
}Table_Entry;
//----------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------
typedef struct HeapStruct* PriorityQueue;
typedef int ElementType;
typedef struct HeapStruct{
	int Capacity;
	int Size;
	Table Elements;
}HeapStruct;
//----------------------------------------------------------------------------------------
//         
//----------------------------------------------------------------------------------------
Graph Initialize_Graph(int num_Vertex);
void Set_Graph(int vtx_u, int vtx_v, int weight, Graph G);
void Insert_Key(ElementType element, ElementType weight, List L);
//----------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------
void Print_adjacencyList(Graph G);
void Set_Table(Vertex Start, Graph G, Table T);
void Print_Path(Vertex V, Table T);
void Dijkstra(Table T, Graph G, Vertex Start);
//----------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------
PriorityQueue CreateHeap(int HeapSize);
void Insert(PriorityQueue H, Table_Entry X);
ElementType DeleteMin(PriorityQueue H, Table T);
void PrintHeap(PriorityQueue H);
int IsFull(PriorityQueue H);
int IsEmpty(PriorityQueue H);

int main(int argc, char** argv)
{
	FILE* input = fopen(argv[1], "r");
	Graph my_Graph;
	Table my_Table;
	char* context, *token;
	char* str_buff = (char*)malloc(sizeof(char)*MAX_BUFF_SIZE);
	char select;
	int counter = 0, Start = 0;
	int data[3];
	//----------------------------------------------------------------------------------------
	//        DATA INPUT FROM INPUT.TXT
	//----------------------------------------------------------------------------------------
	fgets(str_buff, MAX_BUFF_SIZE, input);
	token = strtok_s(str_buff, " ", &context);
	//printf("token:%s\n", token);
	counter++;
	while (token != NULL)
	{
		token = strtok_s(NULL, " \n", &context);
		if (token == NULL)
		{
			break;
		}
		else
		{
			//printf("token:%s\n", token);
			counter++;
		}
	}
	printf("Number of Vertices:%2d\n", counter);// print num_of_vertex
	printf("\n");
	//----------------------------------------------------------------------------------------
	//   CREATE NEW GRAPH
	//----------------------------------------------------------------------------------------
	my_Graph = Initialize_Graph(counter);
	my_Table = (Table)malloc(sizeof(Table_Entry)* my_Graph->Number_of_Vertices + 1);
	printf("\n");
	//----------------------------------------------------------------------------------------
	//        DATA INPUT FROM INPUT.TXT & SET UP THE GRAPH
	//----------------------------------------------------------------------------------------

	fgets(str_buff, MAX_BUFF_SIZE, input);
	token = strtok_s(str_buff, " -", &context);
	printf("-------------------------INPUT DATA LIST-------------------------\n");
	printf(" vertex_u\t vertex_v\t  weight\n");
	while (token != NULL)
	{
		data[0] = atoi(token);
		printf("Data[0]=%2d\t", data[0]);
		token = strtok_s(NULL, " -", &context);
		data[1] = atoi(token);
		printf("Data[1]=%2d\t", data[1]);
		token = strtok_s(NULL, " -", &context);
		data[2] = atoi(token);
		printf("Data[2]=%2d\t", data[2]);
		token = strtok_s(NULL, " -", &context);
		printf("\n");

		//----------------------------------------------------------------------------------------
		//        SET UP THE GRAPH
		//----------------------------------------------------------------------------------------
		Set_Graph(data[0], data[1], data[2], my_Graph); // Set_Graph(Vertex_u, Vertex_v, weight, Graph);
	}
	printf("-----------------------------------------------------------------\n");

	Print_adjacencyList(my_Graph); // Set_Table(Vertex Start, Graph G, Table T)
	while (1)
	{
		printf("Start Source: ");
		scanf("%d", &Start);
		printf("\n");
		Set_Table(Start, my_Graph, my_Table);
		Dijkstra(my_Table, my_Graph, Start);
		printf("Restart?(Y/N): ");
		scanf(" %c", &select);
		printf("\n");

		if (select == 'N')
		{
			break;
		}
	}
	//----------------------------------------------------------------------------------------
	//      FREE DYNAMIC ALLOCACTIONS & CLOSE FILE STREAM
	//----------------------------------------------------------------------------------------
	free(str_buff);

	return 0;;
}
//----------------------------------------------------------------------------------------
//        FUNCTION DEFINITION BELOW
//----------------------------------------------------------------------------------------

Graph Initialize_Graph(int num_Vertex)
{
	Graph G;
	int i;

	G = (Graph)malloc(sizeof(Graph_adjList));

	if (G == NULL)
	{
		printf("Fatal Error occurs! : OUT OF SPACE!\n");
		return NULL;
	}

	G->Linked_List = (List*)malloc(sizeof(List)*num_Vertex + 1);

	if (G->Linked_List == NULL)
	{
		printf("Fatal Error occurs! : OUT OF SPACE!\n");
		return NULL;
	}
	G->Number_of_Vertices = num_Vertex;
	for (i = 1; i <= num_Vertex; i++)
	{
		G->Linked_List[i] = (List)malloc(sizeof(List_Node)*num_Vertex + 1);
	}

	for (i = 1; i <= num_Vertex; i++)
	{
		G->Linked_List[i]->next = NULL;
		printf("NEXT POINTER of List[%d]: %s\n", i, G->Linked_List[i]->next);
	}

	for (i = 1; i <= num_Vertex; i++)
	{
		G->Linked_List[i]->Key = i;
	}
	return G;
}
void Set_Graph(int vtx_u, int vtx_v, int weight, Graph G)
{
	switch (vtx_u)
	{
	case 1:
		Insert_Key(vtx_v, weight, G->Linked_List[1]);
		break;
	case 2:
		Insert_Key(vtx_v, weight, G->Linked_List[2]);
		break;
	case 3:
		Insert_Key(vtx_v, weight, G->Linked_List[3]);
		break;
	case 4:
		Insert_Key(vtx_v, weight, G->Linked_List[4]);
		break;
	case 5:
		Insert_Key(vtx_v, weight, G->Linked_List[5]);
		break;
	case 6:
		Insert_Key(vtx_v, weight, G->Linked_List[6]);
		break;
	case 7:
		Insert_Key(vtx_v, weight, G->Linked_List[7]);
		break;
	default:
		break;

	}

}


void Insert_Key(ElementType element, ElementType weight, List L)
{
	List temp;
	Position P;

	P = L;
	temp = (List)malloc(sizeof(List_Node));

	if (temp == NULL)
	{
		printf("Fatal Error occurs! : OUT OF SPACE!\n");
	}
	else
	{
		temp->Key = element;
		temp->Weight = weight;
		temp->next = P->next;
		P->next = temp;
	}
}

void Print_adjacencyList(Graph G)
{
	int i;
	Position P;

	printf("-------------------------Graph Diagram-------------------------\n");
	printf("Source\tHeader\t  Address current\t   Next v\tWeigth\tNext Address\n");
	for (i = 1; i <= G->Number_of_Vertices; i++)
	{
		P = G->Linked_List[i]->next;

		while (P != NULL)
		{
			printf("%4d\t[%d]\t%2d\t\t%2d\t%2d\t%d\n", i, &G->Linked_List[i]->next, P, P->Key, P->Weight, P->next);
			P = P->next;
		}
	}
	printf("-----------------------------------------------------------------\n");

}

void Set_Table(Vertex Start, Graph G, Table T)
{
	int i, j = 0;

	for (i = 1; i <= G->Number_of_Vertices; i++)
	{
		T[i].Know = FALSE;
		T[i].Distance = Infinity;
		T[i].Path = Not_A_vertex;
		T[i].Index = i;
		T[i].Header = G->Linked_List[i];
	}

	T[Start].Distance = 0;
}
void Dijkstra(Table T, Graph G, Vertex Start)
{
	Vertex U, V;
	PriorityQueue Min_Heap;
	Position P;
	int i;
	Min_Heap = CreateHeap(G->Number_of_Vertices);
	P = G->Linked_List[Start]->next;
	T[Start].Know = TRUE;
	while (P != NULL)
	{
		T[P->Key].Distance = P->Weight;
		printf("Distance of T[%d].Distance=%d\n", P->Key, T[P->Key].Distance);
		T[P->Key].Path = Start;
		Insert(Min_Heap, T[P->Key]);
		P = P->next;
	}

	while (!IsEmpty(Min_Heap))
	{
		U = DeleteMin(Min_Heap, T);// ADD EACH VERTEX IN PRIORITY QUEUE

		printf("%2d\n", U);

		P = G->Linked_List[U]->next;

		T[U].Know = TRUE;

		while (P != NULL)
		{

			V = P->Key;
			printf("V=%2d\n", V);
			if (T[V].Know != TRUE)
			{
				printf("Distance of T[%d]:%d\n", U, T[U].Distance);
				printf("Distance of T[%d]:%d\n", V, T[V].Distance);
				if (T[U].Distance + P->Weight < T[V].Distance)
				{
					T[V].Distance = T[U].Distance + P->Weight;
					T[V].Path = U;
					printf("Distance of T[%d]:%d\n", V, T[V].Distance);
					Insert(Min_Heap, T[V]);
				}

				
			}
			P = P->next;
		}
	} printf("\n");


}

void Print_Path(Vertex V, Table T)
{
	if (T[V].Path != Not_A_vertex)
	{
		Print_Path(T[V].Path, T);
		printf(" to ");
	} printf("%d", V);
}

//----------------------------------------------------------------------------------------
//        IMPLEMENTATION OF Priority Queue(Min_Heap)
//----------------------------------------------------------------------------------------

PriorityQueue CreateHeap(int HeapSize)
{
	PriorityQueue Q;
	int i;
	Q = (PriorityQueue)malloc(sizeof(HeapStruct));
	if (Q == NULL)
	{
		printf("Fatal Error occurs!\n");
		return NULL;
	}

	else
	{
		Q->Capacity = HeapSize;
		Q->Size = 0;
		Q->Elements = (Table)malloc(sizeof(Table_Entry)*HeapSize);
		if (Q->Elements == NULL)
		{
			printf("Fatal Error occurs!\n");
			return NULL;
		}

	}
	Q->Elements[0].Distance = SENTINAL_NODE;
	return Q;
}
void Insert(PriorityQueue H, Table_Entry X)
{
	int i;
	if (IsFull(H))
	{
		printf("HEAP IS FULL!\n");
	}
	else
	{
		for (i = ++H->Size; X.Distance < H->Elements[i / 2].Distance; i = i / 2)
		{
			H->Elements[i] = H->Elements[i / 2];
		}
		H->Elements[i] = X;
	}
}
ElementType DeleteMin(PriorityQueue H, Table T)
{
	Table_Entry Min_Data;
	Table_Entry Last_data;
	int child;
	int i;
	if (IsEmpty(H))
	{
		printf("HEAP IS EMPTY!!\n");
	}
	else
	{
		Min_Data = H->Elements[1];
		Last_data = H->Elements[H->Size--];
		for (i = 1; 2 * (i) <= H->Size; i = child)
		{
			child = 2 * (i);
			if (H->Elements[child].Distance > H->Elements[child + 1].Distance && child != H->Size)
			{
				child++;
			}

			if (H->Elements[child].Distance < Last_data.Distance)
			{
				H->Elements[i] = H->Elements[child];
			}
			else
			{
				break;
			}
		}
	}

	H->Elements[i] = Last_data;
	return Min_Data.Index;

}
void PrintHeap(PriorityQueue H)
{

}
int IsFull(PriorityQueue H)
{
	return H->Size == H->Capacity;
}
int IsEmpty(PriorityQueue H)
{
	return H->Size == 0;
}
	
