/*
 * Code by Matthew Zuberbuhler & Sergio Gutierrez 
 */
#include <stdio.h>

int main(void) {

   int dividend;
   int divisor;
   int res;
   scanf("%d %d", &dividend, &divisor);

   for (int i = 0; i < 2; i++) 
   {
      dividend = dividend / divisor;
      printf("%d ", dividend);
   }
    printf("%d", dividend / divisor);
   printf("\n");
   
   return 0;
   
   
}

