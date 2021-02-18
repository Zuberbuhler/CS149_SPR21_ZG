/*
 * Code by Matthew Zuberbuhler & Sergio Gutierrez 
 */
#include <stdio.h>

void swap(int *xp, int *yp) 
{ 
    int temp = *xp; 
    *xp = *yp; 
    *yp = temp; 
} 

void SortArray(int sortingList[], int numVals) 
{
   //bubble
   int i, j; 
   for (i = 0; i < numVals-1; i++)       
  
       // Last i elements are already in place    
       for (j = 0; j < numVals-i-1; j++)  
           if (sortingList[j] > sortingList[j+1]) 
              swap(&sortingList[j], &sortingList[j+1]); 

}

int main(void) {

   int size;
   scanf("%i", &size);
   int arr[size];
   for(int i = 0; i < size; i++) 
   {
      scanf("%i", &arr[i]);
   }
   SortArray(arr, size);
   for(int i = 0; i < size; i++) 
   {
      printf("%i ", arr[i]);
   }
   
   printf("\n");
   return 0;
}
