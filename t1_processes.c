/* http://stackoverflow.com/questions/15328285/how-to-fork-an-exact-number-of-children
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> /* for pid_t */
#include <sys/wait.h> /* for wait */
#include <sys/shm.h> /* shared memory related */
#include <sys/stat.h> /* shared memory related */

int M, K, N;
int *A, *B; 
int nProcesses;

int segmentId;
int *C;

void runner (int id);
inline void multiply (int row, int col);

int main(int argc, char *argv[])
{
	/* Checking parameter */
	if (argv[1] == NULL)
	{
		printf ("usage ./a.out nProcesses\n");
		return 0;
	}
	nProcesses = atoi(argv[1]);
	
	/* Opening files */
	FILE *in1, *in2, *out;	
	in1 = fopen("in1.txt", "r");
	in2 = fopen("in2.txt", "r");
	out = fopen("out.txt", "w");
	if (in1 == NULL || in2 == NULL || out == NULL)
	{	
		printf ("Couldn't open files.\n");
		return 0;	
	}	

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
	
	/* Allocates matrices A and B in heap. C needs to be shared memory */
	A = (int *) malloc (M * K * sizeof(int));
	B = (int *) malloc (K * N * sizeof(int));
	
	/* Allocates shared memory segment */
	const int size = N * M * sizeof(int);	
	segmentId = shmget(IPC_PRIVATE, size, S_IRUSR | S_IWUSR);
	
	/* Filling matrices */
	int r, c;
	for (r = 0; r < M; r++)
		for(c =0; c < K; c++)
			fscanf(in1, "%d", &A[r*K + c]);	
			
	for (r = 0; r < K; r++)
		for(c =0; c < N; c++)
			fscanf(in2, "%d", &B[r*N + c]);
	
	/* Forking processes */		
	int i;
	for (i = 0; i < nProcesses; i++)
	{
		if (fork() == 0) // only child processes
		{	 
			runner(i);
			exit(0);
		}
	}
	
	/* Wait all child processes */
	int status;
	for (i = 0; i < nProcesses; ++i)
		wait(&status);
	

	/* Saving results */
	C = (int *) shmat(segmentId, NULL, 0);
	fprintf(out, "LINHAS = %d\nCOLUNAS = %d\n", M, N);
	for (r = 0; r < M; r++)
	{
		for(c =0; c < N; c++){
			fprintf(out, "%d", C[r*N + c]); 
			if (c < N -1) fprintf(out, " "); 
		}
		fprintf (out, "\n");
	}	
	shmdt(C);	
	shmctl(segmentId, IPC_RMID, NULL);
	return 0;
}


void runner (int id)
{
	int currentRow = id;
	C = (int *) shmat(segmentId, NULL, 0);
	
	int i = 0, currentCol = 0;
	/* Chooses which rows to handle given an argument id */
	while (currentRow < M)
	{
		for (currentCol = 0; currentCol < N; currentCol++)
			multiply (currentRow, currentCol);
		currentRow += nProcesses;
	}	
	shmdt(C); // detaches the shared memory segment
}

inline void multiply (int row, int col)
{
	int i;
	C[row*N+col] = 0;
	for (i = 0; i < K; i++)
		C[row*N+col] += A[row*K+i] * B[i*N + col];
}



