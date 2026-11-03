/*******************************************************************************
 * @author Miguel Ugsimar <jmjugsimar@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 *******************************************************************************/
#ifndef __MMU_PRIV__H
#define __MMU_PRIV__H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup mmu_priv
 * @ingroup mmu
 *
 * @{
 *
 * @brief ARM memory management unit.
 *
 * @note	None.
 *
 * modification history:
 *  1. mugsimar, oct 21 2026
 *    > Initial version ...
 */

/*
 * Device memory type attributes
 * 1.Gathering (G/nG)
 *   Determines whether multiple accesses can be merged into a single
 *   bus transaction.
 *
 * 2.Reordering (R/nR)
 *   Determines whether accesses to the same device can be reordered.
 *
 * 3 Early Write Acknowledgment (E/nE)
 *   Indicates to the memory system whether a buffer can send
 *   acknowledgements.
 */
#define DEVICE_nGnRnE (0x0U)
#define DEVICE_nGnRE  (0x4U)
#define DEVICE_nGRE   (0x8U)
#define DEVICE_GRE    (0xCU)

/* Read/Write Allocation Configurations for Cacheable Memory */
#define R_NON_W_NON     (0x0U) /* Do not allocate Read/Write */
#define R_NON_W_ALLOC   (0x1U) /* Do not allocate Read, Allocate Write */
#define R_ALLOC_W_NON   (0x2U) /* Allocate Read, Do not allocate Write */
#define R_ALLOC_W_ALLOC (0x3U) /* Allocate Read/Write */

/* Memory Attributes for Normal Memory
 *
 * 1. read-alloc: cache-missed on read, fetch cache line from main memory to cache
 * 2. write-alloc: cache-missed on write, fetch cache line from main memory to cache,
 *      update the cache then update main memory.
 * 3. non-transient: data might be used again so keep it in cache
 * 4. non-alloc: cache-missed will still cause cpu accessing main memory but will
 *      not update the cache line
 * 5. write-through: When data is written to cache, it is immediately written to memory.
 *      This keeps the cache and main memory sync but at the cost of increase in main
 *      memory traffic due to write transactions.
 * 6. write-back: Writes are only performed in cache. This could cause data in memory
 *      to be **stale**. When cache is written , a **dirty** bit in cache is set to `1`
 *      to indicate that data is written in cache but not yet in main memory.
 */
#define NORMAL_O_WT_NT (0x80U) /* Normal, Outer Write-through non-transient */
#define NORMAL_O_WB_NT (0xC0U) /* Normal, Outer Write-back non-transient */
#define NORMAL_O_NON_C (0x40U) /* Normal, Outer Non-Cacheable	*/

#define NORMAL_I_WT_NT (0x08U) /* Normal, Inner Write-through non-transient */
#define NORMAL_I_WB_NT (0x0CU) /* Normal, Inner Write-back non-transient */
#define NORMAL_I_NON_C (0x04U) /* Normal, Inner Non-Cacheable	*/

#define NORMAL_WT_NT (NORMAL_O_WT_NT | NORMAL_I_WT_NT)
#define NORMAL_WB_NT (NORMAL_O_WB_NT | NORMAL_I_WB_NT)
#define NORMAL_NON_C (NORMAL_O_NON_C | NORMAL_I_NON_C)

#define MEM_TYPE_ROM    (NORMAL_NON_C)
#define MEM_TYPE_FLASH  (MEM_TYPE_ROM)
#define MEM_TYPE_SRAM   (NORMAL_WT_NT | R_ALLOC_W_ALLOC)
#define MEM_TYPE_DRAM   (NORMAL_WT_NT | R_ALLOC_W_ALLOC)
#define MEM_TYPE_DEVICE (DEVICE_nGnRnE)

#define IPA_4KB_MAX_BIT_POS  (38U)
#define IPA_64KB_MAX_BIT_POS (40U)

#define DESC_TYPE_TBL   (3U)
#define DESC_TYPE_BLOCK (1U)
#define DESC_TYPE_PAGE  (3U)

#define PAGE_BLOCK_DESC_ATTR_MEMTYPE(x)   (x << 2)
#define PAGE_BLOCK_DESC_ATTR_NS           (1ULL << 5)
#define PAGE_BLOCK_DESC_ATTR_AP_ELx       (1ULL << 6)
#define PAGE_BLOCK_DESC_ATTR_AP_EL_HIGHER (0ULL << 6)
#define PAGE_BLOCK_DESC_ATTR_AP_RO        (1ULL << 7)
#define PAGE_BLOCK_DESC_ATTR_AP_RW        (0ULL << 7)
#define PAGE_BLOCK_DESC_ATTR_NON_SHARE    (0ULL << 8)
#define PAGE_BLOCK_DESC_ATTR_OUTER_SHARE  (2ULL << 8)
#define PAGE_BLOCK_DESC_ATTR_INNER_SHARE  (3ULL << 8)
#define PAGE_BLOCK_DESC_ATTR_AF           (1ULL << 10)
#define PAGE_BLOCK_DESC_ATTR_NG           (1ULL << 11)
#define PAGE_BLOCK_DESC_ATTR_GP           (1ULL << 50)
#define PAGE_BLOCK_DESC_ATTR_PXN          (1ULL << 53)
#define PAGE_BLOCK_DESC_ATTR_UXN          (1ULL << 54)

#ifdef __cplusplus
}
#endif

/* @} */

#endif // __MMU_PRIV__H
