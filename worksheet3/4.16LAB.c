/*
 * Code by Matthew Zuberbuhler & Sergio Gutierrez 
 */
#include <stdio.h>

int main(void) {

   int tmp = 0, max = 0, sum = 0, count = 0;
   while (tmp > -1) {
      scanf("%i", &tmp);
      if (tmp > -1) 
      {
         count++;
         sum += tmp;
      }
      if (tmp > max) 
      {
         max = tmp;
      }
   }
   printf("%i %i\n", max, sum / count);

   return 0;
}
