#include "print.h"
#include "init.h"
#include "debug.h"
#include "thread.h"
#include "interrupt.h"
#include "console.h"
void k_thread_a(void* arg);
void k_thread_b(void* arg);
int main(void){
	put_str("Hello, I'm kernel.\n");
	init_all();
//	asm volatile("sti");
//	thread_start("k_thread_a", 31, k_thread_a, "argA");
//	thread_start("k_thread_a", 8, k_thread_b, "argB");
	
	intr_enable();
	while(1);//{ 
	//	console_put_str("MAIN ");
//	}
	return 0;
}

void k_thread_a(void* arg) {
	char * para = arg;
	while(1) {
		console_put_str(para);	
	}
}

void k_thread_b(void* arg) {
	char * para = arg;
	while(1) {
		console_put_str(para);
	}
}
