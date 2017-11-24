#ifndef __FS_DIR_H
#define __FS_DIR_H
#include "stdint.h"
#include "inode.h"
#include "ide.h"
#include "global.h"
#include "fs.h"
#define MAX_FILE_NAME_LEN  16   

struct dir {
   struct inode* inode;   
   uint32_t dir_pos;    
   uint8_t dir_buf[512]; 
};

struct dir_entry {
   char filename[MAX_FILE_NAME_LEN]; 
   uint32_t i_no;            
   enum file_types f_type;  
};

extern struct dir root_dir;           
void open_root_dir(struct partition* part);
struct dir* dir_open(struct partition* part, uint32_t inode_no);
void dir_close(struct dir* dir);
bool search_dir_entry(struct partition* part, struct dir* pdir, const char* name, struct dir_entry* dir_e);
void create_dir_entry(char* filename, uint32_t inode_no, uint8_t file_type, struct dir_entry* p_de);
bool sync_dir_entry(struct dir* parent_dir, struct dir_entry* p_de, void* io_buf);
#endif
