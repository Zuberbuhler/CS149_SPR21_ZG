/*
 * Code by Matthew Zuberbuhler & Sergio Gutierrez 
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(void) 
{

   char str[10][50];
   int count = 0, flag = 0;
   
   for(int i = 0; flag == 0; i++)
   {
      
      count++;
      scanf("%[^\n]%*c", str[i]);
      if(strcmp(str[i], "Done") == 0 || strcmp(str[i], "done") == 0 || strcmp(str[i], "d") == 0) 
      {
         count--;
         break;
      }
      if(count > 10) {
         break;
      }
   
   }
   for(int i = 0; i < count; i++)
   {
      for(int j = strlen(str[i]) - 1; j > -1; j--) 
      {
         printf("%c", str[i][j]);
      }
      printf("\n");
   }
   
   //use strlen(str[i]) to find length of strings
   
   return 0;
}
