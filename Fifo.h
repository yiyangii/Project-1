

#ifndef FIFO_FIFO_H
#define FIFO_FIFO_H

struct Event{
    int id; //event number
    int status; // store Done,run,arrive
    int runtime;
};

// A linked list (LL) node to store a queue entry
struct QNode { 
    struct Event *event;
    struct QNode* next;
};

// The queue, front stores the front node of LL and rear stores the
// last node of LL
struct Queue {
    struct QNode *front, *rear;
    int QSize;
};

struct Queue* createQueue(int size);
void enQueue(struct Queue* q, struct Event *k);
struct QNode*  deQueue(struct Queue* q);
struct Event* GetDataFromNode(struct QNode* node);

#endif //FIFO_FIFO_H
