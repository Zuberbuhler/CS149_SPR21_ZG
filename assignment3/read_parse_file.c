#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(void)
{
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    // this is us making a static array to hold as many as 50 words each up to 49 characters long
    // we can make this dynamically allocated array by parsing through file first and finding
    // the longest word and the max amount of words
    int i,j,ctr, maxStrLen = 0, maxNumOfLines = 0;
    fp = fopen("cmdfile.txt", "r");

    while ((read = getline(&line, &len, fp)) != -1) 
    {
        j=0; ctr=0; //ctr stores the amount of words in the 2d array!

        for(i=0; i<= (strlen(line)); i++)
        {
            // if space or NULL found, assign NULL into newString[ctr]
            if(line[i]==' '|| line[i]=='\0')
            {
                printf("j + 1: %d\n", j + 1);
                ctr++;  //for next word
                maxStrLen = (j > maxStrLen)? j + 1: maxStrLen;
                maxNumOfLines = (j > maxNumOfLines)? ctr: maxNumOfLines;
                j=0;    //for next word, init index to 0
            }
            else
            {
                j++;
            }
        }
    }

    printf("maxStrLen: %d\nnumberOfWords = %d\n", maxStrLen, maxNumOfLines);
    fclose(fp);
    char newString[maxNumOfLines][maxStrLen]; 
    

    fp = fopen("cmdfile.txt", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1) 
    {

        printf("---------------------------------------\n");

        printf("\n\n Split string by space into words :\n");

        printf("Retrieved line of length %zu:\n", read);
        printf("%s\n", line);
        printf("string length of line: %lu\n", strlen(line));


        //reseting the char array
	    for (i=0; i<20; i++) 
        {
            for (j=0; j<20; j++)
            {
                newString[i][j]=0;
            }
        }

    	j=0; ctr=0; //ctr stores the amount of words in the 2d array!

        for(i=0; i<= (strlen(line)); i++)
        {
            // if space or NULL found, assign NULL into newString[ctr]
            if(line[i]==' '||line[i]=='\0')
            {
                printf("j = %d\n", j);
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
    }

    char *vector[maxNumOfLines + 1];
    for(int i = 0; i < maxNumOfLines; i++) 
    {
        vector[i] = (char *) &newString[i][0];
        printf("vector[%i] = %s\n", i, vector[i]);
    }
    vector[maxNumOfLines] = (char *) 0;
    

    printf("\n%s\n",newString[maxNumOfLines - 1]);
    //so basically after the 2d char array has completed, the words are stored in each row of the array 
    // there value of ctr holds the amount of words in the array.

    fclose(fp);
    if (line)
    {
        free(line);
    }

    execvp(vector[0], vector);
    exit(EXIT_SUCCESS);
}

