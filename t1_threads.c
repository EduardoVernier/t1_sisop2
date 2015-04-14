#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int M, K, N;
int *A, *B, *C; 
int nThreads;

void *runner (void* arg);
void multiply (int row, int col);

int main (int argc, char *argv[]) 
{
	/* Checking parameter */
	if (argv[1] == NULL)
	{
		printf ("Usage ./a.out #treads\n");
		return 0;
	}
	nThreads = atoi(argv[1]);
	
	/* Opening files */
	FILE *in1, *in2, *out;	
	in1 = fopen("in1.txt", "r");
	in2 = fopen("in2.txt", "r");
	out = fopen("out.txt", "w");
	if (in1 == NULL || in2 == NULL || out == NULL)
		return 0;	
	

	/* Reading rows and columns number*/
	int buffer;
	fscanf(in1, "LINHAS = %d\n", &M);
	fscanf(in1, "COLUNAS = %d", &K);
	fscanf(in2, "LINHAS = %d\n", &buffer);
	fscanf(in2, "COLUNAS = %d", &N);	
	if (K != buffer)
	{
		printf ("Invalid Matrices\n");
		return 0;
	}
	
	/* Allocates matrices in heap */
	A = (int *) malloc (M * K * sizeof(int));
	B = (int *) malloc (K * N * sizeof(int));
	C = (int *) malloc (M * N * sizeof(int));
	
	/* Filling matrices */
	int r, c;
	for (r = 0; r < M; r++)
		for(c =0; c < K; c++)
			fscanf(in1, "%d", &A[r*K + c]);	
			
	for (r = 0; r < K; r++)
		for(c =0; c < N; c++)
			fscanf(in2, "%d", &B[r*N + c]);
	
	/* Starting threads */
	int t = 0;		
	pthread_t threads[nThreads];
	for (t = 0; t < nThreads; t++)
	{
		int *count = malloc(sizeof(int)); //value needs to be on the heap (not stack) to be accessible from within threads
		if (count == NULL)
			printf("Couldn't allocate memory for thread arg.\n");
		*count = t;
		pthread_create(&threads[*count], NULL, runner,(void*)count);
	}
	
	/* Waiting for all threads to finish */
	for (t = 0; t < nThreads; t++)
		pthread_join(threads[t],NULL);    
		
	/* Saving results */
	fprintf(out, "LINHAS = %d\nCOLUNAS = %d\n", M, N);
	for (r = 0; r < M; r++)
	{
		for(c =0; c < N; c++){
			fprintf(out, "%d", C[r*N + c]); 
			if (c < N -1) fprintf(out, " "); 
		}
		fprintf (out, "\n");
	}
	
	
	return 0;	
}


void *runner (void* arg)
{
	int t = *(int *) arg;
	int currentRow = 0;
	int i = 0, currentCol = 0;
	/* Chooses which rows to handle given an argument t */
	while (1)
	{
		currentRow = i * nThreads + t % nThreads;
		if (currentRow < M)
		{
			for (currentCol = 0; currentCol < N; currentCol++)
				multiply (currentRow, currentCol);
			i++;
		}
		else break;
	}	
	free(arg);		
}

void multiply (int row, int col)
{
	int i;
	C[row*N+col] = 0;
	for (i = 0; i < K; i++)
		C[row*N+col] += A[row*K+i] * B[i*N + col];
}





