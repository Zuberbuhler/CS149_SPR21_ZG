#include <stdio.h>
#include <string.h>

int CountCharacters(char userChar, char* userString) 
{
   int count = 0;
   for (int i = 0; userString[i]; i++) 
   {
      if(userString[i] == userChar) 
      {
         count++;
      }
   }
   return count;
}

int main(void) {

   char ch;
   char *userString;
   scanf("%c %s", &ch, userString);
   printf("%i\n", CountCharacters(ch, userString));

   return 0;
}

