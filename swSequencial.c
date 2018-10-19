#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MATCH 3
#define MISS -3
#define PENALTY -2

#define MAX 20000
#define MAX_MATRIX MAX+1

char *seq1,*seq2;
int *M;

char *respSeq1;
char *respSeq2;

int *direcI,*direcJ;

int main() {
	

	int i,j;
	int maiorI,maiorJ,maior;
	maior = 0;

	int numSeq1,numSeq2;
	scanf("%d %d",&numSeq1,&numSeq2);

	seq1 = malloc(  (numSeq1+1)* sizeof(char));
	seq2 = malloc(  (numSeq2+1)* sizeof(char));
	respSeq1 = malloc((numSeq1*2+1)*sizeof(char));
	respSeq2 = malloc((numSeq2*2+1)*sizeof(char));

	scanf("%s",seq1);
	scanf("%s",seq2);

	M = calloc( (numSeq1+1) * (numSeq2+1), sizeof(int));
	
	direcI = malloc( (numSeq1+1) * (numSeq2+1) * sizeof(int));
	direcJ = malloc( (numSeq1+1) * (numSeq2+1) * sizeof(int));

	/*numSeq1 = 9;
	numSeq2 = 8;
	char *seq1 = "GGTTGACTA";
	char *seq2 = "TGTTACGG";*/


	for(i=1;i<numSeq1+1;i++) {
		for(j=1;j<numSeq2+1;j++) {
			//printf("(%d,%d): %c %c\n",i,j,seq1[i-1], seq2[j-1] );
			if(seq1[i-1] == seq2[j-1]) {
				M[i*(numSeq2+1)+j] = M[(i-1)*(numSeq2+1)+j-1] + MATCH;
				direcI[i*(numSeq2+1)+j] = i-1;
				direcJ[i*(numSeq2+1)+j] = j-1;
			} else { 
				M[i*(numSeq2+1)+j] = M[(i-1)*(numSeq2+1)+j-1] + MISS;
				direcI[i*(numSeq2+1)+j] = i-1;
				direcJ[i*(numSeq2+1)+j] = j-1;
			}
			if(M[i*(numSeq2+1)+j] < M[(i-1)*(numSeq2+1)+j] + PENALTY ) {
				M[i*(numSeq2+1)+j] = M[(i-1)*(numSeq2+1)+j] + PENALTY;
				direcI[i*(numSeq2+1)+j] = i-1;
				direcJ[i*(numSeq2+1)+j] = j;
			}
			if(M[i*(numSeq2+1)+j] < M[i*(numSeq2+1)+j-1] + PENALTY ) {
				M[i*(numSeq2+1)+j] = M[i*(numSeq2+1)+j-1] + PENALTY;
				direcI[i*(numSeq2+1)+j] = i;
				direcJ[i*(numSeq2+1)+j] = j-1;
			}
			if(M[i*(numSeq2+1)+j] < 0)
				M[i*(numSeq2+1)+j] = 0;
			if(maior < M[i*(numSeq2+1)+j]) {
				maior = M[i*(numSeq2+1)+j];
				maiorI=i;maiorJ=j;
			}
		}
	}

	// for(i=0;i<numSeq1+1;i++) {
	// 	for(j=0;j<numSeq2+1;j++) {
	// 		printf("%u ",M[i*(numSeq2+1)+j]);
	// 	}
	// 	printf("\n");
	// }

	//#ifdef DEBUG
	//printf("i: %d j: %d maior: %d\n", maiorI,maiorJ,maior);
	//#endif
	
	int count = 0;
	i = maiorI; j = maiorJ;
	while(M[i*(numSeq2+1)+j] != 0) {
		if( direcI[i*(numSeq2+1)+j] == i-1 && direcJ[i*(numSeq2+1)+j] == j-1 ) {
			respSeq1[count] = seq1[i-1];
			respSeq2[count] = seq2[j-1];
			i = i-1;j = j-1; // Diagonal
			#ifdef DEBUG
			printf("Diagonal\n");
			#endif
		} else if( direcI[i*(numSeq2+1)+j] == i-1 && direcJ[i*(numSeq2+1)+j] == j ) {
			respSeq1[count] = seq1[i-1];
			respSeq2[count] = '-';
			i = i-1; // Cima
			#ifdef DEBUG
			printf("Cima\n");
			#endif
		} else {
			respSeq1[count] = '-';
			respSeq2[count] = seq2[j-1];
			j = j-1; // Esquerda
			#ifdef DEBUG
			printf("Esquerda\n");
			#endif
		}
		count++;
	}

	// for(i=0;i<numSeq2+1;i++) {
	// 	for(j= 0;j < numSeq1+1;j++) {
	// 		printf("(%d %d)",direcI[i*(numSeq2+1)+j],direcJ[i*(numSeq2+1)+j]);
	// 	}
	// 	printf("\n");
	// }


	printf("count = %d\n", count );
	//#ifdef DEBUGMIN
	for(i=0;i<count;i++)
		printf("%c",respSeq1[i]);
	printf("\n");
	for(i=0;i<count;i++)
		printf("%c",respSeq2[i]);
	printf("\n");
	//#endif
	return 0;
}



