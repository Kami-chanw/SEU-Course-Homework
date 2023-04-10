#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "mmu.h"

#define NUM_THREADS 2
#define TARGET_COUNT_PER_THREAD 50000

volatile uint g_counter;
struct lock_t lk;

void thread(void *arg, void* arg2)
{
	int i;

	sleep(10);
	printf(1, "thread %d: started...\n", *(int*)arg);

	for (i=0; i<TARGET_COUNT_PER_THREAD; i++) {
		lock_acquire(&lk);
		g_counter++;
		lock_release(&lk);
	}
	printf(1, "thread %d: exit...\n", *(int*)arg);
	exit();
}

int main(int argc, char **argv)
{
	int i,j;
	int final_counter;
	int final_target = NUM_THREADS*TARGET_COUNT_PER_THREAD;

	lock_init(&lk);

	// Initialize counter
	g_counter = 0;

	// Set up thread stuff

	// Stacks
	void *stacks[NUM_THREADS];
	// Args
	int *args[NUM_THREADS];

	// Allocate stacks and args and make sure we have them all
	// Bail if something fails
	for (i=0; i<NUM_THREADS; i++) {
		stacks[i] = (void*) malloc(PGSIZE);
		if (!stacks[i]) {
			printf(1, "main: could not get stack for thread %d, exiting...\n");
			exit();
		}

		args[i] = (int*) malloc(4);
		if (!args[i]) {
			printf(1, "main: could not get memory (for arg) for thread %d, exiting...\n");
			exit();
		}

		*args[i] = i;
	}

	printf(1, "main: running with %d threads...\n", NUM_THREADS);

	// Start all children
	for (i=0; i<NUM_THREADS; i++) {
		int pid = clone(thread, args[i], 0,stacks[i]);
		printf(1, "main: created thread with pid %d\n", pid);
	}
	
	// Wait for all children

	for (i=0; i<NUM_THREADS; i++) {
		void *joinstack;
		join(&joinstack);
		for (j=0; j<NUM_THREADS; j++) {
			if (joinstack == stacks[i]) {
				printf(1, "main: thread %d joined...\n", i);
				break;
			}
		}

	} 

	// Check the result
	final_counter = g_counter;
	printf(1, "Final counter is %d, target is %d\n", final_counter, final_target);
	if (final_counter == final_target)
		printf(1, "TEST PASSED!\n");
	else
		printf(1, "TEST FAILED!\n");

	// Exit
	exit();
}
