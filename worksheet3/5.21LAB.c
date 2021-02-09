/*
 * Code by Matthew Zuberbuhler & Sergio Gutierrez 
 */
#include <stdio.h>
#include <string.h>

int main(void) {
	const int NUM_ELEMENTS = 21;         // Number of input integers
	char userVals[NUM_ELEMENTS][NUM_ELEMENTS]; 
	char tmpStr[20];
	char ch;
	int count;
	int wordCount;

	scanf("%i", &count);

	for (int i = 0; i < count + 1; i++) 
	{
			scanf("%s", tmpStr);
			strcpy(userVals[i], tmpStr);
	}
	ch = userVals[count][0];
   //	printf("letter: %c\n", ch);

	for (int i = 0; i < count; i++) 
	{
		for(int j = 0; userVals[i][j]; j++) {
			if(userVals[i][j] == ch) {
				printf("%s,", userVals[i]);
				break;
			}
		}
	}
	//printf("\n");
	return 0;
}