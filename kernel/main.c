#include "print.h"
#include "init.h"
#include "debug.h"
#include "thread.h"
#include "interrupt.h"
#include "console.h"
#include "process.h"
#include "syscall-init.h"
#include "syscall.h"
#include "stdio.h"
void k_thread_a(void* arg);
void k_thread_b(void* arg);
void u_prog_a(void);
void u_prog_b(void);


int prog_a_pid = 0, prog_b_pid = 0;
int main(void){
	put_str("Hello, I'm kernel.\n");
	init_all();
//	asm volatile("sti");
  
	process_execute(u_prog_a, "user_prog_a");
    process_execute(u_prog_b, "user_prog_b");
	intr_enable();
    console_put_str(" main_pid:0x");
    console_put_int(sys_getpid());
    console_put_char('\n');
	thread_start("k_thread_a", 31, k_thread_a, "argA ");
	thread_start("k_thread_a", 31, k_thread_b, "argB ");
    
	while(1);//{ 
//        	console_put_str("MAIN ");
//	}
	return 0;
}

void k_thread_a(void* arg) {
    char* para = arg;
    console_put_str(" thread_a_pid:0x");
    console_put_int(sys_getpid());
    console_put_char('\n');
	while(1); 
}

void k_thread_b(void* arg) {
    char* para = arg;
    console_put_str(" thread_b_pid:0x");
    console_put_int(sys_getpid());
    console_put_char('\n');
	while(1); 
}
void u_prog_a(void) {
    char* name = "prog_a";
    printf("I am %s, my a_pid:%d%c", name, getpid(), '\n');
    while(1);       
}

void u_prog_b(void) {
    char* name = "prog_b";
    printf("I am %s, my b_pid:%d%c", name, getpid(), '\n');
    while(1);
}

