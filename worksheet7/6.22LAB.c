/*
 * Code by Matthew Zuberbuhler & Sergio Gutierrez
 */
#include <stdio.h>
#include <stdlib.h>

void SwapValues(int* userVal1, int* userVal2)
{
   int tmp = *userVal1;
   *userVal1 = *userVal2;
   *userVal2 = tmp;
}

int main(void) {

   int * userVal1 = (int *)malloc(sizeof(int));
   int * userVal2 = (int *)malloc(sizeof(int));;
   scanf("%d %d", userVal1, userVal2);
   SwapValues(userVal1, userVal2);
   printf("%i %i\n", *userVal1, *userVal2);

   free(userVal1);
   free(userVal2);

   return 0;
}
