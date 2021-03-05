#include "apue.h"

int
main(void)
{
	pid_t	pid;
	
	if ((pid = fork()) < 0)
		err_sys("fork error");
	else if (pid != 0) {		/* parent */
	
		printf("main process pid: %i, getpid(): %i\n", (int)pid, getpid()); /* PRINT PROCESS */
		
		sleep(2);
		exit(2);				/* terminate with exit status 2 */
	}

	printf("Child of main process pid: %i\n, getpid(): %i\n", (int)pid, getpid()); /* PRINT PROCESS */
	
	if ((pid = fork()) < 0)
		err_sys("fork error");
	else if (pid != 0) {		/* first child */
	
		printf("Child of main process: %i\n, getpid(): %i\n", (int)pid, getpid()); /* PRINT PROCESS */
		
		sleep(4);
		abort();				/* terminate with core dump */
	}

	printf("Child of main process's child(grand child) pid: %i\n, getpid(): %i\n", (int)pid, getpid()); /* PRINT PROCESS */	
	
	if ((pid = fork()) < 0)
		err_sys("fork error");
	else if (pid != 0) {
							/* second child */
		printf("Child of main process's child(grand child) pid: %i\n, getpid(): %i\n", (int)pid, getpid()); /* PRINT PROCESS */
							
		execl("/bin/dd", "dd", "if=/etc/passwd", "of=/dev/null", NULL);
		exit(7);				/* shouldn't get here */
	}
	
	printf("child of the child of main process's child(great grand child) pid: %i\n, getpid(): %i\n", (int)pid, getpid()); /* PRINT PROCESS */

	if ((pid = fork()) < 0)
		err_sys("fork error");
	else if (pid != 0) {		/* third child */
	
		printf("child of the child of main process's child(great grand child) pid: %i\n, getpid(): %i\n", (int)pid, getpid()); /* PRINT PROCESS */
	
		sleep(8);
		exit(0);				/* normal exit */
	}
	
	printf("child of child of the child of main process's child(great great grand child)pid: %i\n, getpid(): %i\n", (int)pid, getpid()); /* PRINT PROCESS */

	sleep(6);					/* fourth child */
	kill(getpid(), SIGKILL);	/* terminate w/signal, no core dump */
	printf("No children or main process!\n");
	exit(6);					/* shouldn't get here */
}