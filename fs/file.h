#ifndef __FS_FILE_H
#define __FS_FILE_H
#include "stdint.h"
#include "ide.h"
#include "dir.h"
#include "global.h"

struct file {
    uint32_t fd_pos;
    uint32_t fd_flag;
    struct inode* fd_inode;
};

enum std_fd {
    stdin_no,
    stdout_no,
    stderr_no
};


enum bitmap_type {
    INODE_BITMAP,
    BLOCK_BITMAP
};

#define MAX_FILE_OPEN   32
extern struct file file_table[MAX_FILE_OPEN];
int32_t inode_bitmap_alloc(struct partition* part);
int32_t block_bitmap_alloc(struct partition* part);
int32_t file_create(struct dir* parent_dir, char* filename, uint8_t flag);
void bitmap_sync(struct partition* part, uint32_t bit_idx, uint8_t btmp);
int32_t get_free_slot_in_global(void);
int32_t pcb_fd_install(int32_t globa_fd_idx);
int32_t file_close(struct file* file);
int32_t file_write(struct file* file, const void* buf, uint32_t count);
int32_t file_open(uint32_t inode_no, uint8_t flag);
int32_t file_read(struct file* file, void* buf, uint32_t count);
#endif
