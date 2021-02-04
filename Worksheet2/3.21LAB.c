/*
 * Code by Matthew Zuberbuhler & Sergio Gutierrez 
 */
#include <stdio.h>

int main(void) {

   int x, y ,z;
   scanf("%i %i %i", &x, &y, &z);
   if (x <= y) 
   {
      if (x <= z) 
      {
         printf("%i\n", x);
      }
      else 
      {
         printf("%i\n", z);
      }
   }
   else if (y <= z) 
   {
      printf("%i\n", y);
   } 
   else
   {
     printf("%i\n", z); 
   }

   return 0;
}
