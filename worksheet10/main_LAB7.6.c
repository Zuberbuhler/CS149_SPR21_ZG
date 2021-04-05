#include<stdio.h>
#include<string.h>

#include "ItemToPurchase.h"

int main(void) {
   
   struct ItemToPurchase item1;
   struct ItemToPurchase item2;
   
   printf("Item 1\n");
   printf("Enter the item name:\n");
   scanf("%[^\n]%*c", item1.itemName);
   printf("Enter the item price:\n");
   scanf("%i", &item1.itemPrice);
   printf("Enter the item quantity:\n");
   scanf("%i", &item1.itemQuantity);
   
   char c;
   c = getchar();
   while(c != '\n' && c != EOF) {
      c = getchar();
   }
   printf("\nItem 2\n");
   printf("Enter the item name:\n");
   scanf("%[^\n]%*c", item2.itemName);
   printf("Enter the item price:\n");
   scanf("%i", &item2.itemPrice);
   printf("Enter the item quantity:\n");
   scanf("%i", &item2.itemQuantity);
   
   printf("\nTOTAL COST\n");
   PrintItemCost(&item1);
   PrintItemCost(&item2);
   printf("\nTotal: $%i\n", (item1.itemPrice * item1.itemQuantity) + (item2.itemPrice * item2.itemQuantity));
   
   return 0;
}
