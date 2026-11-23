#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{
	int sem = sem_init(1);

	if (sem < 0) {
		printf("Semaphore creation failed\n");
		exit(1);
	}

	printf("Semaphore created: %d\n", sem);

	if (fork() == 0) {
		sem_wait(sem);
		printf("Child entered critical section\n");
		pause(20);
		printf("Child leaving critical section\n");
		sem_signal(sem);
		exit(0);
	}

	sem_wait(sem);
	printf("Parent entered critical section\n");
	pause(20);
	printf("Parent leaving critical section\n");
	sem_signal(sem);

	wait(0);
	printf("Semaphore test completed\n");
	exit(0);
}

