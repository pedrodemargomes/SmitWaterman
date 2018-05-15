#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MATCH 3
#define MISS -3
#define PENALTY -2

#define MAX 20000
#define MAX_MATRIX MAX+1

char *seq1,*seq2;
long int *M;

char *respSeq1;
char *respSeq2;

int main() {
	

	long int i,j;
	long int maiorI,maiorJ,maior;
	maior = 0;

	long int numSeq1,numSeq2;
	scanf("%ld %ld",&numSeq1,&numSeq2);

	seq1 = malloc(numSeq1*sizeof(char));
	seq2 = malloc(numSeq2*sizeof(char));
	respSeq1 = malloc(numSeq1*sizeof(char));
	respSeq2 = malloc(numSeq2*sizeof(char));

	scanf("%s",seq1);
	scanf("%s",seq2);

	M = calloc( (numSeq1+1) * (numSeq2+1), sizeof(long int));

	/*numSeq1 = 9;
	numSeq2 = 8;
	char *seq1 = "GGTTGACTA";
	char *seq2 = "TGTTACGG";*/


	M[0] = 0;
	for(i=0;i<numSeq1+1;i++) {
		for(j=0;j<numSeq2+1;j++) {
			M[i*(numSeq2+1)+j] = 0;
		}
	}

	for(i=1;i<numSeq1+1;i++) {
		for(j=1;j<numSeq2+1;j++) {
			if(seq1[i-1] == seq2[j-1])
				M[i*(numSeq2+1)+j] = M[(i-1)*(numSeq2+1)+j-1] + MATCH;
			else
				M[i*(numSeq2+1)+j] = M[(i-1)*(numSeq2+1)+j-1] + MISS;
			if(M[i*(numSeq2+1)+j] < M[(i-1)*(numSeq2+1)+j] + PENALTY )
				M[i*(numSeq2+1)+j] = M[(i-1)*(numSeq2+1)+j] + PENALTY;
			if(M[i*(numSeq2+1)+j] < M[i*(numSeq2+1)+j-1] + PENALTY )
				M[i*(numSeq2+1)+j] = M[i*(numSeq2+1)+j-1] + PENALTY;
			if(M[i*(numSeq2+1)+j] < 0)
				M[i*(numSeq2+1)+j] = 0;
			if(maior < M[i*(numSeq2+1)+j]) {
				maior = M[i*(numSeq2+1)+j];
				maiorI=i;maiorJ=j;
			}
		}
	}

	#ifdef DEBUG
	for(i=0;i<numSeq1+1;i++) {
		for(j=0;j<numSeq2+1;j++) {
			printf("%ld ",M[i*(numSeq2+1)+j]);
		}
		printf("\n");
	}
	#endif

	#ifdef DEBUG
	printf("i: %ld j: %ld maior: %ld\n", maiorI,maiorJ,maior);
	#endif
	

	long int count = 0;
	i = maiorI; j = maiorJ;
	while(M[i*(numSeq2+1)+j] != 0) {
		if( M[(i-1)*(numSeq2+1)+j-1] >= M[(i-1)*(numSeq2+1)+j] && M[(i-1)*(numSeq2+1)+j-1] >= M[i*(numSeq2+1)+j-1] ) {
			respSeq1[count] = seq1[i-1];
			respSeq2[count] = seq2[j-1];
			i = i-1;j = j-1; // Diagonal
			#ifdef DEBUG
			printf("Diagonal\n");
			#endif
		} else if(M[(i-1)*(numSeq2+1)+j] >= M[(i-1)*(numSeq2+1)+j-1] && M[(i-1)*(numSeq2+1)+j] >= M[i*(numSeq2+1)+j-1] ) {
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



