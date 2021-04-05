/*
 * Code by Matthew Zuberbuhler & Sergio Gutierrez
 */
#include<stdio.h>
#include<string.h>

#include "ItemToPurchase.h"

void MakeItemBlank(ItemToPurchase* item) {
   strcpy(item->itemName, "none");
   item->itemPrice = 0;
   item->itemPrice = 0;
}

void PrintItemCost(ItemToPurchase* item) {
   //Bottled Water 10 @ $1 = $10
   printf("%s %i @ $%i = $%i\n", item->itemName, item->itemQuantity, item->itemPrice, item->itemPrice * item->itemQuantity);
}
