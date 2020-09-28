#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "Fifo.h"

int cpustatus = 0;
int disk1status = 0;
int disk2status = 0;
int networkstatus = 0;
//Event have number to see which job is, status to check event is run/ finish
//Usuage of time
//job status print when 1 enter queue 2 in cpu 3 finish

//Read Config data
int seedvalue;
int inittime;
int fintime;
int arrivemin;
int arrivemax;
int quitprob;
int networkprob;
int cpumin;
int cpumax;
int disk1min;
int disk2min;
int disk1max;
int disk2max;
int networkmin;
int networkmax;
int statuscpu;
int statusdisk1;
int statusdisk2;

struct Queue *equeue = NULL;
struct Queue *cpu = NULL;
struct Queue *disk1 = NULL;
struct Queue *disk2 = NULL;
struct Queue *network = NULL;


char *typetoString(struct Event *eve, char *buffer);

void printLogHeader(FILE *logfile);

void AddNewEvent(int totaltime, struct Queue *equeue, struct Event *currentjob);


/* Move pro and Randgenerator functions above */
int randgenerator(int num1, int num2) { //get rand num between min - max
    int r;
    r = num1 + (rand() % (num2 - num1 + 1));
    return r;
}


int pro(int num) {
    int temp = randgenerator(0, 100);// get pro via compare two rand num, temp for percent, num for prob
    if (temp < num) {
        return 1;
    } else {
        return 0;
    }
}



int DetermineTimeforEvent() {
    return randgenerator(arrivemin, arrivemax);
}

int DetermineExecutionTimeForEvent_Network() {
    return randgenerator(networkmin, networkmax);
}

int DetermineExecutionTimeForEvent_Disk() {
    return randgenerator(disk1min, disk1max);

}

int DetermineExecutionTimeForEvent_CPU() {
    return randgenerator(cpumin, cpumax);
}

int isJobFinished() {
    return pro(quitprob);
}

int isJobNeedDisKProcessing() {
    return pro(networkprob);
}


int GetLeastBusyDisk(struct Queue *disk1, struct Queue *disk2) {//compare two disk to see which disk need to enter
    if (disk1->QSize > disk2->QSize)
        return DISK2ARRIVAL;
    else
        return DISK1ARRIVAL;

}


struct Event *CreateEvent(int runtime, int status, int number) {
    struct Event *event1;
    event1 = malloc(sizeof(struct Event));
    if (event1 == NULL) {
        printf("Malloc Error event!");
        exit(1);
    }
    event1->runtime = runtime;
    event1->status = status;
    event1->id = number;

    return event1;
}

int readvalue(FILE *config) {
    char buffer[100];
    int value;
    fscanf(config, "%s", buffer);//read first element point to number;
    fscanf(config, "%d", &value);
    return value;
}


char *typetoString(struct Event *eve, char *buffer) {
    memset(buffer, 0, sizeof(*buffer));
    switch (eve->status) {
        case 0:
            strcpy(buffer, "  entered the system");
            break;
        case CPUARRIVAL:
            strcpy(buffer, "  entered the cpu queue");
            break;
        case CPUENTER:
            strcpy(buffer, "  arrive cpu");
            break;
        case CPUFINISHED:
            strcpy(buffer, " finished at cpu");
            break;
        case DISK1ARRIVAL:
            strcpy(buffer, " entered the disk1 queue");
            break;
        case DISK1ENTER:
            strcpy(buffer, " arrive disk 1");
            break;
        case DISK1FINISHED:
            strcpy(buffer, " finish disk 1");
            break;
        case DISK2ARRIVAL:
            strcpy(buffer, " enter disk 2 queue");
            break;
        case DISK2ENTER:
            strcpy(buffer, " arrive disk 2");
            break;
        case DISK2FINISHED:
            strcpy(buffer, " finish disk 2");
            break;
        case NETWORKARRIVAL:
            strcpy(buffer, " enter network queue");
            break;
        case NETWORKENTER:
            strcpy(buffer, " arrive network");
            break;
        case NETWORKFINISHED:
            strcpy(buffer, " finished network");
            break;
        case END:
            break;
        default:
            printf(" Not match");
            exit(7);
            break;
    }
    return buffer;
}


int getNextTimeIncrement(struct Queue* eventQ,struct Queue *cpu, struct Queue *disk1, struct Queue *disk2, struct Queue *network)
{
    unsigned int nextEvent = 0xFFFF;
    unsigned int cpuTime = 0xFFFF;
    unsigned int disk1Time = 0xFFFF;
    unsigned int disk2Time = 0xFFFF;
    unsigned int networkTime = 0xFFFF;

    if (eventQ->QSize > 0)
        nextEvent = eventQ->front->event->runtime;
    if (cpu->QSize > 0)
        cpuTime = cpu->front->event->runtime;
    if (disk1->QSize > 0)
        disk1Time = disk1->front->event->runtime;
    if (disk2->QSize > 0)
        disk2Time = disk2->front->event->runtime;
    if (network->QSize > 0)
        networkTime = network->front->event->runtime;

    unsigned t = cpuTime > disk1Time ? disk1Time : cpuTime;
    unsigned int r = disk2Time > networkTime ? networkTime : disk2Time;
    unsigned x = t > r ? r : t;

    unsigned int nextTimeIncrement =  nextEvent > x ? x : nextEvent;

    return nextTimeIncrement == 0xFFFF ? 0 : nextTimeIncrement;
}

void ProcessCPUFinished(unsigned int totalTime, struct Event* handleEvent)
{
    struct Event* currentEvent = handleEvent;

    int doJobExit = isJobFinished();
    if (doJobExit)
    {
        free(currentEvent);
    }
    else
    {
        if (isJobNeedDisKProcessing())
        {
            currentEvent->status = GetLeastBusyDisk(disk1, disk2);
            currentEvent->runtime = totalTime + 1;
            enQueue(equeue, currentEvent);
        }
        else
        {
            currentEvent->status = NETWORKARRIVAL;
            currentEvent->runtime = totalTime + 1;
            enQueue(equeue, currentEvent);
        }
    }
}


void processCPUFifo(unsigned int totalTime, struct Queue* cpu)
{
    struct Event* currentEvent = NULL;
    if (cpu && cpu->QSize > 0)
    {
        if (totalTime >= cpu->front->event->runtime)
        {
            currentEvent  = GetDataFromNode(deQueue(cpu));
            int doJobExit = isJobFinished();
            if (doJobExit)
            {
                free(currentEvent);
            }
            else
            {
                currentEvent->status = CPUFINISHED;
                currentEvent->runtime = totalTime+1;
                enQueue(equeue, currentEvent);
            }

        }
    }
}

void processDisk1Finished(unsigned int totalTime, struct Event* disk1Finished)
{
    if (totalTime >= disk1Finished->runtime) {
        disk1Finished->status = CPUARRIVAL;
        disk1Finished->runtime = totalTime + DetermineExecutionTimeForEvent_CPU();;
        enQueue(equeue, disk1Finished);
    }
}


void processDisk1Fifo(unsigned int totalTime, struct Queue* disk1)
{
    struct Event* currentEvent = NULL;
    if (disk1 && disk1->QSize > 0) {
        if (totalTime >= disk1->front->event->runtime) {
            currentEvent = GetDataFromNode(deQueue(disk1));
            currentEvent->status = DISK1FINISHED;
            currentEvent->runtime = totalTime + 1;
            enQueue(equeue, currentEvent);
        }
    }
}


void processDisk2Finished(unsigned int totalTime, struct Event* disk2Finished)
{
    if (totalTime >= disk2Finished->runtime) {
        disk2Finished->status = CPUARRIVAL;
        disk2Finished->runtime = totalTime + DetermineExecutionTimeForEvent_CPU();
        enQueue(equeue, disk2Finished);
    }
}

void processDisk2Fifo(unsigned int totalTime, struct Queue* disk2)
{
    struct Event* currentEvent = NULL;
    if (disk2 && disk2->QSize > 0) {
        if (totalTime >= disk2->front->event->runtime) {
            currentEvent = GetDataFromNode(deQueue(disk2));
            currentEvent->status = DISK2FINISHED;
            currentEvent->runtime = totalTime + 1;
            enQueue(equeue, currentEvent);
        }
    }
}

void ProcessNetworkFinished(unsigned int totalTime, struct Event* network)
{
    if (totalTime >= network->runtime) {
        network->status = CPUARRIVAL;
        network->runtime = totalTime + DetermineExecutionTimeForEvent_CPU();
        enQueue(equeue, network);
    }
}

void processNetworkFifo(unsigned int totalTime, struct Queue* network)
{
    struct Event* currentEvent = NULL;
    if (network && network->QSize > 0) {
        if (totalTime >= network->front->event->runtime) {
            currentEvent = GetDataFromNode(deQueue(network));
            currentEvent->status = NETWORKFINISHED;
            currentEvent->runtime = totalTime + 1;
            enQueue(equeue, currentEvent);
        }
    }
}

int main() {
    char buffer[30];
    int totaltime = 0; // System time
    FILE *logfile = fopen("Log.txt", "w"); // Write file
    FILE *config = fopen("config.txt", "r");

    seedvalue = readvalue(config);
    inittime = readvalue(config);
    fintime = readvalue(config);
    arrivemin = readvalue(config);
    arrivemax = readvalue(config);
    quitprob = readvalue(config);
    networkprob = readvalue(config);
    cpumin = readvalue(config);
    cpumax = readvalue(config);
    disk1min = readvalue(config);
    disk2min = readvalue(config);
    disk1max = readvalue(config);
    disk2max = readvalue(config);
    networkmin = readvalue(config);
    networkmax = readvalue(config);
    fclose(config); // read all value end;

    printLogHeader(logfile);
    //check if the processor is having item run turn 1 when enter

    static int processId = 1;

    srand(seedvalue);

    equeue = createQueue(0);
    cpu = createQueue(0);
    disk1 = createQueue(0);
    disk2 = createQueue(0);
    network = createQueue(0);

    struct Event *firstjob = CreateEvent(inittime, CPUARRIVAL, processId++); //1 For type, 0 for status;

    enQueue(equeue, firstjob);
    

    //Logic: Get the firstjob and push into the Eventqueue
    //While programming is running then pop the first element from the CreateEventQueue
    //The first program type is 1 then push into cpu queue
    //Break from the switch to check if the cpu is available by cpustatus = 0;
    //If status is 0 then


    while ((totaltime < fintime) && (equeue->QSize != 0)) {
        struct Event *current = NULL;
        totaltime = getNextTimeIncrement(equeue, cpu, disk1, disk2, network);
        if (equeue && equeue->QSize > 0 && equeue->front->event->runtime <= totaltime)
        {
            current = GetDataFromNode(deQueue(equeue));
        }

        if (current)
        {
            switch (current->status) {
                case CPUARRIVAL:
                    current->runtime += DetermineExecutionTimeForEvent_CPU();
                    fprintf(logfile, "\n in Time: %d Job no.%d %s ", totaltime, current->id, typetoString(current, buffer));
                    enQueue(cpu, current);
                    break;
                case CPUFINISHED:
                    fprintf(logfile, "\n in Time: %d Job no.%d %s ", totaltime, current->id, typetoString(current, buffer));
                    ProcessCPUFinished(totaltime, current);
                    break;
                case DISK1ARRIVAL:
                    current->runtime += DetermineExecutionTimeForEvent_Disk();
                    fprintf(logfile, "\n in Time: %d Job no.%d %s ", totaltime, current->id, typetoString(current, buffer));
                    enQueue(disk1, current);
                    break;
                case DISK1FINISHED:
                    fprintf(logfile, "\n in Time: %d Job no.%d %s ", totaltime, current->id, typetoString(current, buffer));
                    processDisk1Finished(totaltime, current);
                case DISK2ARRIVAL:
                    current->runtime += DetermineExecutionTimeForEvent_Disk();
                    fprintf(logfile, "\n in Time: %d Job no.%d %s ", totaltime, current->id, typetoString(current, buffer));
                    enQueue(disk2, current);
                    break;
                case DISK2FINISHED:
                    fprintf(logfile, "\n in Time: %d Job no.%d %s ", totaltime, current->id, typetoString(current, buffer));
                    processDisk2Finished(totaltime, current);
                case NETWORKARRIVAL:
                    current->runtime += DetermineExecutionTimeForEvent_Network();
                    fprintf(logfile, "\n in Time: %d Job no.%d %s ", totaltime, current->id, typetoString(current, buffer));
                    enQueue(network, current);
                    break;
                case NETWORKFINISHED:
                    fprintf(logfile, "\n in Time: %d Job no.%d %s ", totaltime, current->id, typetoString(current, buffer));
                    ProcessNetworkFinished(totaltime, current);
                    break;
                case END:
                    fprintf(logfile, "\n in Time: %d Job no.%d %s ", totaltime, current->id, typetoString(current, buffer));
                    exit(0);
                default:
                    break;
            }
        }

        processCPUFifo(totaltime, cpu);
        processDisk1Fifo(totaltime, disk1);
        processDisk2Fifo(totaltime, disk2);
        processNetworkFifo(totaltime, network);
        struct Event*  newEvent =  CreateEvent(totaltime+ DetermineTimeforEvent(), CPUARRIVAL, processId++);
        enQueue(equeue, newEvent);
        fprintf(logfile, "\n in Time: %d Job no.%d %s ", totaltime, newEvent->id, typetoString(newEvent, buffer));
    }
    return 0;
}


void printLogHeader(FILE *logfile) {
    fprintf(logfile, "Assumption:\n");
    fprintf(logfile, "Start time ---%d\n", inittime);
    fprintf(logfile, "Finish time %d\n", fintime);
    fprintf(logfile, "Arrival minimum %d\n", arrivemin);
    fprintf(logfile, "Arrial maximum %d\n", arrivemax);
    fprintf(logfile, "Quit probability %d\n", quitprob);
    fprintf(logfile, "Network probability %d\n", networkprob);
    fprintf(logfile, "CPU minimum %d\n", cpumin);
    fprintf(logfile, "CPU maximum %d\n", cpumax);
    fprintf(logfile, "Disk 1 minimum %d\n", disk1min);
    fprintf(logfile, "Disk 1 maximum %d\n", disk1max);
    fprintf(logfile, "Disk 2 minimum %d\n", disk2min);
    fprintf(logfile, "Disk 2 maximum %d\n", disk2max);
    fprintf(logfile, "Network minimum %d\n", networkmin);
    fprintf(logfile, "Network maximum %d\n", networkmax);
}
