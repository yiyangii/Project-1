/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int cpustatus = 0;
int disk1status = 0;
int disk2status = 0;
int networkstatus = 0;
//Event have number to see which job is, status to check event is run/ finish
//Usuage of time
//job status print when 1 enter queue 2 in cpu 3 finish
#define CPUARRIVAL 1;
#define CPUENTER 2;
#define CPUFINISHED 3;
#define DISK1ARRIVAL 4;
#define DISK1ENTER 5;
#define DISK1FINISHED 6;
#define DISK2ARRIVAL 7;
#define DISK2ENTER 8;
#define DISK2FINISHED 9;
#define NETWORKARRIVAL 10;
#define NETWORKENTER 11;
#define NETWORKFINISHED 12;
#define END 13;
//Read Config data 
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

typedef struct{
    int id; //event number    
    int status; // store Done,run,exit
    int runtime;   
}Event;
//FIFO queue have size, event
//data structure first,last
typedef struct{
    Event *eventlist;
    int size;
    int firstelement;
    int lastelement;
}FIFOQueue;
//priority queue have a list of event
//size
//constructor event,queue,priority queue 
//method push,pop,heap,
typedef struct{
    int size;
    Event *events;
    int capacity;
}Eventqueue;

Event *CreateEvent(int runtime, int status, int number){
    Event* event1;
    event1 = malloc(sizeof(Event));
    if(event1 == NULL){
        printf("Malloc Error event!");
        exit(1);
    }
    event1-> runtime = runtime;
    event1-> status = status;
    event1-> id = number;
    
    return event1;
}
FIFOQueue *CreateFIFO(){
    FIFOQueue *fqueue;
    fqueue = malloc(sizeof(FIFOQueue));
    if(fqueue == NULL){
        printf("Queue Error!");
        exit(2);
    }
    fqueue -> eventlist = malloc(500 * sizeof(Event));
    if(fqueue -> eventlist == NULL){
        printf("Queuelist Error!");
        exit(2);
    }
    fqueue-> firstelement = 0;
    fqueue-> lastelement = 0;
    fqueue-> size = 0; 
    return fqueue;
}

Eventqueue *CreateEventQueue(int cap){
    Eventqueue *equeue;
    equeue = malloc(sizeof(Eventqueue));
    if(equeue == NULL){
        printf("Malloc Error Eventqueue!");
        exit(3);
    }   
    equeue -> events = malloc(sizeof(Event));
    if(equeue -> events == NULL){
        printf("Malloc Error Events!");
        exit(4);
    }   
    equeue -> size = 0;
    equeue -> capacity = cap;
    return equeue;
}
void Qpushitem(FIFOQueue *fqueue, Event *event1){
    fqueue -> eventlist[(fqueue -> lastelement)%500] = *event1;
    fqueue -> lastelement++;
    fqueue -> size++;
    
}
Event *Qpop(FIFOQueue *fqueue){
    Event *popevent = &(fqueue -> eventlist[fqueue->firstelement]);
    fqueue -> firstelement++;
    fqueue -> size--;
    return popevent;
}

void EQpush(Eventqueue *equeue, Event evt){
    equeue -> events[equeue -> size] = evt;
    equeue -> size = (equeue -> size) + 1;
}
void EQpop(Eventqueue *equeue){
    Event a;
    a = equeue -> events[0];
    equeue -> events[0] = equeue -> events[(equeue -> size) - 1];
    (equeue ->size)--;
    return a;
}

void readvalue(FILE *config){
    
    char buffer[100];
    int value;
    fscanf(config,"%s",buffer);//read first element point to number;
    fscanf(config,"%d",&value);
    return value;
    
    
}
void processcpu(FILE *logfile,Event *getjob,int arrivemin,int arrivemax,int quitprob,FIFOQueue *cpu,FIFOQueue *disk1,FIFOQueue *disk2,Eventqueue *equeue,FIFOQueue *network, int networkprob){
    if (getjob -> status == 1){
        char buffer5[100];
        fprintf(logfile,"in Time: %d Job no.%d %s ", getjob -> runtime, getjob -> id, typetoString(getjob,buffer5));
        Qpushitem(cpu, getjob);
        Event *secondjob = CreateEvent(getjob->runtime + randgenerator(arrivemin,arrivemax),1,getjob->id + 1);
        EQpush(equeue, *secondjob);
        
    }else{
        cpustatus = 0;
        char buffer6 [30];
        fprintf(logfile,"in Time: %d Job no.%d %s ", getjob -> runtime, getjob -> id, typetoString(getjob,buffer6));
        if (pro(quitprob) == 1){
            if(pro(networkprob) == 1){
                Event newnet = *CreateEvent(getjob -> runtime,10, getjob->id);
                EQpush(equeue, newnet);    
            }else{
                if(disk1 -> size <= disk2 -> size){
                    Event newdisk1 = *CreateEvent(getjob -> runtime,4, getjob->id);
                    EQpush(equeue, newdisk1);
                }else{
                    Event newdisk2 = *CreateEvent(getjob -> runtime,7, getjob->id);
                    EQpush(equeue, newdisk2);
                }
            }    
        }else{
            getjob ->status = 13;
            char buffer[30];
            fprintf(logfile,  "\nTime %d Job %d%s" , getjob->runtime, getjob->id , type_to_string(getjob, buffer));
        }
        
        
    }
}
void processdisk(FILE *logfile, Event *getjob, FIFOQueue *disk1, FIFOQueue *disk2, Eventqueue *equeue ){
    if (getjob -> status == 4 ){
        Qpushitem(disk1,getjob);
        char buffer7[30];
        fprintf(logfile,"in Time: %d Job no.%d %s ", getjob -> runtime, getjob -> id, typetoString(getjob,buffer7));
    }else if(getjob -> status == 7 ){
        Qpushitem(disk2,getjob);
        char buffer8[30];
        fprintf(logfile,"in Time: %d Job no.%d %s ", getjob -> runtime, getjob -> id, typetoString(getjob,buffer8));

    }else if(getjob->status == 6){
        char buffer9[30];
        disk1status = 0;
        getjob -> status = 1;
        Qpushitem(equeue,getjob);
        fprintf(logfile,"in Time: %d Job no.%d %s ", getjob -> runtime, getjob -> id, typetoString(getjob,buffer9));

    }else{
        char buffer10[30];
        disk2status = 0;
        getjob -> status = 1;
        Qpushitem(equeue,getjob);
        fprintf(logfile,"in Time: %d Job no.%d %s ", getjob -> runtime, getjob -> id, typetoString(getjob,buffer10));
        
    }
   
}
void processnetwork(FILE *logfile, Event *getjob, Eventqueue *equeue, FIFOQueue *network) {
    if (getjob -> status == 10){
        Qpushitem(network,getjob);
        char buffer[30];
        fprintf(logfile,"in Time: %d Job no.%d %s ", getjob -> runtime, getjob -> id, typetoString(getjob,buffer));
    }else{
        networkstatus = 0;
        char buffer10[30];
        getjob -> status = 13;
        Qpushitem(equeue,getjob);
        fprintf(logfile,"in Time: %d Job no.%d %s ", getjob -> runtime, getjob -> id, typetoString(getjob,buffer10));
    }    
     
}
int pro(int num){
    int temp = randgenerator(0,100);
    if(temp < num){
        return 1;
    }else{
        return 0;
    }
}
char *typetoString(Event *eve, char *buffer){
    memset(buffer, 0, sizeof(*buffer));
    switch(eve -> status){
        case 0:
            strcpy(buffer, " entered the system");
            break;
        case 1:
            strcpy(buffer, " entered the cpu queue");
            break;
        case 2:
            strcpy(buffer, " arrive cpu");
            break;
        case 3:
            strcpy(buffer, " finished at cpu");
            break;
        case 4:
            strcpy(buffer, " entered the disk1 queue");
            break;
        case 5:
            strcpy(buffer, " arrive disk 1");
            break;
        case 6:
            strcpy(buffer, " finish disk 1");
            break;
        case 7:
            strcpy(buffer, " enter disk 2 queue");
            break;
        case 8:
            strcpy(buffer, " arrive disk 2");
            break;
        case 9:
            strcpy(buffer, " finish disk 2");
            break;
        case 10:
            strcpy(buffer, " enter network queue");
            break;
        case 11:
            strcpy(buffer, " arrive network");
            break;
        case 12:
            strcpy(buffer, " finished network");
            break;
        case 13:
            break;
        default:
            printf("Not match");
            exit(7);
            break;
        
    } 
    return buffer;
}
int randgenerator(int num1, int num2){
    int r;
    r = num1 + (rand() % (num2 - num1 + 1));
    return r;
}



int main(){
    int totaltime = 0; // System time
    FILE *logfile = fopen ("Log.txt","w"); // Write file 
    FILE *config = fopen("config.txt", "r");
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
    //check if the processor is having item run turn 1 when enter
    
    Eventqueue equeue = *CreateEventQueue(20);
    FIFOQueue cpu = *CreateFIFO();
    FIFOQueue disk1 = *CreateFIFO();
    FIFOQueue disk2 = *CreateFIFO();
    FIFOQueue network = *CreateFIFO();
    Event *firstjob = CreateEvent(inittime,1,0); //1 For type, 0 for status;
    EQpush(&equeue,*firstjob);
    Event *endjob = CreateEvent(fintime, 11, 0);
    EQpush(&equeue,*endjob);
    //Logic: Get the firstjob and push into the Eventqueue
    //While programming is running then pop the first element from the CreateEventQueue
    //The first program type is 1 then push into cpu queue
    //Break from the switch to check if the cpu is available by cpustatus = 0;
    //If status is 0 then 
    Event *getjob = (Event*)malloc(sizeof(Event));
    while ((totaltime < fintime) && (equeue.size == 0)){
        *getjob = EQpop(&equeue);
        //get the first job;
        totaltime = getjob -> runtime;
        switch(getjob -> status){
            case 1:
                processcpu(logfile,getjob,arrivemin,arrivemax,quitprob,&cpu,&disk1,&disk2,&equeue,&network,networkprob);
                break;
            case 3:
                processcpu(logfile,getjob,arrivemin,arrivemax,quitprob,&cpu,&disk1,&disk2,&equeue,&network,networkprob);
                break;
            case 5:
                processdisk1(logfile,getjob,&equeue,&disk1);
                break;
            case 7: 
                processdisk2(logfile,getjob,&equeue,&disk2);
                break;
            case 9:
                processdisk1(logfile,getjob,&equeue,&disk1);
                break;
            case 11:
                processdisk2(logfile,getjob,&equeue,&disk2);
                break;
            case 13:
                processnetwork(logfile,getjob,&equeue,&network);
                break;
            case 15:
                processnetwork(logfile,getjob,&equeue,&network);
                break;
            case 16:
                break;
        }
        //workload
        if((cpu.size == 0) && (cpustatus == 0)){
            Event *currentjob = Qpop(&cpu);
            currentjob -> status == 2;
            
            char buffer1[100];
            fprintf(logfile,"in Time: %d Job no.%d %s ", currentjob -> runtime, currentjob -> id, typetoString(currentjob,buffer1));
            
            Event *newjob = CreateEvent(totaltime + randgenerator(cpumin,cpumax),3,currentjob->id);
            EQpush(&equeue,*newjob);
            cpustatus = 1;
        }
        if((disk1.size == 0) && (disk1status == 0)){
            Event *currentjob = Qpop(&disk1);
            currentjob -> status == 5;
            
            char buffer2[100];
            fprintf(logfile,"in Time: %d Job no.%d %s ", currentjob -> runtime, currentjob -> id, typetoString(currentjob,buffer2));
            
            Event *newjob = CreateEvent(totaltime + randgenerator(disk1min,disk1max),6,currentjob->id);
            EQpush(&equeue,*newjob);
            disk1status = 1;
        }
        if((disk2.size == 0) && (disk2status == 0)){
            Event *currentjob = Qpop(&disk2);
            currentjob -> status == 8;
            
            char buffer3[100];
            fprintf(logfile,"in Time: %d Job no.%d %s ", currentjob -> runtime, currentjob -> id, typetoString(currentjob,buffer3));
            
            Event *newjob = CreateEvent(totaltime + randgenerator(disk2min,disk2max),9,currentjob->id);
            EQpush(&equeue,*newjob);
            disk2status = 1;
        }
        if((network.size == 0) && (networkstatus == 0)){
            Event *currentjob = Qpop(&network);
            currentjob -> status == 11;
            
            char buffer2[100];
            fprintf(logfile,"in Time: %d Job no.%d %s ", currentjob -> runtime, currentjob -> id, typetoString(currentjob,buffer2));
            
            Event newjob = *CreateEvent(totaltime + randgenerator(disk1min,disk1max),12,currentjob->id);
            EQpush(&equeue,newjob);
            networkstatus = 1;
        }
        /*#define CPUARRIVAL 1;
        #define CPUENTER 2;
        #define CPUFINISHED 3;
        #define DISK1ARRIVAL 4;
        #define DISK1ENTER 5;
        #define DISK1FINISHED 6;
        #define DISK2ARRIVAL 7;
        #define DISK2ENTER 8;
        #define DISK2FINISHED 9;
        #define NETWORKARRIVAL 10;
        #define NETWORKENTER 11;
        #define NETWORKFINISHED 12;
        #define END 13;*/
        return 0;
        
        
    }
    

    return 0;
}





