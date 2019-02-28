Name of group members: Juan Qin Ren, Edwin Low, Thien Nguyen (3 members)


Code Organization:
alarm_handler.h & alarm_handler.c
------------------------------------
Alarm code that handles interrupts which is used in project1.c

mysem.c
------------------------------------
Contains functions such as semInit (Initializes semaphore), semUp (Increases semaphore value by 1 and releases all sleeping threads), and semDown (reduces semaphore value by 1 or block threads) which are used in project1.c.

mysem.h
------------------------------------
Contains semaphore and blocking queue struct and function prototypes for semUp, semDown, and semInit.

queue.c
------------------------------------
Contains functions related to queue such as enqueue, dequeue, and getQsize which is needed in mysem.c and when initializing semaphores.

queue.h
------------------------------------
Contains a queue struct and function prototypes for the functions in queue.c

thread_handler.c
------------------------------------
Contains thread handling functions for creating, starting, joining, blocking, terminating, and getting threads where some of these functions are required in project1.c and mysem.c (semUp, semDown).

thread_handler.h
------------------------------------
Contains  a tcb struct (thread control blocks) and function prototypes for the functions in thread_handler.c

project1.c
------------------------------------
The main source code the contains the algorithm for the Sleeping Stylist problem and uses some of the functions from alarm_handler.c, thread_handler.c, and mysem.c.


How to compile and run (including setup):
1) Assuming everything is setup and good to go, at the Nios II IDE for Eclipse find and click the "Project" tab.
2) Click "Build All".
3) Once that is done, click the "Run" tab (just left of "Project" tab) and click Run Configurations.
4) Assuming everything is properly setup as shown by Professor Witty's video on Canvas, just click "Run" at the bottom right.