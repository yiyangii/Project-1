Yi Yang / Section 3

This program is a discrete event simulation model be designed to understand how a single-core processor completes the virtualization 
of a multi-task scenario. First, let's assume that each job takes the same amount of time and that all jobs take the same time to reach 
the CPU. In our process system, only one event can be processed at a time and each event needs to go through arrival, use and exit. If 
the job being determined not to go to the CPU then it will be add into network queue.

Events will be written to the CPU one by one. When the CPU is occupied, the next job in the FIFO queue will be determined to enter Disk 
1 to perform I/O or exit. The determining factor in this situation will depend on the QUIT_PROB in Confi.txt. When disk 1 is occupied, repeat 
the previous steps to add job to disk 2. In this program, there are two queues for processing jobs, one is the CPU and the other is the disk. 
The disk queue is designed to process jobs that cannot be run due to CPU usage. When processing the program queue, the FIFO queue will be 
applied to the CPU and Disk 1/2. The second queue will use priority queue or link list to place all events and record their running time 
and arrival time. 

The arrival time, start time and end time of each event will be determined by Config.txt and the file will be read when the program starts
 and applied to the entire program. Give the queue memory through malloc() at the beginning of the program. The queue will check whether 
there is at least one element in the queue, if not, it will start adding new events to the priority queue and write the first event added 
to the CPU as the first.After the first event is over, use pop to delete it from the queue and write it to the event queue. Use free() to 
release the memory after all events in the FIFO queue are completed.Heap priority as an event queue can place the least used event first in
the queue and sort the events in the queue from small to large. Each event will enter the event queue after it is completed or determined exit 
the system. In order to allow the program to keep adding events to the queue, the while loop will check whether the event queue is empty or 
whether there are still events running in the system. 

Create the event "simulation start" at the beginning of the simulation and create the "simulation end" at the end and start giving the first 
event a random arrival time. The arrival time will be generated from ARRIVAL_MIN-ARRIVAL_MAX, and then QUIT_PROB will decide whether to enter 
the CPU work . At this time, Job1 may enter the network queue and CPU queue. When Job1 enters the CPU and starts to work, it will be assigned 
a Job finish time in JOB_MIN-JOB_MAX. After Job1 enters the CPU, Job2 will start to repeat the process experienced by Job1, and during this 
period the priority queue will continue to generate two jobs and the arrival time and completion time of job1 and job2 will be recorded and 
print to the screen. After the job is over, each job will pop out from the FIFO queue.When a job finishes its work, pop out it from the priority 
queue and change its status to "Finished". After the while loop detects that there is no job in the priority queue and CPU, it will stop the loop 
and print the work process of each program in the priority queue.

	






