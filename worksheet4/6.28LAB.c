/*
 * Code by Matthew Zuberbuhler & Sergio Gutierrez 
 */
#include <stdio.h>

int LargestNumber(int userNum1, int userNum2, int userNum3)
{
   if (userNum1 < userNum2) 
   {
      if(userNum2 < userNum3) 
      {
         return userNum3;
      }
      else 
      {
         return userNum2;
      }
   } else if(userNum1 < userNum3) 
   {
      return userNum3;
   }
   else 
   {
      return userNum1;
   }
   
}
int SmallestNumber(int userNum1, int userNum2, int userNum3)
{
   if (userNum1 > userNum2) 
   {
      if(userNum2 > userNum3) 
      {
         return userNum3;
      }
      else 
      {
         return userNum2;
      }
   } else if(userNum1 > userNum3) 
   {
      return userNum3;
   }
   else 
   {
      return userNum1;
   }
   
}

int main(void) {

   int in1, in2, in3;
   scanf("%i %i %i", &in1, &in2, &in3);
   printf("largest: %i\nsmallest: %i\n", LargestNumber(in1, in2, in3), SmallestNumber(in1, in2, in3));

   return 0;
}
