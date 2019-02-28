/*
 * mysem.h
 *
 *  Created on: Nov 12, 2017
 *      Author: witty
 */

#ifndef MYSEM_H_
#define MYSEM_H_

typedef struct blockElement{
	void *data;
	struct blockElement *next;
} bElement;

typedef struct blockQueue{
	bElement *head;
	bElement *tail;
	unsigned int size;
} bQueue;

struct mySem{
	int threadCount;
	int value;
	/* You need to add your blocking queue in this struct. */
	bQueue *aQueue;
};

typedef struct mySem semaphore;

int semInit(semaphore *, int semValue);
void semDown(semaphore *);
void semUp(semaphore *);
unsigned int semValue(semaphore *);
unsigned int semThreadCount(semaphore *);


#endif /* MYSEM_H_ */
