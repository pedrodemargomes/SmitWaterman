#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>
#include <time.h>

#define MATCH 3
#define MISS -3
#define PENALTY -2

#define MAX 20000
#define MAX_MATRIX MAX+1

struct Maior { 
	int val; 
	int i;
	int j;
};    
#pragma omp declare reduction(maximum : struct Maior : omp_out = omp_in.val > omp_out.val ? omp_in : omp_out)

int numSeq1,numSeq2;
char *seq1,*seq2;
int *M; //[MAX_MATRIX][MAX_MATRIX];

int *direcI,*direcJ;

int numProcs;
struct Maior maior;

char *respSeq1;
char *respSeq2;

double timestamp(void){
	struct timeval tp;
	gettimeofday(&tp, NULL);
	return((double)(tp.tv_sec*1000.0 + tp.tv_usec/1000.0));
}


inline int max(int a, int b) {
	if(a > b)
		return a;
	return b;
}

inline int min(int a, int b) {
	if(a > b)
		return b;
	return a;
}

inline int numElementosDiagonal(int i, int numSeq1, int numSeq2) {
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

inline void score(int bi, int bj){
	if(seq1[bi-1] == seq2[bj-1]) {
		M[bi*(numSeq2+1)+bj] = M[(bi-1)*(numSeq2+1)+bj-1] + MATCH;
		//direcI[bi*(numSeq2+1)+bj] = bi-1;
		//direcJ[bi*(numSeq2+1)+bj] = bj-1;
	}
	else {
		M[bi*(numSeq2+1) + bj] = M[(bi-1)*(numSeq2+1) + bj-1] + MISS;
		//direcI[bi*(numSeq2+1)+bj] = bi-1;
		//direcJ[bi*(numSeq2+1)+bj] = bj-1;
	}
	if(M[bi*(numSeq2+1)+ bj] < M[(bi-1)*(numSeq2+1) + bj] + PENALTY ) {
		M[bi*(numSeq2+1)+ bj] = M[(bi-1)*(numSeq2+1) + bj] + PENALTY;
		//direcI[bi*(numSeq2+1)+bj] = bi-1;
		//direcJ[bi*(numSeq2+1)+bj] = bj;
	}
	if(M[bi*(numSeq2+1)+ bj] < M[bi*(numSeq2+1)+ bj-1] + PENALTY ) {
		M[bi*(numSeq2+1)+ bj] = M[bi*(numSeq2+1)+ bj-1] + PENALTY;
		//direcI[bi*(numSeq2+1)+bj] = bi;
		//direcJ[bi*(numSeq2+1)+bj] = bj-1;
	}
	if(M[bi*(numSeq2+1)+ bj] < 0)
		M[bi*(numSeq2+1)+ bj] = 0;

	if(maior.val < M[bi*(numSeq2+1)+bj]) {
		maior.val = M[bi*(numSeq2+1)+bj];
		maior.i=bi;maior.j=bj;
	}
}

int main() {
	numProcs= 100;//omp_get_num_procs();

	int i,j;
	double ti,tf,t;
	t = 0;

	maior.val = 0;

	scanf("%d %d",&numSeq1,&numSeq2);
	
	seq1 = malloc(numSeq1*sizeof(char));
	seq2 = malloc(numSeq2*sizeof(char));
	respSeq1 = malloc(numSeq1*sizeof(char));
	respSeq2 = malloc(numSeq2*sizeof(char));

	scanf("%s",seq1);
	scanf("%s",seq2);

	M = calloc( (numSeq1+1) * (numSeq2+1), sizeof(int));

	//direcI = malloc( (numSeq1+1) * (numSeq2+1) * sizeof(int));
	//direcJ = malloc( (numSeq1+1) * (numSeq2+1) * sizeof(int));


	int s_block = ceil((double)numSeq2/numProcs);
	int bi,bj; // i e d do bloco

	int pi,pj; // Primeiros i e j da diagonal
	int ki,kj;
	int numDiagonais = numSeq1/s_block+numSeq2/s_block-1; // Numero de diagonais a percorrer
	int numElementos; // Numero de elementos na diagonal
	
	for(i =1; i <= numDiagonais; i++) {
		numElementos = numElementosDiagonal(i,numSeq1/s_block,numSeq2/s_block);
		calcPrimElemDiagonal(i,&pi,&pj,numSeq1/s_block);
		
		#pragma omp parallel for private(ki,kj,bi,bj) reduction(maximum:maior) //num_threads(8)
		//ti = timestamp();
		for(j = 1;j <= numElementos; j++) {
			
			ki = pi - j +1;
			kj = pj + j -1;

			//printf("ki = %d kj = %d\n",(ki-1)*s_block+1,(kj-1)*s_block+1 );
			// BLOCO
			for(bi=(ki-1)*s_block+1; bi < (ki-1)*s_block+1+s_block; bi++) {

				for(bj=(kj-1)*s_block+1;bj < (kj-1)*s_block+1+s_block; bj++) {
					//printf("bi = %d bj = %d\n",bi,bj);
					score(bi,bj);
				} 
				if((numSeq2-numSeq2%s_block<=bj) ) {
					for(;bj<numSeq2+1;bj++) {
						//printf("bi = %d bj = %d\n",bi,bj);
						score(bi,bj);	
					}
					//printf("bi = %d bj = %d\n",bi,bj);
				}
			}
			if((numSeq1-numSeq1%s_block<=bi) ) {
				for(;bi<numSeq1+1;bi++) {
					for(bj=(kj-1)*s_block+1;bj < (kj-1)*s_block+1+s_block; bj++) {
						//printf("bi = %d bj = %d\n",bi,bj);
						score(bi,bj);
					} 
					if((numSeq2-numSeq2%s_block<=bj) ) {
						for(;bj<numSeq2+1;bj++) {
							//printf("bi = %d bj = %d\n",bi,bj);
							score(bi,bj);	
						}
						//printf("bi = %d bj = %d\n",bi,bj);
					}
				}
			}
			// ++++++++++++==
				
		
		}
		//tf = timestamp();
		//t += tf-ti;
		
	}
	//printf("TempoParalelo: %lf\n",t);

	#ifdef DEBUG
	for(i=0;i<numSeq1+1;i++) {
		for(j=0;j<numSeq2+1;j++) {
			printf("%d ",M[i*(numSeq2+1)+j]);
		}
		printf("\n");
	}
	#endif

	#ifdef DEBUG
	printf("i: %d j: %d maior: %d\n", maior.i,maior.j,maior.val);
	#endif
	

	/*int count = 0;
	i = maior.i; j = maior.j;
	while(M[i*(numSeq2+1)+j] != 0) {
		if( direcI[i*(numSeq2+1)+j] == i-1 && direcJ[i*(numSeq2+1)+j] == j-1 ) {
			respSeq1[count] = seq1[i-1];
			respSeq2[count] = seq2[j-1];
			i = i-1;j = j-1; // Diagonal
			#ifdef DEBUG
			printf("Diagonal\n");
			#endif
		} else if(direcI[i*(numSeq2+1)+j] == i-1 && direcJ[i*(numSeq2+1)+j] == j  ) {
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
	}*/

	/*#ifdef DEBUGMIN
	for(i=0;i<count;i++)
		printf("%c",respSeq1[i]);
	printf("\n");
	for(i=0;i<count;i++)
		printf("%c",respSeq2[i]);
	printf("\n");
	#endif*/
	return 0;
}


