#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char * argv[])
{
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int numOfCommands = 0;

    // this is us making a static array to hold as many as 50 words each up to 49 characters long
    // we can make this dynamically allocated array by parsing through file first and finding
    // the longest word and the max amount of words
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

    printf("number of commands: %d\n", numOfCommands);

    fclose(fp);

/*________________________________________________________________________________________________________________*/
/*________________________________________________________________________________________________________________*/
/*________________________________________________________________________________________________________________*/
/*________________________________________________________________________________________________________________*/
/*________________________________________________________________________________________________________________*/
/*________________________________________________________________________________________________________________*/

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
            if (fp == NULL) {
                exit(EXIT_FAILURE);
            }

            char newString[maxNumOfWords][maxStrLen]; 

            // //get the correct line for the child
            // for(int i = 0; i < numOfChildren; i++) 
            // {
            //     if((read = getline(&line, &len, fp)) == -1) 
            //     {
            //         printf("read failed with child: %i\n", numOfChildren);
            //         return 1;
            //     }       
            // }

            printf("---------------------------------------\n");
            printf("\n\n Split string by space into words :\n");
            printf("Retrieved line of length %zu:\n", read);
            printf("%s\n", line);


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

            printf("\n Strings or words after split by space are :\n");
            for(i=0;i < ctr;i++) 
            {
                printf(" %s\n", newString[i]);
            }

            printf("maxNumOfWords: %d\n", maxNumOfWords);

            char* vector[maxNumOfWords + 1];
            for(int i = 0; i < maxNumOfWords; i++) 
            {
                vector[i] = (char *) &newString[i][0];
                printf("vector[%i] = _%s_\n", i, vector[i]);
            }
            vector[maxNumOfWords] = (char*) 0;

            fclose(fp);
            if (line)
            {
                free(line);
            }

            execvp(vector[0], vector);

            /*
                DO SOMETHING HERE WITH CHILDREN
            */
        }


        // main process reads from pipe after waiting for each child
        if(id1 != 0)
        {
            //DO SOMETHING HERE WITH PARENT PROCESS
            while(wait(NULL) > 0);
        }
        else 
        {
             //DO SOMETHING HERE WITH CHILD PROCESS
        }

        if (id1 == 0) // all children leave loop after created
        {
            break;
        }
    }           

/*________________________________________________________________________________________________________________*/
/*________________________________________________________________________________________________________________*/
/*________________________________________________________________________________________________________________*/
/*________________________________________________________________________________________________________________*/
/*________________________________________________________________________________________________________________*/
/*________________________________________________________________________________________________________________*/


    exit(EXIT_SUCCESS);
}

