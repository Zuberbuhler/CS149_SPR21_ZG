/* Code by Matthew Zuberbuhler & Sergio Gutierrez CS149 SPR21*/

#include <stdio.h>
#include <stdlib.h>
#include "ContactNode.h"

int main(int argc, char* argv[]) {
    ContactNode* contact[3] = {NULL, NULL, NULL};
    for(int i=0; i<=2; i++) {
        ContactNode n;
        printf("Person %d\n", i+1);
        printf("Enter name:\n");
        fgets(n.contactName, 50, stdin);
        
        n.contactName[strlen(n.contactName)-1] ='\0';
        printf("Enter phone number:\n");
        fgets(n.contactPhoneNum, 50, stdin);
        
        n.contactPhoneNum[strlen(n.contactPhoneNum)-0] ='\0';
        printf("You entered: %s, %s\n", n.contactName, n.contactPhoneNum);
        
        contact[i] = (ContactNode*)malloc(sizeof(ContactNode));
        *contact[i] = CreateContactNode(contact[i], n.contactName, n.contactPhoneNum, NULL);
    }
    InsertContactAfter(contact[0], contact[1]);
    InsertContactAfter(contact[1], contact[2]);
    
    printf("\nCONTACT LIST\n");
    
    PrintContactNode(contact[0]);
    PrintContactNode(contact[1]);
    PrintContactNode(contact[2]);
    printf("\n");
    return 0;
}