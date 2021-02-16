/*
 * Code by Matthew Zuberbuhler & Sergio Gutierrez 
 */
#include <stdio.h>
#include <string.h>

void IntegerToReverseBinary(int integerValue, char binaryValue[])
{
   for(int i = 0; integerValue > 0; i++) 
   {
      binaryValue[i] = (integerValue % 2) + '0';
      integerValue /= 2;
   }
}
void ReverseString(char inputString[], char reversedString[])
{
   int j = 0;
   for (int i = strlen(inputString) -1; i >= 0; i--) 
   {
      reversedString[j] = inputString[i];
      j++;
   }
   reversedString[j] = '\0';
}

int main(void) {
   int input, size = 0;
   scanf("%i", &input);
   //printf("%i", input);
   for(int i = input; i > 0; ) 
   {
      i /= 2;
      size++;
   }
  // printf("%i", size);
   char rbVal[size], bVal[size];
   
   

   IntegerToReverseBinary(input, rbVal);
   //printf("%s\n", rbVal);
   ReverseString(rbVal, bVal);
   printf("%s\n", bVal);

   return 0;
}
