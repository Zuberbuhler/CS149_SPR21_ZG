/* Code by Matthew Zuberbuhler & Sergio Gutierrez CS149 SPR21*/

#include <string.h>
typedef struct ContactNode {
    char contactName[50];
    char contactPhoneNum[50];
    struct ContactNode* nextNodePtr;
}ContactNode;

ContactNode CreateContactNode(struct ContactNode* contact, char name[], char number[], ContactNode* address);

void InsertContactAfter(ContactNode* contact, ContactNode* nextcontact);

ContactNode* GetNextContact(ContactNode* contact);

void PrintContactNode(const ContactNode* contact);