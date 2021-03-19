#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>

// FUNCTIONS BEGIN//
int getNumOfDigits(int x) 
{
    int tmp = x, numOfDigits = 0;
    while (tmp > 0) 
    {
        numOfDigits++;
        tmp /= 10;
    }
    return numOfDigits;
}

void pidToStr(int pid, int numOfDigits, char *result) 
{
    int tmp = pid;
    for (int i = numOfDigits - 1; i > -1; i--) 
    {
        result[i] = tmp % 10 + '0';
        tmp /= 10;
    }
}
// FUNCTIONS END//

int main(int argc, char * argv[])
{
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int numOfCommands = 0;

    fp = fopen(argv[1], "r");
    if (!fp) 
    {
                fclose(fp);
                printf("range: cannot open file\n");
                return 1; // file failed to open.
    }

    while ((read = getline(&line, &len, fp)) != -1) 
    {
        numOfCommands++;
    }

    fclose(fp);

    int id1 = 0, numOfChildren = 0;

    while (numOfChildren < numOfCommands) // only main process performs loop
    {
        id1 = fork();
        if (id1 == -1) {
            printf("Fork failed");
            return 1;
        }

        numOfChildren++; //use this is map to filename arguments

            // all child process open their own file: mapped with (numOfChildren)
        if (id1 == 0)
        {
            int i,j,ctr, maxStrLen = 0, maxNumOfWords = 0;
            fp = fopen(argv[1], "r");
            if (!fp) 
            {
                        fclose(fp);
                        printf("range: cannot open file\n");
                        return 1; // file failed to open.
            }

            //get the correct line for the child
            for(int i = 0; i < numOfChildren; i++) 
            {
                if((read = getline(&line, &len, fp)) == -1) 
                {
                    printf("read failed with child: %i\n", numOfChildren);
                    return 1;
                }       
            }

            j=0; ctr=0; //ctr stores the amount of words in the 2d array!

            for(i=0; i<= (strlen(line)); i++)
            {
                // if space or NULL found, assign NULL into newString[ctr]
                if(line[i]==' '|| line[i]=='\0')
                {
                    ctr++;  //for next word
                    maxStrLen = (j > maxStrLen)? j + 1: maxStrLen;
                    maxNumOfWords = (ctr > maxNumOfWords)? ctr : maxNumOfWords;
                    j=0;    //for next word, init index to 0
                }
                else
                {
                    j++;
                }
            }
            
            // Each child process reads the numbers in their file
            // and creates partial summation
            // stored in result
            fp = fopen(argv[1], "r");
            if (fp == NULL) 
            {
                exit(EXIT_FAILURE);
            }

            char newString[maxNumOfWords][maxStrLen]; 

            // printf("---------------------------------------\n");
            // printf("\n\n Split string by space into words :\n");
            // printf("Retrieved line of length %zu:\n", read);
            // printf("%s\n", line);


            j=0; ctr=0; //ctr stores the amount of words in the 2d array!

            for(i=0; i<= (strlen(line)); i++)
            {
                // if space or NULL found, assign NULL into newString[ctr]
                if(line[i]==' '||line[i]=='\0' || line[i] == '\n')
                {
                    newString[ctr][j]='\0';
                    ctr++;  //for next word
                    j=0;    //for next word, init index to 0
                }
                else
                {
                    newString[ctr][j]=line[i];
                    j++;
                }
            }

            char* vector[maxNumOfWords + 1];
            for(int i = 0; i < maxNumOfWords; i++) 
            {
                vector[i] = (char *) &newString[i][0];
            }
            vector[maxNumOfWords] = (char*) 0;

            fclose(fp);
            if (line)
            {
                free(line);
            }

            /* Create out and err filenames */
            int pid = getpid();

            int numOfDigits = getNumOfDigits(pid);
            char * errFileName = (char *)malloc(sizeof(char) * (numOfDigits + 5));
            char * outFileName = (char *)malloc(sizeof(char) * (numOfDigits + 5));

            pidToStr(pid, numOfDigits, outFileName);
            pidToStr(pid, numOfDigits, errFileName);

            outFileName[numOfDigits] = '.';
            outFileName[numOfDigits + 1] = 'o';
            outFileName[numOfDigits + 2] = 'u';
            outFileName[numOfDigits + 3] = 't';
            outFileName[numOfDigits + 4] = '\0';

            errFileName[numOfDigits] = '.';
            errFileName[numOfDigits + 1] = 'e';
            errFileName[numOfDigits + 2] = 'r';
            errFileName[numOfDigits + 3] = 'r';
            errFileName[numOfDigits + 4] = '\0';

            /* Redirect the output! */
            int outFile = open(outFileName, O_RDWR | O_CREAT | O_APPEND, 0777);
            if(outFile == -1) 
            {
                return 2;
            }
            int errFile = open(errFileName, O_RDWR | O_CREAT | O_APPEND, 0777);
            if(errFile == -1) 
            {
                return 2;
            }

            // redirect output to STDOUT and STDERR
            int outFile2 = dup2(outFile, STDOUT_FILENO);
            int errFile2 = dup2(errFile, STDERR_FILENO);

            close(outFile);
            close(errFile);

            printf("Starting command %i: child %i pid of parent %i\n", numOfChildren, getpid(), getppid());
            free(outFileName);
            free(errFileName);

            if(execvp(vector[0], vector) == -1) 
            {
                int errFile2 = dup2(errFile2, STDOUT_FILENO);
                perror(vector[0]);
                return 2;
            }
        }
        else //parent process
        {
            int pid2, wstatus;

            while((pid2 = wait(&wstatus)) > 0)
            {
                // printf("pid2: %d\n", pid2);
                if(WIFEXITED(wstatus))
                {
                    int statusCode = WEXITSTATUS(wstatus);
                    if(statusCode == 0) 
                    {
                        // printf("Success!\n");
                        int cpid = pid2;

                        int numOfDigits = getNumOfDigits(cpid);
                        char * errFileName = (char *)malloc(sizeof(char) * (numOfDigits + 5));
                        char * outFileName = (char *)malloc(sizeof(char) * (numOfDigits + 5));

                        pidToStr(cpid, numOfDigits, outFileName);
                        pidToStr(cpid, numOfDigits, errFileName);

                        outFileName[numOfDigits] = '.';
                        outFileName[numOfDigits + 1] = 'o';
                        outFileName[numOfDigits + 2] = 'u';
                        outFileName[numOfDigits + 3] = 't';
                        outFileName[numOfDigits + 4] = '\0';

                        errFileName[numOfDigits] = '.';
                        errFileName[numOfDigits + 1] = 'e';
                        errFileName[numOfDigits + 2] = 'r';
                        errFileName[numOfDigits + 3] = 'r';
                        errFileName[numOfDigits + 4] = '\0';

                        /* Redirect the output! */
                        FILE * outFile = fopen(outFileName, "a+b");
                        if(outFile == NULL) 
                        {
                            return 2;
                        }

                        FILE * errFile = fopen(errFileName, "a+b");
                        if(errFile == NULL) 
                        {
                            return 2;
                        }

                        fprintf(outFile, "Finished child %i pid of parent %i", cpid, getpid());
                        fprintf(errFile, "Exited with exitcode = %i", wstatus);

                        fclose(outFile);
                        fclose(errFile);
                    }   
                }
                else if (WIFSIGNALED(wstatus))
                {
                    int cpid = pid2;

                    int numOfDigits = getNumOfDigits(cpid);
                    char * errFileName = (char *)malloc(sizeof(char) * (numOfDigits + 5));
                    char * outFileName = (char *)malloc(sizeof(char) * (numOfDigits + 5));

                    pidToStr(cpid, numOfDigits, outFileName);
                    pidToStr(cpid, numOfDigits, errFileName);

                    outFileName[numOfDigits] = '.';
                    outFileName[numOfDigits + 1] = 'o';
                    outFileName[numOfDigits + 2] = 'u';
                    outFileName[numOfDigits + 3] = 't';
                    outFileName[numOfDigits + 4] = '\0';

                    errFileName[numOfDigits] = '.';
                    errFileName[numOfDigits + 1] = 'e';
                    errFileName[numOfDigits + 2] = 'r';
                    errFileName[numOfDigits + 3] = 'r';
                    errFileName[numOfDigits + 4] = '\0';

                    /* Redirect the output! */
                    FILE * outFile = fopen(outFileName, "a+b");
                    if(outFile == NULL) 
                    {
                        return 2;
                    }

                    FILE * errFile = fopen(errFileName, "a+b");
                    if(errFile == NULL) 
                    {
                        return 2;
                    }

                    fprintf(outFile, "Finished child %i pid of parent %i", cpid, getpid());
                    fprintf(errFile, "Killed with signal %i", WTERMSIG(wstatus));

                    free(errFileName);
                    free(outFileName);
                    fclose(outFile);
                    fclose(errFile);

                }
                else 
                {
                    return 2;
                }
            }
        }
    }           

    exit(EXIT_SUCCESS);
}

