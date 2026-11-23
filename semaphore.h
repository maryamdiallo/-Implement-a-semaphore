#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "spinlock.h"

struct semaphore {
     int value;
     struct spinlock lock;
};

void sem_init(struct semaphore *s, int value);
void sem_wait(struct semaphore *s);
void sem_signal(struct semaphore *s);
void sem_table_init(void);
int sem_create(int value);
int sem_wait_id(int id);
int sem_signal_id(int id); 


#endif
