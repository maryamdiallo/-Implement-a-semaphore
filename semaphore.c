#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"
#include"semaphore.h"

void
sem_init(struct semaphore *s, int value)
{
	initlock(&s->lock, "semaphore");
	s->value = value;
}

void
sem_wait(struct semaphore *s)
{
	acquire(&s->lock);

	while (s->value == 0) {
		sleep_prepare(s);
		release(&s->lock);
		sleep();
		acquire(&s->lock);
	}

	s->value--;
	release(&s->lock);
}

void
sem_signal(struct semaphore *s)
{
	acquire(&s->lock);
	s->value++;
	wakeup(s);
	release(&s->lock);
}


#define NSEM 16

static struct semaphore table[NSEM];
static int used[NSEM];
static struct spinlock tablelock;

void
sem_table_init(void)
{
	initlock(&tablelock, "semtable");

	for (int i = 0; i < NSEM; i++) {
		used[i] = 0;
		sem_init(&table[i], 0);
	}
}

int
sem_create(int value)
{
	if (value < 0)
		return -1;
	acquire(&tablelock);

	for (int i = 0; i < NSEM; i++) {
		if (used[i] == 0) {
			used[i] = 1;

			acquire(&table[i].lock);
			table[i].value = value;
			release(&table[i].lock);

			release(&tablelock);
			return i;
		} 
	}

	release(&tablelock);
	return -1;
}

int
sem_signal_id(int id)
{
	if (id < 0 || id >= NSEM)
		return -1;
	
	acquire(&tablelock);

	if (used[id] == 0) {
		release(&tablelock);
		return -1;
	}

	release(&tablelock);
	sem_signal(&table[id]);
	return 0;
}

int
sem_wait_id(int id)
{
	if (id < 0 || id >= NSEM)
		return -1;

	acquire(&tablelock);

	if (used[id] == 0) {
		release(&tablelock);
		return -1;
	}

	release(&tablelock);
	sem_wait(&table[id]);
	return 0;
}




