#include "memory.h"
#include "stdint.h"
#include "print.h"

#define PG_SIZE	4096

#define MEM_BITMAP_BASE	0xc009a000

#define K_HEAP_START 0xc0100000

struct pool {
	struct bitmap pool_bitmap;
	uint32_t phy_addr_start;
	uint32_t pool_size;
};

struct pool kernel_pool, user_pool;
struct virtual_addr kernel_vaddr;

static void mem_pool_init(uint32_t all_mem) {
	put_str("   mem_pool_init start\n");
	uint32_t page_table_size = PG_SIZE * 256;
	uint32_t used_mem = page_table_size + 0x100000;
	uint32_t free_mem = all_mem - used_mem;
	uint16_t all_free_pages = free_mem / PG_SIZE;
	
	uint16_t kernel_free_pages = all_free_pages / 2;
	uint16_t user_free_pages = all_free_pages - kernel_free_pages;
	uint32_t kbm_length = kernel_free_pages / 8;
	uint32_t ubm_length = user_free_pages / 8;
	
	uint32_t kp_start = used_mem;
	uint32_t up_start = kp_start + kernel_free_pages * PG_SIZE;
	
	kernel_pool.phy_addr_start = kp_start;
	kernel_pool.pool_size = kernel_free_pages * PG_SIZE;
	kernel_pool.pool_bitmap.btmp_bytes_len = kbm_length;
	kernel_pool.pool_bitmap.bits = (void*)MEM_BITMAP_BASE;	

	user_pool.phy_addr_start = up_start;
	user_pool.pool_size = user_free_pages * PG_SIZE;
	user_pool.pool_bitmap.btmp_bytes_len = ubm_length;
	user_pool.pool_bitmap.bits = (void*)(MEM_BITMAP_BASE + kbm_length);
	
	put_str("    kernel_pool_bitmap_start:");
	put_int((int)kernel_pool.pool_bitmap.bits);
	put_str("  kernel_pool_phy_addr_start:");
	put_int(kernel_pool.phy_addr_start);
	put_str("\n");
	
	put_str("    user_pool_bitmap_start:");
	put_int((int)user_pool.pool_bitmap.bits);
	put_str("  user_pool_phy_addr_start:");
	put_int(user_pool.phy_addr_start);
	put_str("\n");
	
	bitmap_init(&kernel_pool.pool_bitmap);
	bitmap_init(&user_pool.pool_bitmap);

	kernel_vaddr.vaddr_bitmap.btmp_bytes_len = kbm_length;
	kernel_vaddr.vaddr_bitmap.bits = (void*)(MEM_BITMAP_BASE + kbm_length + ubm_length);
	
	kernel_vaddr.vaddr_start = K_HEAP_START;
	bitmap_init(&kernel_vaddr.vaddr_bitmap);
	put_str("  mem_pool_init done\n");
}


void mem_init() {
	put_str("mem_init start\n");
	uint32_t mem_bytes_total = (*(uint32_t*)(0xb00));
	mem_pool_init(mem_bytes_total);
	put_str("mem_init done\n");
}


