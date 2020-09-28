// A C program to demonstrate linked list based implementation of queue
#include <stdio.h>
#include <stdlib.h>
#include "Fifo.h"


// A utility function to create a new linked list node.
struct QNode *newNode(struct Event *newEvent) {
    struct QNode *temp = (struct QNode *) malloc(sizeof(struct QNode));
    temp->event = newEvent;
    temp->next = NULL;
    return temp;
}

// A utility function to create an empty queue
struct Queue *createQueue(int size) {
    struct Queue *q = (struct Queue *) malloc(sizeof(struct Queue));
    q->front = q->rear = NULL;
    q->QSize = size;
    return q;
}




// The function to add a key k to q
void enQueue(struct Queue *q, struct Event *k) {

   
    // Create a new LL node
    struct QNode *nextNode = newNode(k);

    // If queue is empty, then new node is front and rear both
    if (q->rear == NULL)
    {
        q->front = q->rear = nextNode;
        q->QSize++;
        return;
    }

    //if new person priority equal or smaller than head node, we should make it first node
    if(nextNode->event->runtime <= q->front->event->runtime)
    {
        struct QNode* temp;
        temp = q->front;
        q->front = nextNode;
        nextNode->next = temp;
        q->QSize++;
        return;
    }

    //if new persons priority is equal or bigger than tail node, we should make it tail.
    if (nextNode->event->runtime >= q->rear->event->runtime)
    {
        // Add the new node at the end of queue and change rear
        q->rear->next = nextNode;
        q->rear = nextNode;
        q->QSize++;
        return;
    }

    //this is general algoritm for priority queues
    struct QNode* temp = q->front;
    while(temp->next != NULL)
    {
        if( nextNode->event->runtime < temp->next->event->runtime)
        {
            struct QNode* newy;
            newy = temp->next;
            temp->next = nextNode;
            nextNode->next = newy;
            q->QSize++;
            break;
        }
        temp = temp->next;
    }
    
}

// Function to remove a key from given queue q
struct QNode *deQueue(struct Queue *q) {

 
    // If queue is empty, return NULL.
    if (q->front == NULL)
        return NULL;

    // Store previous front and move front one node ahead
    struct QNode *temp = q->front;
    q->front = q->front->next;

    // If front becomes NULL, then change rear also as NULL
    if (q->front == NULL)
        q->rear = NULL;

    q->QSize--;

    
    return temp;
}

struct Event *GetDataFromNode(struct QNode *node) {
    struct Event *event = NULL;
    if (node) {
        event = node->event;
    }
    return event;
}