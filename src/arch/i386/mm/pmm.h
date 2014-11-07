/*
 * =====================================================================================
 *
 *       Filename:  pmm.h
 *
 *    Description:  物理内存管理
 *
 *        Version:  1.0
 *        Created:  2014年11月04日 13时03分24秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Qianyi.lh (liuhuan), qianyi.lh@alibaba-inc.com
 *        Company:  Alibaba-Inc Aliyun
 *
 * =====================================================================================
 */

#ifndef INCLUDE_MM_PMM_H
#define INCLUDE_MM_PMM_H

#include <types.h>

// 默认栈的大小
#define STACK_SIZE 8192

// 物理内存页框大小 
#define PMM_PAGE_SIZE 0x1000

// 页掩码 按照 0x1000(4096) 对齐地址
#define PHY_PAGE_MASK 0xFFFFF000

// 内核在物理内存起始位置
#define RAM_KERNEL_START 0x100000

// 内核代码在内存中的起始和结束位置，在链接脚本中定义
extern uint8_t kern_start[];
extern uint8_t kern_end[];

// BIOS int 0x15 AX = 0xE820 常量
#define E820MAX             20      // 最大的表项数目
#define E820_ARM            1       // 可用 RAM
#define E820_ARR            2       // 保留区域

typedef
struct e820map_t {
        int count;
        struct {
                uint32_t addr_low;
                uint32_t addr_high;
                uint32_t length_low;
                uint32_t length_high;
                uint32_t type;
        } __attribute__((packed)) map[E820MAX];
} e820map_t;

// 内存页类型
typedef
enum mem_zone_t {
        ZONE_DMA = 0,
        ZONE_NORMAL = 1,
        ZONE_HIGHMEM = 2
} mem_zone_t; 

// 物理页结构
typedef
struct page_t {
        uint32_t ref;
        mem_zone_t type;
        uint32_t addr;
} page_t;

static inline uint32_t page_ref(page_t *page)
{
    return page->ref;
}

static inline void set_page_ref(page_t *page, uint32_t val)
{
    page->ref = val;
}

static inline uint32_t page_ref_inc(page_t *page)
{
    page->ref += 1;
    return page->ref;
}

static inline uint32_t page_ref_dec(page_t *page)
{
    page->ref -= 1;
    return page->ref;
}

// 物理页帧数组指针
extern page_t *phy_pages;

// 物理页帧数组长度
extern uint32_t phy_pages_count;

// 内存管理子系统管理对象
struct pmm_manager {
        const char *name;                                 // 管理算法的名称
        void (*page_init)(void);                         // 初始化
        page_t *(*alloc_pages)(uint32_t n);              // 申请物理内存页(n为字节数)
        void (*free_pages)(page_t *page, uint32_t n);   // 释放内存页
        void (*show_memory_info)(void);                   // 输出内存信息
        void (*show_management_info)(void);               // 输出当前管理信息
        void (*test_mm)(void);                            // 测试当前管理子系统
};

// 初始化物理内存管理
void init_pmm(void);

// 内存管理算法初始化
void page_init(void);

// 申请内存页
page_t *alloc_pages(uint32_t n);

// 释放内存页
void free_pages(page_t *page, uint32_t n);

#define alloc_page() alloc_pages(1)
#define free_page(page) free_pages(page, 1)

// 输出内存信息
void show_memory_info(void);

// 输出当前管理信息
void show_management_info(void);

// 测试当前管理子系统
void test_mm(void);

#endif  // INCLUDE_MM_PMM_H
