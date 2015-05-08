/*
DataMining Assignment #1
Finding All the Association Rules Using Apriori Algorithm
2015.03.26
John Park

----Data Structure---------
typedef struct Transaction
{
int* item;
int support;
int check;
}Transaction;
---------------------------

------input form--------------
[item]\t[itme]\t[itme]\n
[item]\t[itme]\n
[item]\t[itme]\[item]\t[itme]\n
-------------------------------
*/

#define _CRT_SECURE_NO_DEPRECATE
#define TRUE 1
#define FALSE 0
#define MAXCANDIDATES 100
#define NMAX 100
#include <stdio.h>
#include <stdlib.h>

typedef struct Transaction{
	int* item;
	int support;
	int check;
}Transaction;

typedef Transaction Candidate;
typedef Transaction Large;
typedef Transaction Frequent;

#define BUF_SIZE 50
int compare(void *, void *);
int compare2(void *, void *);
Candidate* self_join(Transaction *);
Candidate* pruning(Transaction*, Transaction*);
Candidate* cal_sup(Transaction*, Candidate*, double MIN_SUPPORT);
int is_Subset(Transaction, Candidate);
int is_NULL(Transaction*);
void Find_AssociationRule(Frequent*, FILE*);
void SWAP(int* des, int* src){
	int tmp = *des;
	*des = *src;
	*src = tmp;
}



int is_a_solution(int a[], int k, int n, Frequent* F, FILE* out, int f_n)
{
	return (k == n);
}

void construct_candidates(int a[], int k, int n, int c[], int* ncandidates, Frequent* F, FILE* out, int f_n)
{
	c[0] = TRUE;
	c[1] = FALSE;
	*ncandidates = 2;
}
void process_solution(int a[], int k, Frequent* F, FILE* out, int f_n,Transaction *TID)
{
	int i;
	int *set1, *set2;
	int count1=0, count2=0;

	for (i = 1; i <= k; i++)
	{
		if (a[i] == TRUE)
		{
			count1++;
		}
		else
		{
			count2++;
		}
			
	}
	set1 = (int*)malloc(sizeof(int)*(count1+1));
	set2 = (int*)malloc(sizeof(int)*(count2+1));
	int c_1 = 1;
	int c_2 = 1;
	for (i = 1; i <= k; i++)
	{
		if (a[i] == TRUE)
		{
			set1[c_1] = F[f_n].item[i-1];
			c_1++;
		}
		else
		{
			
			set2[c_2] = F[f_n].item[i-1];
			c_2++;
		}
	}
	if (count1 == 0 || count2 == 0)
	{

	}
	else
	{
		Find_AssociationRule(F, out, TID, set1, set2, count1, count2, f_n);
	}
	
	
}
void backtrack(int a[], int k, int input, Frequent* F, FILE* out, int f_n, Transaction *TID)
{
	int c[MAXCANDIDATES] = { 0, }; // 다음 위치가 될 수 있는 후보 위치
	int ncandidates = 0;   // 다음 위치가 될 수 있는 후보 개수
	int i = 0;             // 카운터

	if (is_a_solution(a, k, input, F,out, f_n))
	{
		process_solution(a, k, F, out, f_n, TID);
	}
	else
	{
		k++;
		construct_candidates(a, k, input, c, &ncandidates, F, out, f_n);
		for (i = 0; i<ncandidates; i++)
		{
			a[k] = c[i];
			backtrack(a, k, input, F, out, f_n, TID);
			if (0) return;  // 일찍 종료함
		}
	}
}

void generate_subsets(int n, Frequent* F, FILE* out, int f_n, Transaction *TID)
{
	int a[NMAX];
	backtrack(a, 0, n, F, out, f_n,TID);
}

int main(int argc, char* argv[])
{
	errno_t err;
	FILE *in, *out;

	int Transaction_count = 0;
	char *item;

	int count, Lcount;

	double MIN_SUPPORT = atof(argv[1]) / 100;
	char * InputFile = argv[2];
	char * OutputFile = argv[3];

	char buf[BUF_SIZE];
	int i, j, c, Fcount = 0;
	int turn = 2;
	int sum = 0;

	Transaction *TID = NULL;
	Candidate	*CID = (Candidate *)malloc(sizeof(Candidate));
	Candidate	*C2ID = (Candidate *)malloc(sizeof(Candidate));
	Large		*LID = (Large *)malloc(sizeof(Large));
	Large		*L2ID = (Large *)malloc(sizeof(Large));
	Frequent	*F = (Frequent*)malloc(sizeof(Frequent));
	int**		full_set = (int**)malloc(sizeof(int));
	int			*full_count = (int*)malloc(sizeof(int));

	//FILE OPEN TEST
	if ((err = fopen_s(&in, InputFile, "rt")) != 0)
	{
		printf("FILE OPEN ERROR");
		return -1;
	}
	if ((err = fopen_s(&out, OutputFile, "wt")) != 0)
	{
		printf("FILE OPEN ERROR");
		return -1;
	}
	F[0].check = 0;

	//Count the number of transactions
	while (!feof(in))
	{
		fgets(buf, sizeof(buf), in);
		Transaction_count++;
	}
	TID = (Transaction*)malloc(sizeof(Transaction)*(Transaction_count + 1));// allocate the transactions
	for (i = 0; i <= Transaction_count; i++)//initiallization
	{
		TID[i].item = NULL;
		TID[i].support = 0;
	}
	fseek(in, 0, SEEK_SET); //move the file pointer to the start point

	//Scan the data and Store all the item sets
	//TID.support means the number of items in TID[i]
	i = 1;
	while (fgets(buf, sizeof(buf), in) != NULL)
	{
		j = 0;
		//parsing
		item = strtok(buf, "\t");
		if (item[strlen(item) - 1] == '\n')
		{
			item[strlen(item) - 1] = '\0';
			TID[i].item = (int *)malloc(sizeof(int));
			TID[i].item[0] = atoi(item);
			TID[i].support++;


			i++;
			continue;
		}
		TID[i].item = (int *)malloc(sizeof(int));
		TID[i].item[0] = atoi(item);
		TID[i].support++;
		j++;

		while (item != NULL)
		{
			item = strtok(NULL, "\t");
			if (item == NULL)	break;
			if (item[strlen(item) - 1] == '\n')
			{
				item[strlen(item) - 1] = '\0';
				TID[i].item = (int *)realloc(TID[i].item, sizeof(int)*(j + 1));// 추가로 4바이트를 더 할당 (realloc 추가가 아니라 최종배열길이)
				TID[i].item[j] = atoi(item);
				TID[i].support++;
				//////////////////////////////////////////////////PRINT TEST//////////////////////////////////
				/*
				for (c = 0; c < TID[i].support; c++)
				{
				printf("%d-->", TID[i].item[c]);
				}
				printf("\n");
				*/
				break;
			}
			TID[i].item = (int *)realloc(TID[i].item, sizeof(int)*(j + 1));	//4바이트를 추가로 할당
			TID[i].item[j] = atoi(item);
			TID[i].support++;
			j++;
		}
		i++;
	}
	TID[0].check = Transaction_count;
	printf("Transaction count is %d", Transaction_count);
	for (i = 0; i < TID[0].check; i++)
	{
		qsort(TID[i].item, TID[i].support, sizeof(TID[0].item), compare2);
	}
	

	//Generate C1 and calculate the support
	c = 1;
	count = 0;
	for (i = 1; i <= Transaction_count; i++)
	{
		for (j = 0; j < TID[i].support; j++)
		{
			//먼저 CID[c]에 넣고자 하는 값을 찾는다.
			for (c = 1; c <= count; c++)
			{
				if (CID[c].item[0] == TID[i].item[j]) //찾으면 support를 증가시켜줌
				{
					CID[c].support++;
					break;
				}
			}
			//다 찾아봤는데도 없다? 그러면 하나 만들자
			if (c == (count + 1))
			{
				count++;
				CID = (Candidate *)realloc(CID, sizeof(Candidate)*(count + 1));
				CID[count].support = 1;
				CID[count].item = (int*)malloc(sizeof(int)); //int 한개만 할당해주면됨
				CID[count].item[0] = TID[i].item[j];
				CID[count].check = 1;
			}
		}
	}
	//Prunning C1 and Make L1
	//count는 CID의 개수
	Lcount = 0;
	for (i = 1; i <= count; i++)
	{
		if (((double)CID[i].support / Transaction_count) < MIN_SUPPORT)
			continue;
		else
		{
			Lcount++;
			if (Lcount == 1)
			{
				LID = (Large *)realloc(LID, sizeof(Large)*(Lcount + 1));
				LID[Lcount].support = CID[i].support;
				LID[Lcount].item = (int*)malloc(sizeof(int)); //int 한개만 할당해주면됨
				LID[Lcount].item[0] = CID[i].item[0];
				LID[Lcount].check = 1;
			}
			else
			{
				LID = (Large *)realloc(LID, sizeof(Large)*(Lcount + 1)); //
				LID[Lcount].support = CID[i].support;
				LID[Lcount].item = (int*)malloc(sizeof(int)); //int 한개만 할당해주면됨
				LID[Lcount].item[0] = CID[i].item[0];
				LID[Lcount].check = 1;
			}

		}
	}

	qsort(LID + 1, Lcount, sizeof(Transaction), compare);
	//LID TEST
	for (c = 1; c <= Lcount; c++)
	{
		//printf("%d-->", LID[c].item[0]);
	}
	//printf("\n");

	LID[0].check = Lcount;
	//----------------------L1까지 끝

	//Lk-1 --> Ck --> Lk만듬
	while (1)
	{
		//비었냐 안비었냐 LID가 체크해주자.
		int i, j;
		if (turn == 2) //C2ID에다가 만듦(처음에 CID에 만들어져있으니까), 번갈아가면서 사용
		{
			int a, b;
			C2ID = self_join(LID); //LID->check means the number of LID transactions



			C2ID = pruning(LID, C2ID); //pruning by checking previous CID


			L2ID = cal_sup(TID, C2ID, MIN_SUPPORT); //pruning by MIN_support, make L(k+1)
			turn = 1;
		
			if (is_NULL(L2ID) == FALSE)
			{
				
				break;
			}
			Fcount = F[0].check;
			F[0].check = F[0].check + L2ID[0].check;
			F = (Frequent*)realloc(F, sizeof(Frequent)*(F[0].check + 1));
			for (i = 1; i <= L2ID[0].check; i++)
			{
				Fcount++;
				F[Fcount].check = L2ID[i].check;
				F[Fcount].support = L2ID[i].support;
				F[Fcount].item = (int*)malloc(sizeof(int)*L2ID[i].check);
				for (j = 0; j < L2ID[i].check; j++)
				{
					F[Fcount].item[j] = L2ID[i].item[j];
				}

			}
		}
		else if (turn == 1)
		{
			int a, b;
			CID = self_join(L2ID);
			CID = pruning(L2ID, CID); //pruning by checking previous CID
			LID = cal_sup(TID, CID, MIN_SUPPORT);// pruning by MIN_support, make L(k + 1)
			turn = 2;
			
			if (is_NULL(LID) == FALSE)
			{
				
				break;
			}

			Fcount = F[0].check;
			F[0].check = F[0].check + LID[0].check;
			F = (Frequent*)realloc(F, sizeof(Frequent)*(F[0].check + 1));
			for (i = 1; i <= LID[0].check; i++)
			{
				Fcount++;
				F[Fcount].check = LID[i].check;
				F[Fcount].support = LID[i].support;
				F[Fcount].item = (int*)malloc(sizeof(int)*LID[i].check);
				for (j = 0; j < LID[i].check; j++)
				{
					F[Fcount].item[j] = LID[i].item[j];
				}

			}
		}
	}



	//이제 F만 사용하면됨 F
	//

	for (i = 1; i <= Fcount; i++)
	{
		
		generate_subsets(F[i].check,F,out,i,TID);
	}



	fclose(in);
	fclose(out);
	return 0;
}

void Find_AssociationRule(Frequent* F, FILE* out, Transaction* TID, int *set1, int *set2,int count1, int count2,int f_n)
{
	int Fcount = F[0].check;
	int i, j, k;
	int support = F[f_n].support;
	int sup_count = 0;

	//op_set에 들어있는 모든것에 대해서 association rule을 구한다.
	// {set1}\t{set2}\t{support}\t{confidence}
	//div까지/ div 다음부터~ op_set[i][0](op_Set의 개수) 까지로 두부분으로 나눈다.
	//set1, set2에 저장. 이제 support, confidence만 구하면됨.
	//먼저 op_set을 돌면서 F 에서 찾는다(이즈 서브셋이용? --> ㄴㄴ) --> support를 구하려고 전체에 대한 support를 구하려고. P{A U B} 이거구하려고.
	//B --> A P(AlB) = P(A U B)/ P(B) 이거임.  P(A), P(B)는 각자 계산해야됨. set1을 가지고 전체를 다셈.이즈서브셋으로 세도됨.(개수를 반환하게하자)
	// {set1}\t{set2}t{support}\t{confidence}
	

		for (k = 1; k <= TID[0].check; k++)
		{
			if (int_Subset(TID[k].item, set1+1, TID[k].support, count1))
				sup_count++;
			//P(A) = sup_count / TID[0].support
			//P(AUB) = support
		}
		fprintf(out, "{");
		
		for (i = 1; i <= count1; i++)
		{
			if (i == count1)
			{
				fprintf(out, "%d", set1[i]);
			}
			else{
				fprintf(out, "%d,", set1[i]);
			}
		}
		fprintf(out, "}\t{");
		for (i = 1; i <= count2; i++)
		{
			if (i == count2)
			{
				fprintf(out, "%d", set2[i]);
			}
			else{
				fprintf(out, "%d,", set2[i]);
			}
		}
		fprintf(out, "}");
		double psame = ((double)support / TID[0].check) * 100; //P(AUB)    support 교집합의 개수
		double pone = ((double)sup_count / TID[0].check) * 100; //P(A) A-->B	sup_count A의 개수
		double conf = (psame / pone) * 100;
		fprintf(out, "\t%.2f\t%.2f\n", psame, conf);
		//printf("}\t%.2f\t%.2f", psame, conf);
		support = 0;
		sup_count = 0;

	


}
int int_Subset(int* TID, int* CID, int TIDC, int CIDC)
{
	int i = 0, j;
	int check = 1;

	for (i = 0; i < CIDC; i++)
	{
		for (j = 0; j < TIDC; j++)
		{
			if (TID[j] != CID[i])
			{
				check = 0;

			}
			else if (TID[j] == CID[i])
			{
				check = 1;
				break;
			}
		}
		if (j == TIDC)
		{
			break;
		}
	}
	if (check == 0)
	{
		return FALSE;
	}
	else{
		return TRUE;
	}

}

int is_NULL(Large *LID)
{
	if (LID[0].check == 0)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}
int is_Subset(Transaction TID, Candidate CID)
{
	int i = 0, j;
	int check = 1;
	int Ck = CID.check;
	int Tk = TID.support;

	for (i = 0; i < Ck; i++)
	{
		for (j = 0; j < Tk; j++)
		{
			if (TID.item[j] != CID.item[i])
			{
				check = 0;

			}
			else if (TID.item[j] == CID.item[i])
			{
				check = 1;
				break;
			}
		}
		if (j == Tk)
		{
			break;
		}
	}
	if (check == 0)
	{
		return FALSE;
	}
	else{
		return TRUE;
	}

}
Candidate* cal_sup(Transaction* TID, Candidate* CID, double MIN_SUPPORT)
{
	int i, j, c;
	Large *RLID = (Large*)malloc(sizeof(Large));
	int count = 0;
	int Ck = CID[0].check, Tk = TID[0].check;
	for (i = 1; i <= Ck; i++)
	{
		for (j = 1; j <= Tk; j++)
		if (is_Subset(TID[j], CID[i]) == TRUE)
		{
			CID[i].support++;
		}
	}
	for (i = 1; i <= Ck; i++)
	{
		if ((double)CID[i].support / Tk >= MIN_SUPPORT)
		{
			count++;
			RLID = (Large*)realloc(RLID, sizeof(Large)*(count + 1));
			RLID[count].item = (Large*)malloc((sizeof(Large*)*Ck));
			//복사중
			for (c = 0; c < CID[1].check; c++)
			{
				RLID[count].item[c] = CID[i].item[c];
				RLID[count].check = CID[1].check;
				RLID[count].support = CID[i].support;
			}
		}
	}
	RLID[0].check = count;
	return RLID;
}


Candidate* self_join(Large* LID)
{
	int i, j, c, k, count = 0;
	Candidate * CID = (Candidate*)malloc(sizeof(Candidate));
	k = LID[1].check + 1;

	for (i = 1; i < LID[0].check; i++)
	{
		for (j = i + 1; j <= LID[0].check; j++)
		{
			for (c = 0; c < (k - 2); c++)
			{
				if (LID[i].item[c] != LID[j].item[c])
					break;

			}
			if (c == (k - 2))
			{
				count++;
				CID = (Candidate*)realloc(CID, sizeof(Candidate)*(count + 1));
				CID[count].item = (Candidate*)malloc(sizeof(Candidate*)*(k));
				//공통것 집어넣는과정
				for (c = 0; c < (k - 2); c++)
				{
					CID[count].item[c] = LID[i].item[c];
					CID[count].support = 0;
					CID[count].check = k;
				}
				//다른것 집어넣는과정
				CID[count].item[c] = LID[i].item[k - 2];
				CID[count].item[c + 1] = LID[j].item[k - 2];
				CID[count].support = 0;
				CID[count].check = k;

			}
		}
	}
	CID[0].check = count;
	return CID;

}


//pruning by checking previous CID
Transaction* pruning(Transaction* LID, Transaction* CID)
{
	int Lk = LID[1].check, Ck = CID[1].check;
	int Lcount = LID[0].check, Ccount = CID[0].check;
	int i, j, c, count = 0;
	Candidate* RCID;
	RCID = (Candidate*)malloc(sizeof(Candidate));
	for (i = 1; i <= Ccount; i++)
	{
		for (j = 1; j <= Lcount; j++)
		{
			for (c = Ck - Lk; c < Ck; c++)
			{
				if (LID[j].item[c - 1] != CID[i].item[c])
					break;
			}
			if (c == Ck)
			{
				count++;
				RCID = (Candidate*)realloc(RCID, sizeof(Candidate)*(count + 1));
				RCID[count].item = (Candidate*)malloc((sizeof(Candidate*)*Ck));
				//공통것 집어넣는과정
				for (c = 0; c < Ck; c++)
				{
					RCID[count].item[c] = CID[i].item[c];
					RCID[count].check = Ck;
					RCID[count].support = 0;
				}
			}
		}
	}
	RCID[0].check = count;
	free(CID);
	return RCID;

}

int compare(void *first, void *second)
{
	if (((Transaction*)first)->item[0] > ((Transaction*)second)->item[0])
		return 1;
	else if (((Transaction*)first)->item[0] < ((Transaction*)second)->item[0])
		return -1;
	else
		return 0;
}

int compare2(void *first, void *second)
{
	if (*((int*)first) > *((int*)second))
		return 1;
	else if (*((int*)first) < *((int*)second))
		return -1;
	else
		return 0;
}
