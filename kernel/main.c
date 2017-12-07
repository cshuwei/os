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
#include "fs.h"
#include "file.h"
/*
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
//    console_put_str(" main_pid:0x");
//    console_put_int(sys_getpid());
//    console_put_char('\n');
	thread_start("k_thread_a", 31, k_thread_a, "argA ");
	thread_start("k_thread_a", 31, k_thread_b, "argB ");
 //   uint32_t fd = sys_open("/file2", O_RDWR);
 //   printf("fd:%d\n", fd);
 //   sys_write(fd, "hello, world\n", 12);
 //   sys_close(fd);
 //   printf("%d closed now\n", fd);    
 //  
    uint32_t fd = sys_open("/file1", O_RDWR);
    printf("open /file1, fd:%d\n", fd);
    char  buf[64] = {0};
    int read_bytes = sys_read(fd, buf, 18);
    printf("1_read %d bytes:\n%s", read_bytes, buf);

    memset(buf, 0, 64);
    read_bytes = sys_read(fd, buf, 6);
    printf("2_read %d bytes:\n%s", read_bytes, buf);

    memset(buf, 0, 64);
    read_bytes = sys_read(fd, buf, 6);
    printf("3_read %d bytes:\n%s", read_bytes, buf);
    
    printf("_____ close file1 and reopen ______\n");
    sys_close(fd);
    fd = sys_open("/file1", O_RDWR);
    memset(buf, 0, 64);
    read_bytes = sys_read(fd, buf, 24);
    printf("4_read %d bytes:\n%s", read_bytes, buf);
    sys_close(fd);

//	printf("/file1 delete %s!\n", sys_unlink("/file1") == 0 ? "done" : "fail");
    printf("/dir1/subdir1 create %s!\n", sys_mkdir("/dir1/subdir1") == 0 ? "done" : "fail");
    printf("/dir1 create %s\n", sys_mkdir("/dir1") == 0 ? "done" : "fail");
    printf("now, /dir1/subdir1 create %s!\n", sys_mkdir("/dir1/subdir1") == 0 ? "done" : "fail");
    int fd = sys_open("/dir1/subdir1/file2", O_CREAT | O_RDWR);
    if (fd != -1) {
        printf("/dir1/subdir1/file2 create done!\n");
        sys_write(fd, "Catch me if you can!\n", 21);
        sys_lseek(fd, 0, SEEK_SET);
        char buf[32] = {0};
        sys_read(fd, buf, 21);
        printf("/dir1/subdir1/file2 says:\n%s", buf);
        sys_close(fd);
    }

    struct dir* p_dir = sys_opendir("/dir1/subdir1");
    if (p_dir) {
        printf("/dir1/subdir1 open done!\n");
        if (sys_closedir(p_dir) == 0) {
            printf("/dir1/subdir1 close done!\n");
        } else {
            printf("/dir1/subdir1 close fail!\n");
        }
    } else {
        printf("/dir1/subdir1 open fail!\n");
    }
    while(1);//{ 
//        	console_put_str("MAIN ");
//	}
	return 0;
}

void k_thread_a(void* arg) {
    void* addr1 = sys_malloc(256);
    void* addr2 = sys_malloc(255);
    void* addr3 = sys_malloc(254);
    console_put_str(" thread_b malloc addr:0x");
    console_put_int((int)addr1);
    console_put_char(',');
    console_put_int((int)addr2);
    console_put_char(',');
    console_put_int((int)addr3);
    console_put_char('\n');
    int cpu_delay = 1000000;
    while(cpu_delay-- > 0);
    sys_free(addr1);
    sys_free(addr2);
    sys_free(addr3);
    while(1);
}

void k_thread_b(void* arg) {
    void* addr1 = sys_malloc(256);
    void* addr2 = sys_malloc(255);
    void* addr3 = sys_malloc(254);
    console_put_str(" thread_b malloc addr:0x");
    console_put_int((int)addr1);
    console_put_char(',');
    console_put_int((int)addr2);
    console_put_char(',');
    console_put_int((int)addr3);
    console_put_char('\n');
    int cpu_delay = 1000000;
    while(cpu_delay-- > 0);
    sys_free(addr1);
    sys_free(addr2);
    sys_free(addr3);
	while(1); 
}
void u_prog_a(void) {
    void* addr1 = malloc(256);
    void* addr2 = malloc(255);
    void* addr3 = malloc(254);
    printf(" prog_a malloc addr:0x%x, 0x%x, 0x%x\n", (int)addr1, (int)addr2, (int)addr3);
    int cpu_delay = 100000;
    while(cpu_delay-- > 0);
    free(addr1);
    free(addr2);
    free(addr3);
    while(1);
}

void u_prog_b(void) {
    void* addr1 = malloc(256);
    void* addr2 = malloc(255);
    void* addr3 = malloc(254);
    printf(" prog_a malloc addr:0x%x, 0x%x, 0x%x\n", (int)addr1, (int)addr2, (int)addr3);
    int cpu_delay = 100000;
    while(cpu_delay-- > 0);
    free(addr1);
    free(addr2);
    free(addr3);
    while(1);
}
*/

int main(void) {
    put_str("I am kernel\n");
    init_all();
/*    char cwd_buf[32] = {0};
    sys_getcwd(cwd_buf, 32);
    printf("cwd:%s\n", cwd_buf);
    sys_chdir("/dir1");
    printf("change cwd now\n");
    sys_getcwd(cwd_buf, 32);
    printf("cwd:%s\n", cwd_buf);
*/
    struct stat obj_stat;
    sys_stat("/", &obj_stat);
    printf("/`s info\n  i_no:%d\n   size:%d\n   filetype:%s\n",\
            obj_stat.st_ino, obj_stat.st_size,\
            obj_stat.st_filetype == 2 ? "directory" : "regular");
    sys_stat("/dir1", &obj_stat);
    printf("/dir1`s info\n  i_no:%d\n   size:%d\n   filetype:%s\n",\
            obj_stat.st_ino, obj_stat.st_size,\
            obj_stat.st_filetype == 2 ? "directory" : "regular");
    while(1);
    return 0;   
}
