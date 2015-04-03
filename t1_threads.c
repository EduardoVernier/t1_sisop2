#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int main (int argc, char *argv[]) 
{
	FILE *in1, *in2;
	int m1Rows, m1Cols;
	int m2Rows, m2Cols;
	int nThreads;

	if (argv[1] == NULL)
	{
		printf ("No argument was found. Use ./a.out #treads\n");
		return 0;
	}
	nThreads = atoi(argv[1]);
	
	in1 = fopen("in1.txt", "r");
	in2 = fopen("in2.txt", "r");
	if (in1 == NULL || in2 == NULL)
		return 0;	

	/* Reading rows and columns number*/
	fscanf(in1, "LINHAS = %d\n", &m1Rows);
	fscanf(in1, "COLUNAS = %d", &m1Cols);
	fscanf(in2, "LINHAS = %d\n", &m2Rows);
	fscanf(in2, "COLUNAS = %d", &m2Cols);	
	if (m1Rows != m2Cols || m1Cols != m2Rows)
	{
		printf ("Invalid Matrices\n");
		return 0;
	}
	
	/* Allocate matrices */
	int *m1 = (int *) malloc (m1Rows * m1Cols * sizeof(int));
	int *m2 = (int *) malloc (m2Rows * m2Cols * sizeof(int));
	
	/* Filling matrices */
	int r, c;
	for (r = 0; r < m1Rows; r++)
		for(c =0; c < m1Cols; c++)
			fscanf(in1, "%d", &m1[r*m1Cols + c]);
	
	for (r = 0; r < m2Rows; r++)
		for(c =0; c < m2Cols; c++)
			fscanf(in2, "%d", &m2[r*m2Cols + c]);
	
	
	return 0;	
}
