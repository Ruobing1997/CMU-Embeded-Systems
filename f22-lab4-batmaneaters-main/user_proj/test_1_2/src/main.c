/** @file   test_1_2/main.c
 *
 *  @brief  user-space project "test_1_2", demonstrates enforcement of compute times C_i
 *  @note   Not for public release, do not share
 *
 *  @date   last modified 25 October 2022
 *  @author CMU 14-642
 *
 *  @output self-expanatory success/failure messages
**/

#include <lib642.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/** @brief thread user space stack size - 1KB */
#define USR_STACK_WORDS 256
#define NUM_THREADS 1
#define NUM_MUTEXES 0
#define CLOCK_FREQUENCY 1000

/** @brief Computation time of the task */
#define THREAD_C_MS 123
// #define THREAD_C_MS 31
/** @brief Period of the thread */
#define THREAD_T_MS 1024
// #define THREAD_T_MS 256

void thread_0(UNUSED void* vargp) {
	int cnt = 0;

	while(1) {
		int t = get_time();

		if(t < THREAD_T_MS * cnt) {
			printf("Failed. Should have woken up after t = %d\n", THREAD_T_MS * cnt);
			printf("Woke up at t = %d\n", t);
			while(1);
		}

		print_num_status_cnt(0, cnt++);

		if(cnt > 20) {
			printf("Test passed!\n");
			while(1);
		}

		printf("Should only wake up after t = %d.\n", THREAD_T_MS * cnt);
		spin_until(THREAD_T_MS * (cnt - 1) + THREAD_C_MS + 1);

	}
}

void idle_thread() {
	while(1);
}


int main(UNUSED int argc, UNUSED char* const argv[]) {

	ABORT_ON_ERROR(thread_init(NUM_THREADS, USR_STACK_WORDS, &idle_thread, KERNEL_ONLY, NUM_MUTEXES));

	ABORT_ON_ERROR(thread_create(&thread_0, 0, THREAD_C_MS, THREAD_T_MS, NULL));

	printf("Successfully created threads! Starting scheduler...\n");
	ABORT_ON_ERROR(scheduler_start(CLOCK_FREQUENCY));

	return 0;

}
