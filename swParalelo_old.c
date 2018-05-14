#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define MATCH 3
#define MISS -3
#define PENALTY -2

#define MAX 20000
#define MAX_MATRIX MAX+1

char seq1[MAX],seq2[MAX];
int M[MAX_MATRIX][MAX_MATRIX];
int numProcs=1;

char respSeq1[MAX];
char respSeq2[MAX];


inline int max(int a,int b) {
	if(a > b)
		return a;
	return b;
}

inline int min(int a,int b) {
	if(a > b)
		return b;
	return a;
}

inline int numElementosDiagonal(int i,int numSeq1, int numSeq2) {
	if( i < numSeq1+1 && i < numSeq2+1 )
		return i;
	else if(i < max(numSeq1+1,numSeq2+1) )
		return min(numSeq1,numSeq2);
	else  
		return 2*min(numSeq1+1,numSeq2+1) - i + abs(numSeq1+1 -(numSeq2+1) ) - 2;
}

inline void calcPrimElemDiagonal(int i,int *pi,int *pj,int numSeq1) {
	if (i < numSeq1+1) {
		*pi = i;
 		*pj = 1;
	} else {
		*pi = (numSeq1+1) - 1;
		*pj = i -(numSeq1+1) + 2;
	}
}

int main() {
	numProcs= omp_get_num_procs();

	int i,j;
	int maiorI,maiorJ,maior;
	maior = 0;

	int numSeq1,numSeq2;
	scanf("%d %d",&numSeq1,&numSeq2);
	scanf("%s",seq1);
	scanf("%s",seq2);

	M[0][0] = 0;
	for(i=0;i<numSeq1+1;i++) {
		for(j=0;j<numSeq2+1;j++) {
			M[i][j] = 0;
		}
	}


	int s_block = 100;//max(numSeq1+1,numSeq2+1)/numProcs;
	int bi,bj; // i e d do bloco

	int pi,pj; // Primeiros i e j da diagonal
	int ki,kj;
	int numDiagonais = numSeq1/s_block+numSeq2/s_block-1; // Numero de diagonais a percorrer
	int numElementos; // Numero de elementos na diagonal

	for(i =1; i <= numDiagonais; i++) {
		numElementos = numElementosDiagonal(i,numSeq1/s_block,numSeq2/s_block);
		calcPrimElemDiagonal(i,&pi,&pj,numSeq1/s_block);
		
		#pragma omp parallel for private(ki,kj,bi,bj)
		for(j = 1;j <= numElementos; j++) {
			
			ki = pi - j +1;
			kj = pj + j -1;

			//printf("ki = %d kj = %d\n",(ki-1)*s_block+1,(kj-1)*s_block+1 );
			// BLOCO
			for(bi=(ki-1)*s_block+1; bi < (ki-1)*s_block+1+s_block; bi++) {
				for(bj=(kj-1)*s_block+1;bj < (kj-1)*s_block+1+s_block; bj++) {
					//printf("bi = %d bj = %d\n",bi,bj);
					if(seq1[bi-1] == seq2[bj-1])
						M[bi][bj] = M[bi-1][bj-1] + MATCH;
					else
						M[bi][bj] = M[bi-1][bj-1] + MISS;
					if(M[bi][bj] < M[bi-1][bj] + PENALTY )
						M[bi][bj] = M[bi-1][bj] + PENALTY;
					if(M[bi][bj] < M[bi][bj-1] + PENALTY )
						M[bi][bj] = M[bi][bj-1] + PENALTY;
					if(M[bi][bj] < 0)
						M[bi][bj] = 0;
					if(maior < M[bi][bj]) {
						#pragma omp critical
						{
						maior = M[bi][bj];
						maiorI=bi;maiorJ=bj;
						}
					}
				}
			}
			// ++++++++++++==
				
		
		}
		
	}

	#ifdef DEBUG
	for(i=0;i<numSeq1+1;i++) {
		for(j=0;j<numSeq2+1;j++) {
			printf("%d ",M[i][j]);
		}
		printf("\n");
	}
	#endif

	#ifdef DEBUG
	printf("i: %d j: %d maior: %d\n", maiorI,maiorJ,maior);
	#endif
	

	int count = 0;
	i = maiorI; j = maiorJ;
	while(M[i][j] != 0) {
		if( M[i-1][j-1] >= M[i-1][j] && M[i-1][j-1] >= M[i][j-1] ) {
			respSeq1[count] = seq1[i-1];
			respSeq2[count] = seq2[j-1];
			i = i-1;j = j-1; // Diagonal
			#ifdef DEBUG
			printf("Diagonal\n");
			#endif
		} else if(M[i-1][j] >= M[i-1][j-1] && M[i-1][j] >= M[i][j-1] ) {
			respSeq1[count] = '-';
			respSeq2[count] = seq1[i-1];
			i = i-1; // Cima
			#ifdef DEBUG
			printf("Cima\n");
			#endif
		} else {
			respSeq1[count] = seq2[j-1];
			respSeq2[count] = '-';
			j = j-1; // Esquerda
			#ifdef DEBUG
			printf("Esquerda\n");
			#endif
		}
		count++;
	}

	#ifdef DEBUGMIN
	for(i=0;i<count;i++)
		printf("%c",respSeq1[i]);
	printf("\n");
	for(i=0;i<count;i++)
		printf("%c",respSeq2[i]);
	printf("\n");
	#endif
	return 0;
}



