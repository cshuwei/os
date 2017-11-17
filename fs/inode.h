#ifndef __INODE_H
#define __INODE_H
#include "stdint.h"
#include "list.h"

struct inode {
    uint32_t i_no;
    uint32_t i_size;
    uint32_t i_open_cnts;  
    bool write_deny;
    uint32_t i_sectors[13];
    struct list_elem inode_tag;    
};
#endif
