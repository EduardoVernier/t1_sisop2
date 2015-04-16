//http://stackoverflow.com/questions/15328285/how-to-fork-an-exact-number-of-children
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> /* for pid_t */
#include <sys/wait.h> /* for wait */

void doWork(char *arg) {
    while (1) {
		printf("%s\n", arg);
	}
}

int main()
{
	int processes = 5;
	int i;

	for (i = 0; i < processes; i++)
	{
		if (fork() == 0) //only child processes
		{
			printf("Child\n");
			exit(0);
		}
	}
	
	// wait all child processes
	int status;
	for (i = 0; i < processes; ++i)
		wait(&status);

	return 0;
}

