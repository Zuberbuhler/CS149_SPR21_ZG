/* Code by Matthew Zuberbuhler & Sergio Gutierrez CS149 SPR21*/

#include <stdio.h>
#include <stdlib.h>
#include "ContactNode.h"

ContactNode CreateContactNode(struct ContactNode* contact, char name[50], char number[50], ContactNode* address) {
    strcpy(contact->contactName, name);
    strcpy(contact->contactPhoneNum, number);
    contact->nextNodePtr=address;
    return *contact;
}
void InsertContactAfter(ContactNode* contact, ContactNode* nextcontact) {
    ContactNode* temp = NULL;
    temp = contact->nextNodePtr;
    contact->nextNodePtr = nextcontact;
    nextcontact->nextNodePtr = temp;
    return;
}
ContactNode* GetNextContact(ContactNode* contact) {
    return contact->nextNodePtr;
}
void PrintContactNode(const ContactNode* contact) {
    printf("Name: %s\n", contact->contactName);
    printf("Phone number: %s\n", contact->contactPhoneNum);
    return;
}