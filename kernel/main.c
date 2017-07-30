#include "print.h"
#include "init.h"
#include "thread.h"

void k_thread_hello(void*);
void k_thread_bye(void*);
void main(void) {
   put_str("I am kernel\n");
   init_all();

   thread_start("k_thread_hello", 31, k_thread_hello, "hello_world");
   thread_start("k_thread_bye", 31, k_thread_bye, "bye_world");
   int_enable();
   while(1)
   	{
   		put_str("I am Main thread");
   	}
}

void k_thread_hello(void* arg) {     

   char* para = arg;
   while(1) {
      put_str(para);
   }
}

void k_thread_bye(void* arg) {     

   char* para = arg;
   while(1) {
      put_str(para);
   }
}
