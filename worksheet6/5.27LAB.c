/*
 * Code by Matthew Zuberbuhler & Sergio Gutierrez 
 */
#include <stdio.h>
#include <string.h>

int main(void) {

   int count = 0;
   char str[50];
   scanf("%[^\n]%*c", str);
   int size = strlen(str), flag = 0, tmp;

   //count spaces
   for(int i = 0; str[i]; i++)
   {
      if(str[i] == ' ') 
      {
         count++;
      }
   }
   if(count != 0) 
   {
      char pStr[size - count];
      for(int i = 0, j = 0; i < size; i++)
      {
         if(str[i] == ' ') 
         {
            
         } else
         {
            pStr[j] = str[i];
            j++;
            tmp = j;
         }
      }
      pStr[tmp] = '\0';
      for(int i = 0, j = strlen(pStr) - 1; i < strlen(pStr); i++)
      {
         if(pStr[i] != pStr[j]) 
         {
            flag = 1;
            printf("not a palindrome: %s\n", str);
            break;
         }
         j--;
         
      }
      if(flag == 0) 
      {
         printf("palindrome: %s\n", str);
      }
      
   }
   else 
   {
      for(int i = 0, j = size - 1; i < size; i++)
      {
         if(str[i] != str[j]) 
         {
            flag = 1;
            printf("not a palindrome: %s\n", str);
            break;
         }
         j--;
         
      }
      if(flag == 0) 
      {
         printf("palindrome: %s\n", str);
      }
   }

   return 0;
}
