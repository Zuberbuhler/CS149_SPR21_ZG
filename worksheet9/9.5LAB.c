#include<stdio.h>
#include<string.h>

int main()
{
      while(1)
   {
    char string[1000],first_word[1000],second_word[1000];
     printf("Enter input string:\n");
      scanf(" %[^\t\n]s",string);
      if(string[0]=='q')
       break;

int flag=0,i,len=strlen(string);

for(i=0;i<len;i++)
{

if(string[i]==',')
flag++;
}

if(flag==0)
printf("Error: No comma in string.\n\n");
else if(flag==1)
{
i=0;
while(string[i]!=',')
{
if(string[i]!=' ')
first_word[i]=string[i];
i++;
}
first_word[i]='\0';
i++;
int j=0;
   while(string[i]!='\0')
{
if(string[i]!=' ')
{
        second_word[j]=string[i];
     j++;
   }
  i++; }
second_word[j]='\0';
printf("First word: %s\n",first_word);
printf("Second word: %s\n\n",second_word);

   }
  }
}
