/*
 * mysem.c
 *
 *  Created on: Nov 12, 2017
 *      Author: witty
 */

#include <stdio.h>
#include <stdlib.h>
#include "mysem.h"
#include "thread_handler.h"
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
/* The two macros are extremely useful by turnning on/off interrupts when atomicity is required */
#define DISABLE_INTERRUPTS() {  \
    asm("wrctl status, zero");  \
}

#define ENABLE_INTERRUPTS() {   \
    asm("movi et, 1");          \
    asm("wrctl status, et");    \
}

// A function to enqueue data to the queue
void dataEnqueue(void *data, bQueue *queue)
{
    bElement *element;

    if ((element = (bElement *)malloc(sizeof(bElement))) == NULL)
    {
        printf("Unable to allocate space!\n");
        exit(1);
    }
    element->data = data;
    element->next = NULL;

    if (queue->head == NULL)
        queue->head = element;
    else
        queue->tail->next = element;
    queue->tail = element;

    queue->size++;
}

// A function to dequeue data from the queue
void *dataDequeue(bQueue *queue)
{
    bElement *element;
    void *data = NULL;

    if (queue->size != 0)
    {
        element = queue->head;
        if (queue->size == 1)
            queue->tail = NULL;
        queue->head = queue->head->next;

        queue->size--;
        data = element->data;
        free(element);
    }

    return data;
}

// A function that initializes semaphore
int semInit(semaphore * sem, int semVal)
{
	sem = (semaphore *)malloc(sizeof(semaphore));
	
	// Gets the initial value of the semaphore
	sem->value = semVal;
	
	// The initial number of sleeping threads on a semaphore is 0	
	sem->threadCount = 0;
	
	sem->aQueue = (bQueue *)malloc(sizeof(bQueue));
	sem->aQueue->head = NULL;
	sem->aQueue->tail = NULL;
	sem->aQueue->size = 0;
	
	return 1;
}

// A function to lock the semaphore
void semDown(semaphore * sem)
{
	DISABLE_INTERRUPTS();
	// If semaphore value is greater than 0, then decrement it by 1 and continue.
	if (semValue(sem) > 0){
		sem->value--;
		ENABLE_INTERRUPTS();
	}
	
	// Else block the thread
	else{
		while (semThreadCount(sem) > 0){
			tcb *thread = get_current_thread();
			mythread_block(thread);
			sem->threadCount++;
			dataEnqueue(thread, sem->aQueue);
			ENABLE_INTERRUPTS();
			
			// Burn quantum. Keep blocking the thread.
			while (thread->state == BLOCKED);
		}
	}
}

// A function that releases the semaphore
void semUp(semaphore * sem)
{
	DISABLE_INTERRUPTS();
	sem->value++;
	
	// Release all the threads
	while (semThreadCount(sem) > 0){
		tcb *thread = (tcb *) dataDequeue(sem->aQueue);
		mythread_start(thread);
		sem->threadCount--;
	}
	ENABLE_INTERRUPTS();
}

// Returns the current value of the semaphore.
unsigned int semValue(semaphore * sem)
{
	return sem->value;
}

// Returns the number of threads sleeping on the semaphore.
unsigned int semThreadCount(semaphore * sem)
{
	return sem->threadCount;
}
