#include "thread.h"
#include "stdint.h"
#include "string.h"
#include "global.h"
#include "memory.h"
#include "list.h"
#include "interrupt.h"
#include "debug.h"
#include "print.h"
#include "process.h"
#include "sync.h"

struct task_struct* idle_thread;
struct task_struct* main_thread;
struct list thread_ready_list;
struct list thread_all_list;
struct lock pid_lock;
static struct list_elem* thread_tag;

extern void switch_to(struct task_struct* cur, struct task_struct* next);

static void idle(void* arg /*UNUSED*/) {
    while(1) {
        thread_block(TASK_BLOCKED);
        asm volatile ("sti; hlt" : : : "memory");
    }
}

struct task_struct* running_thread() {
	uint32_t esp;
	asm ("mov %%esp, %0": "=g" (esp));
	return  (struct task_struct*) (esp & 0xfffff000);
}

static pid_t allocate_pid(void) {
    static pid_t next_pid = 0;
    lock_acquire(&pid_lock);
    next_pid++;
    lock_release(&pid_lock);
    return next_pid;
}

static void kernel_thread(thread_func* function, void* func_arg) {
	intr_enable();
	function(func_arg);
}

void thread_create(struct task_struct* pthread, thread_func* function, void* func_arg) {
	pthread->self_kstack -= sizeof(struct intr_stack);
	pthread->self_kstack -= sizeof(struct thread_stack);
	struct thread_stack* kthread_stack = (struct thread_stack*)pthread->self_kstack;
	kthread_stack->eip = kernel_thread;
	kthread_stack->function = function;
	kthread_stack->func_arg = func_arg;
	kthread_stack->ebp = kthread_stack->ebx = 0;
	kthread_stack->esi = kthread_stack->edi = 0;
}


void init_thread(struct task_struct* pthread, char* name, int prio) {
	memset(pthread, 0, sizeof(*pthread));
    pthread->pid = allocate_pid();
	strcpy(pthread->name, name);
	if (pthread == main_thread) {
		pthread->status = TASK_RUNNING;
	} else {
		pthread->status = TASK_READY;
	}
	pthread->elapsed_ticks = 0;
	pthread->ticks = prio;
	pthread->pgdir = NULL;
	pthread->priority = prio;
	pthread->self_kstack = (uint32_t*)((uint32_t)pthread + PG_SIZE);	
  
    pthread->fd_table[0] = 0;
    pthread->fd_table[1] = 1;
    pthread->fd_table[2] = 2;
    uint8_t fd_idx = 3;
    while (fd_idx < MAX_FILES_OPEN_PER_PROC) {
        pthread->fd_table[fd_idx] = -1;
        fd_idx++;
    }
    pthread->cwd_inode_nr = 0;
	pthread->stack_magic = 0x20171025;
}

struct task_struct* thread_start(char* name, int prio, thread_func function, void* func_arg) {
	struct task_struct* thread = get_kernel_pages(1);
	init_thread(thread, name, prio);
	thread_create(thread, function, func_arg);
	ASSERT(!elem_find(&thread_ready_list, &thread->general_tag));
	list_append(&thread_ready_list, &thread->general_tag);

	ASSERT(!elem_find(&thread_all_list, &thread->all_list_tag));
	list_append(&thread_all_list, &thread->all_list_tag);
	return thread;
}

static void make_main_thread(void) {
	main_thread = running_thread();
	init_thread(main_thread, "main", 31);
	ASSERT(!elem_find(&thread_all_list, &main_thread->all_list_tag));
	list_append(&thread_all_list, &main_thread->all_list_tag);
}

void schedule() {
	ASSERT(intr_get_status() == INTR_OFF);
	struct task_struct* cur = running_thread();
	if(cur->status == TASK_RUNNING) {
		ASSERT(!elem_find(&thread_ready_list, &cur->general_tag));
		list_append(&thread_ready_list, &cur->general_tag);
		cur->ticks = cur->priority;
		cur->status = TASK_READY;
	} else {
		//nothing
	}
	if (list_empty(&thread_ready_list)) {
        thread_unblock(idle_thread);
    }

	thread_tag = NULL;
	thread_tag = list_pop(&thread_ready_list);
	struct task_struct*  next = elem2entry(struct task_struct, general_tag,thread_tag);
	next->status = TASK_RUNNING;
    process_activate(next);
	switch_to(cur, next);
}

void thread_init(void) {
	put_str("thread_init start\n");
	list_init(&thread_ready_list);
	list_init(&thread_all_list);
    lock_init(&pid_lock);
	make_main_thread();
    idle_thread = thread_start("idle", 10, idle, NULL);
	put_str("thread_init done\n");
}

void thread_block(enum task_status stat) {
	ASSERT(((stat == TASK_BLOCKED) || (stat == TASK_WAITING) || (stat == TASK_HANGING)));
	enum intr_status old_status = intr_disable();
	struct task_struct* cur_thread = running_thread();
	cur_thread->status = stat;
	schedule();
	intr_set_status(old_status);
}

void thread_unblock(struct task_struct* pthread) {
	enum intr_status old_status = intr_disable();
	ASSERT(((pthread->status == TASK_BLOCKED) || (pthread->status == TASK_WAITING) || (pthread->status == TASK_HANGING)));
	if (pthread->status != TASK_READY) {
		ASSERT(!elem_find(&thread_ready_list, &pthread->general_tag));
		if(elem_find(&thread_ready_list, &pthread->general_tag)){
			PANIC("thread_unblocked: blocked thread in ready_list\n");
		}
		list_push(&thread_ready_list, &pthread->general_tag);
		pthread->status = TASK_READY;
	}
	intr_set_status(old_status);
}


void thread_yield(void) {
    struct task_struct* cur = running_thread();
    enum intr_status old_status = intr_disable();
    ASSERT(!elem_find(&thread_ready_list,&cur->general_tag));
    list_append(&thread_ready_list, &cur->general_tag);
    cur->status = TASK_READY;
    schedule();
    intr_set_status(old_status);
}	



