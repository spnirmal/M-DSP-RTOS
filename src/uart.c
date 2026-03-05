#include <stdio.h>
#include <stdlib.h>

#define SIZE 10

typedef struct Node{
    int key;
    int value;
    struct Node *next;
}Node;

Node* table[SIZE];

int hash(int key){
    return key % SIZE;
}

void insert(int key, int value){

    int index = hash(key);

    Node *newNode = malloc(sizeof(Node));
    newNode->key = key;
    newNode->value = value;
    newNode->next = table[index];

    table[index] = newNode;
}

Node* search(int key){

    int index = hash(key);
    Node *curr = table[index];

    while(curr){
        if(curr->key == key)
            return curr;

        curr = curr->next;
    }

    return NULL;
}

int main(){

    insert(10,100);
    insert(20,200);
    insert(30,300);

    Node *n = search(20);

    if(n)
        printf("%d\n",n->value);
}