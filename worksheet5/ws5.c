/*
 * Code by Matthew Zuberbuhler & Sergio Gutierrez 
 */
#include <stdio.h>
#include <unistd.h> 
#include <sys/wait.h>
#include <errno.h>

int main (int argc, char* argv[]) 
{
	fork();
	fork();

	printf("hello world from PID %d!\n", getpid());
	
	while(wait(NULL) != -1 || errno != ECHILD);
	/*{
		printf("ending child process\n");
	}
	*/
}	