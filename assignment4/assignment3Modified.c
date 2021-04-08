//// Created on 3/17/21.
// Sample solution//#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

struct nlist
{
    /* table entry: */
    struct nlist *next; /* next entry in chain */
    // char *name;  /* defined name, can remove */
    // char *defn; /* replacement text, can remove */

    /* starttime and finishtime */
    struct timespec start;
    struct timespec finish;

    /* index // this is the line index in the input text file */
    int index;
    /*
        pid  // the process id. you can use the pid result of
                wait to lookup in the hashtable
    */
    int pid;
    /*
    char *command; // command. This is good to store for when you
                               decide to restart a command
    */
    char* command;
};

#define HASHSIZE 101
int HashTableSize = 0;
static struct nlist *hashtab[HASHSIZE]; /* pointer table */

/* This is the hash function: form hash value for string s */
/* TODO change to: unsigned hash(int pid) */
/* TODO modify to hash by pid . */

/* You can use a simple hash function: pid % HASHSIZE */
unsigned hash(int pid)
{
    //printf("Hash: returning hash: %i\n", pid % HASHSIZE);
    return pid % HASHSIZE;
}

/* lookup: look for s in hashtab */
/* TODO change to lookup by pid: struct nlist *lookup(int pid) */
/* TODO modify to search by pid, you won't need strcmp anymore */
/*
This is traversing the linked list under a slot of the hash
table. The array position to look in is returned by the hash function
*/

struct nlist *lookup(int pid)
{
    //printf("Lookup: Start-> ");
    struct nlist *np;
    for (np = hashtab[hash(pid)]; np != NULL; np = np->next)
    {
        if (np->pid == pid)
        {
            //printf("Lookup: found pid in slot already!\n");
            return np; /* found */
        }

    }
    //printf("Lookup: End\n");
    return NULL; /* not found */
}

/* insert: put (name, defn) in hashtab */

/* TODO: change this to insert in hash table the info for a
new pid and its command */

/* TODO: change signature to:
    struct nlist *insert(char *command,int pid, int index). */

/* This insert returns a nlist node.
Thus when you call insert inyour main function  */

/* you will save the returned nlist node in a variable (mynode). */

/* Then you can set the starttime and finishtime from your main function: */

/* mynode->starttime = starttime; mynode->finishtime = finishtime; */

//char *name, char *defn
struct nlist *insert(char *command, int pid, int index)
{
    //printf("Inserting Start:-> ");
    struct nlist *np;
    unsigned hashval;
    //TODO change to lookup by pid. There are 2 cases:
    if ((np = lookup(pid)) == NULL)
    {
        HashTableSize++;
        //printf("Inserting: case1 creating new node in slot np-> ");
        /* case 1: the pid is not
        found, so you have to create it with malloc.
        Then you want to set the pid, command and index */
        np = (struct nlist *) malloc(sizeof(*np));
        if (np == NULL || ((np->command = strdup(command)) == NULL))
        {
            //printf("Inserting: Failed to malloc np and returning NULL\n");
            return NULL;
        }
        np->pid = pid;
        np->index = index;

        hashval = hash(pid);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    } else
    {
        //printf("Inserting: Case2 np found in slot already-> ");
        /* case 2: the pid is already there in the hashslot,
        i.e. lookup found the pid. In this case you can either
        do nothing, or you may want to set again the command
        and index (depends on your implementation). */
        free((void *) np->command); /*free previous command */
        if ((np->command = strdup(command)) == NULL)
        {
            //printf("Inserting: updating np->command failed and return null\n");
            return NULL;
        }
        np->index = index;
    }


    //printf("Inserting: End\n");
    return np;
}

/** You might need to duplicate the command string to ensure
you don't overwrite the previous command each time a new line
is read from the input file. Or you might not need to duplicate it.
It depends on your implementation. **/

char* strduplicate(char *s) /* make a duplicate of s */
{
    //printf("strduplicate: Start ->");
    char *p;
    p = (char *) malloc(strlen(s)+1); /* +1 for \0 */

    if (p != NULL)
    {
        strcpy(p, s);
    }

    //printf("strduplicate: End\n");
    return p;
}

void clearHashTable() {
    for (int i = 0; i < HASHSIZE; i++)
    {
        if (hashtab[i] != NULL)
        {
            //free the entire linked list
        }
    }
}

//helper method to do execvp
void doExecvp(char lineToParse[], int commandNum, int restartedProcess) {

    //make name of the file
    int pid = getpid();
    char pidArr[10];

    //convert int to string
    sprintf(pidArr, "%d", pid);

    //concatenate pidArr with string ".out"
    char addMe[] = ".out";
    strcat(pidArr, addMe);

    int f_write = open(pidArr, O_RDWR | O_CREAT | O_TRUNC, 0777);
    //creates a copy of the file descriptor f_write
    dup2(f_write, fileno(stdout));

    if(restartedProcess)
    {
        fprintf(stdout, "RESTARTING\n");
    }

    fprintf(stdout, "Starting command %d: child %d pid of parent %d\n",
                                        commandNum, getpid(), getppid());
    //move the buffered data to console

    fflush(stdout);

    //preparing to parse string to pass into execvp arguments    
    char *input[strlen(lineToParse)];
    char *token = strtok(lineToParse, " \n");
    int index = 0;

    //parsing    
    while (token != NULL) {
        input[index] = token;
        index++;
        token = strtok(NULL, " \n");
    }

    input[index] = NULL;

    //printing to the out file
    //fprintf(stdout,"Finished child %d pid of parent %d\n", getpid(), getppid());

    fflush(stdout);
    fflush(stderr);

    close(f_write);

    //call execvp    
    if (execvp(input[0], input) == -1) {
        char pidArrforError[10];

        //convert int pid to string
        sprintf(pidArrforError, "%d", pid);
        char addMeError[] = ".err";

        // concatenate
        strcat(pidArrforError, addMeError);

        int f_err = open(pidArrforError, O_RDWR | O_CREAT | O_APPEND, 0777);
        //If execvp succeeds it doesn't run below but if Execvp fails and it runs the following:
        dup2(f_err, STDOUT_FILENO);
        dup2(f_err, STDERR_FILENO);
        perror("Execvp failed: \n");
        fflush(stdout);
        fflush(stderr);
        close(f_err);
    }
}

int main(int argc, const char * argv[])
{
    //argc at least = 2; 1 for name of the file (proc_manager), other is input file    
    if(argc < 2){
        printf("Incorrect input!\n");
        return 1;
    }

    // Open file
    FILE *fp = fopen(argv[1], "r");
    if(fp == NULL){
        printf("Null file.\n");
        return 1;
    }

    char result[100];
    int commandNum = 0;
    struct timespec start, finish;

    //new line is recorded from text file
    while (fgets(result, 100, fp)){
        commandNum++;
        //printf("result: _%s_\n", result);
        //fflush(stdout);
        //fflush(stderr);

        clock_gettime(CLOCK_MONOTONIC, &start);
        int pid = fork();


        if(pid == 0)
        {
            doExecvp(result, commandNum, 0);
            exit(0);

        }
        else if(pid > 0) //parent process
        {
            //store process data in hash
            struct nlist* myNode = insert(result, pid, commandNum);
            myNode->start = start;
        } else {
            printf("Error in forking.\n");
            exit(1);
        }
    }

    int wpid;

    int status;
    struct nlist* myNode1;
    double elapsed;

    //printf("hashtable size = %i\n", HashTableSize);

    while( (wpid = wait(&status)) > 0)
    {
        fflush(stdout);
        fflush(stderr);
        myNode1 = lookup(wpid);
        clock_gettime(CLOCK_MONOTONIC, &finish);
        if(myNode1 != NULL)
        {
            myNode1->finish = finish;
            elapsed = ((double)myNode1->finish.tv_sec + 1.0e-9 * myNode1->finish.tv_nsec)
                        - ((double)myNode1->start.tv_sec + 1.0e-9 * myNode1->start.tv_nsec); //subtraction

//            printf("myNode1->pid = %i\n"
//                   "myNode1->index = %i\n"
//                   "myNode1->command = _%s_\n"
//                   "elapsed time = %.5f\n\n",
//                   myNode1->pid, myNode1->index, myNode1->command, elapsed);

        }
        else {
            printf("myNode1 = NULL!\n");
        }


        //fflush(stdout);
        //fflush(stderr);

        char outFileName[20];
        sprintf(outFileName, "%d.out", wpid);
        FILE * outFP = fopen(outFileName, "a+b");
        if(outFP == NULL)
        {
            return 2;
        }
        fprintf(outFP, "Finished child %i pid of parent %i\n", wpid, getpid());
        fprintf(outFP, "Finished at %ld, runtime duration %.5f", myNode1->finish.tv_nsec, elapsed);
        //make name of the file
        char pidArrforError[10];

        //convert int pid to string
        sprintf(pidArrforError, "%d.err", wpid);

        int f_err = open(pidArrforError, O_RDWR | O_CREAT | O_APPEND,0777);

        dup2(f_err, fileno(stderr));

        //Normal termination with exit code        
        if(WIFEXITED(status)){
            fprintf(stderr, "Exited with exitcode = %d\n", WEXITSTATUS(status));
            if(elapsed <= 2) {
                fprintf(stderr, "spawning too fast");
            }
        }

            //Abnormal Termination with exit signal
        else if(WIFSIGNALED(status)){
            fprintf(stderr, "Killed with signal = %d\n", WTERMSIG(status));
            if(elapsed <= 2) {
                fprintf(stderr, "spawning too fast");
            }
        }

        close(f_err);

        //decide if you will restart
        if (elapsed > 2) {
            //save the startime!
            clock_gettime(CLOCK_MONOTONIC, &start);
            int pid = fork();

            if (pid < 0)
            {
                printf("Error in forking.\n");
                exit(1);
            }
            if (pid == 0) //child
            {
            // See shell1_execvp.c for execvp usage
                doExecvp(myNode1->command, myNode1->index, 1);
            } else if (pid > 0)
            {
                struct nlist* myNode2 = insert(myNode1->command, pid, myNode1->index);
                myNode2->start = start;
            }
            else {
                printf("Error in forking.\n");
                exit(1);
            }
        }
    }

    return 0;
}