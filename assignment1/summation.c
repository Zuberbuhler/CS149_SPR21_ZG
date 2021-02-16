/* Code by Matthew Zuberbuhler & Sergio Gutierrez CS149 SPR21
compile using the following command:
	gcc summation.c -o main -lm

run using the following command:
	./main <filename> <>
*/
#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
#include <math.h> //use pow() if floats have fraction values Zuberbuhler, Matthew



int findSize(FILE * fp)
{
    int size = 0;
    double tmp;
    while(fscanf(fp, "%lf", &tmp) != EOF)
    {
        size++;
    }
    //printf("size: %i\n", size); //for testing
    return size;
}

double summation(FILE * fp, int size, double exponent) {

    double result = 0;
    rewind(fp); //start scan from beginning of file

    //create arr with exact size from findSize() function
    double arr[size];

    //loop until end of file
    //each fscanf will grab 1 line and returns EOF at "End Of File"
    for(int i = 0; fscanf(fp, "%lf", &arr[i]) != EOF; i++)
    {
        if(exponent == 1) {
            result += arr[i];
        }
        else
        {
            result += pow(arr[i], exponent);
        }

    }
    return result;
}

int main( int argc, char **argv)  {

    /*
        The arguments are provided from terminal...

        type the following into terminal:
            ./<compiled_filename> <insert any string here>
        Ex:
            ./out arg1

        Explanation:
            argc will store the number of arguments passed in terminal + 1
                    ... # of args + 1 because it counts the
                        compiled_filename as the first argument
            argv[0] = compile_filename
            argv[1] = arg1
            argv[2] = arg2
            ...	and so on

        In this assignment:
             argv[1] = <filename>
             argv[2] = <number for exponent>
    */
    //for testing
    //printf(
    //        "argc: %i\n"
    //        "argv[0]: %s\n"
    //        "argv[1]: %s\n"
    //        "argv[2]: %s\n"
    //        , argc, argv[0], argv[1], argv[2]
    //);

    //!!!!!!must check if arguments are even passed! USE argc for count of args

    int size; //flags

    if (argc != 3) {
        //basically, there is nothing to do so we return 0;
        printf("No arguments provided!\n");
        return 0; //program stops after return
    }

    if (argc == 3)  //both a filename and number are passed from terminal
    {
        double summationResult;
        double exponent;
        char *tmpPtr; //for use in strtod ()

        exponent = strtod(argv[2], &tmpPtr);
        //printf("exponent: %0.2lf\n", exponent); //for testing

        char *filename = argv[1];

        FILE * fp;

        //"fopen" opens a file for subsequent reading or writing.
        //If successful, "fopen" returns a pointer-to-structure;
        //if it fails, it returns NULL.
        fp = fopen (filename, "r");

        if(!fp)
        {
            fclose(fp);
            printf("range: cannot open file\n");
            return 1; //file failed to open.

        }
        size = findSize(fp);
        if(size == 0)
        {
            printf("No numbers in file\n");
            return 0;
        }


        summationResult = summation(fp, size, exponent);

        fclose(fp);
        //printf("return 0"); //return 0 if no errors
        printf("%0.2lf\n", summationResult);
        return 0;
    }
}
