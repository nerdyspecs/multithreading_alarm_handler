#include <stdio.h>

#include "alarm_handler.h"
#include "thread_handler.h"
#include "mysem.h"

#define NUM_THREADS 15
#define CHAIRS 5

/* a delay time used to adjust the frequency of printf messages */
#define MAX 500000

int waiting = 0;
int haircut = 0;

semaphore customerAvailable, stylistAvailable, mutex;


void customer(unsigned int tid)
{
	int j;
	while(1){
		semDown(&mutex);
		if (waiting < CHAIRS){
			waiting = waiting + 1;
			//printf("C: stylistAvailable = %d.\n", semThreadCount(&stylistAvailable));
			//printf("C: customerAvailable = %d.\n", semValue(&customerAvailable));
			printf("C: %d customer(s) waiting.\n", waiting);
			semUp(&customerAvailable);
			semUp(&mutex);
			semDown(&stylistAvailable);
			printf("C: stylistAvailable = %d.\n", semThreadCount(&stylistAvailable));
			printf("C: customerAvailable = %d.\n", semValue(&customerAvailable));
			printf("Thread %d is getting a haircut (%d).\n", tid, haircut++);
			break;
		}
		else{
			semUp(&mutex);
			for (j = 0; j < MAX; j++);
			printf("Thread %d is going shopping.\n", tid);
		}
	}

}

void stylist(unsigned int tid)
{
	int j;
	while(1){
		semDown(&customerAvailable);
		semDown(&mutex);
		waiting = waiting - 1;
		semUp(&stylistAvailable);
		semUp(&mutex);
		printf("S: %d customer(s) waiting.\n", waiting);
		printf("S: customerAvailable = %d.\n", semThreadCount(&customerAvailable));
		printf("S: styllistAvaliable = %d.\n", semValue(&stylistAvailable));
		for (j = 0; j < MAX; j++);
		printf("Stylist (%d) is cutting hair.\n", tid);
		if (haircut == NUM_THREADS)
			break;
	}
}

void os_primitive()
{
    unsigned int i;
	int check = 0;
    tcb *thread_pointer;
    semInit(&customerAvailable,0);
    semInit(&stylistAvailable,0);
    semInit(&mutex,1);

    for (i = 0; i <= NUM_THREADS; i++)
    {
    	if (i == NUM_THREADS){
    		thread_pointer = mythread_create(i, 4096, stylist);
		}
    	else{
    		thread_pointer = mythread_create(i, 4096, customer);
		}
        mythread_start(thread_pointer);
        mythread_join(thread_pointer);
    }


    if ( start_alarm_succeed() )
        printf ("Start the alarm successfully\n");
    else
        printf ("Unable to start the alarm\n");

    /* an endless while loop */
    while (1)
    {
        /* delay printf for a while */
        for (i = 0; i < MAX; i++);
        printf ("My simple thread scheduler for CSE351\n");
    }
}

int main()
{
    os_primitive();
    return 0;
}
