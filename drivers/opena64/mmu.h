/*******************************************************************************
 * @author Miguel Ugsimar <jmjugsimar@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 *******************************************************************************/
#ifndef __MMU_H
#define __MMU_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup mmu
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
 * 48-bit virtual address with a 4 KiB granule:
 *
 * +------------+------------+------------+------------+-----------+
 * | VA [47:39] | VA [38:30] | VA [29:21] | VA [20:12] | VA [11:0] |
 * +---------------------------------------------------------------+
 * |     L0     |     L1     |     L2     |     L3     | block off |
 * +------------+------------+------------+------------+-----------+
 */

#define MMU_4KB_GRANULE_SIZE     (1ULL << 12)
#define MMU_4KB_TABLE_ENTRIES    (512ULL)
#define MMU_4KB_TABLE_ENTRY_SIZE (8ULL)
#define MMU_4KB_TABLE_SIZE       (MMU_4KB_TABLE_ENTRIES * MMU_4KB_TABLE_ENTRY_SIZE)

#define MMU_VA_4KB_BLOCK_OFFSET_BIT_MSK (0xFFFULL)
#define MMU_VA_4KB_BLOCK_OFFSET_BIT_POS (0U)

#define MMU_VA_4KB_L3_IDX_BIT_MSK (0x1FFULL)
#define MMU_VA_4KB_L3_IDX_BIT_POS (12U)
#define MMU_VA_4KB_L3_ENTRY_SPAN  (1ULL << MMU_VA_4KB_L3_IDX_BIT_POS)

#define MMU_VA_4KB_L2_IDX_BIT_MSK (0x1FFULL)
#define MMU_VA_4KB_L2_IDX_BIT_POS (21U)
#define MMU_VA_4KB_L2_ENTRY_SPAN  (1ULL << MMU_VA_4KB_L2_IDX_BIT_POS)

#define MMU_VA_4KB_L1_IDX_BIT_MSK (0x1FFULL)
#define MMU_VA_4KB_L1_IDX_BIT_POS (30U)
#define MMU_VA_4KB_L1_ENTRY_SPAN  (1ULL << MMU_VA_4KB_L1_IDX_BIT_POS)

#define MMU_VA_4KB_L0_IDX_BIT_MSK (0x1FFULL)
#define MMU_VA_4KB_L0_IDX_BIT_POS (39U)
#define MMU_VA_4KB_L0_ENTRY_SPAN  (1ULL << MMU_VA_4KB_L0_IDX_BIT_POS)

/*
 * 48-bit virtual address with a 16 KiB granule. The L0 table uses one index
 * bit; the remaining levels use 11 index bits each.
 */
#define MMU_16KB_GRANULE_SIZE     (1ULL << 14)
#define MMU_16KB_TABLE_ENTRIES    (2048ULL)
#define MMU_16KB_TABLE_ENTRY_SIZE (8ULL)
#define MMU_16KB_TABLE_SIZE       (MMU_16KB_TABLE_ENTRIES * MMU_16KB_TABLE_ENTRY_SIZE)

#define MMU_VA_16KB_BLOCK_OFFSET_BIT_MSK (0x3FFFULL)
#define MMU_VA_16KB_BLOCK_OFFSET_BIT_POS (0U)

#define MMU_VA_16KB_L3_IDX_BIT_MSK (0x7FFULL)
#define MMU_VA_16KB_L3_IDX_BIT_POS (14U)
#define MMU_VA_16KB_L3_ENTRY_SPAN  (1ULL << MMU_VA_16KB_L3_IDX_BIT_POS)

#define MMU_VA_16KB_L2_IDX_BIT_MSK (0x7FFULL)
#define MMU_VA_16KB_L2_IDX_BIT_POS (25U)
#define MMU_VA_16KB_L2_ENTRY_SPAN  (1ULL << MMU_VA_16KB_L2_IDX_BIT_POS)

#define MMU_VA_16KB_L1_IDX_BIT_MSK (0x7FFULL)
#define MMU_VA_16KB_L1_IDX_BIT_POS (36U)
#define MMU_VA_16KB_L1_ENTRY_SPAN  (1ULL << MMU_VA_16KB_L1_IDX_BIT_POS)

#define MMU_VA_16KB_L0_IDX_BIT_MSK (0x1ULL)
#define MMU_VA_16KB_L0_IDX_BIT_POS (47U)
#define MMU_VA_16KB_L0_ENTRY_SPAN  (1ULL << MMU_VA_16KB_L0_IDX_BIT_POS)

/*
 * 48-bit virtual address with a 64 KiB granule. Translation starts at L1;
 * the L0 table is not needed for this address size.
 */
#define MMU_64KB_GRANULE_SIZE     (1ULL << 16)
#define MMU_64KB_TABLE_ENTRIES    (8192ULL)
#define MMU_64KB_TABLE_ENTRY_SIZE (8ULL)
#define MMU_64KB_TABLE_SIZE       (MMU_64KB_TABLE_ENTRIES * MMU_64KB_TABLE_ENTRY_SIZE)

#define MMU_VA_64KB_BLOCK_OFFSET_BIT_MSK (0xFFFFULL)
#define MMU_VA_64KB_BLOCK_OFFSET_BIT_POS (0U)

#define MMU_VA_64KB_L3_IDX_BIT_MSK (0x1FFFULL)
#define MMU_VA_64KB_L3_IDX_BIT_POS (16U)
#define MMU_VA_64KB_L3_ENTRY_SPAN  (1ULL << MMU_VA_64KB_L3_IDX_BIT_POS)

#define MMU_VA_64KB_L2_IDX_BIT_MSK (0x1FFFULL)
#define MMU_VA_64KB_L2_IDX_BIT_POS (29U)
#define MMU_VA_64KB_L2_ENTRY_SPAN  (1ULL << MMU_VA_64KB_L2_IDX_BIT_POS)

#define MMU_VA_64KB_L1_IDX_BIT_MSK (0x3FULL)
#define MMU_VA_64KB_L1_IDX_BIT_POS (42U)
#define MMU_VA_64KB_L1_ENTRY_SPAN  (1ULL << MMU_VA_64KB_L1_IDX_BIT_POS)

/*
 * Stage-1 translation descriptor type encodings.
 * Descriptor output addresses are limited here to 48 physical address bits;
 * combine the entry macros with the required attribute bits before use.
 */
#define MMU_DESC_VALID_BIT_POS      (0U)
#define MMU_DESC_VALID_BIT_MSK      (1ULL << MMU_DESC_VALID_BIT_POS)
#define MMU_DESC_TYPE_BIT_POS       (1U)
#define MMU_DESC_TYPE_BIT_MSK       (1ULL << MMU_DESC_TYPE_BIT_POS)
#define MMU_DESC_TYPE_BIT_FIELD_MSK (MMU_DESC_VALID_BIT_MSK | MMU_DESC_TYPE_BIT_MSK)

#define MMU_DESC_TYPE_INVALID    (0ULL)
#define MMU_DESC_TYPE_BLOCK      (MMU_DESC_VALID_BIT_MSK)
#define MMU_DESC_TYPE_TABLE_PAGE (MMU_DESC_VALID_BIT_MSK | MMU_DESC_TYPE_BIT_MSK)

#define MMU_DESC_OUTPUT_ADDR_BIT_MSK (0x0000FFFFFFFFFFFFULL)
#define MMU_DESC_OUTPUT_ADDR_MSK(alignment)                                                        \
	(MMU_DESC_OUTPUT_ADDR_BIT_MSK & ~((uint64_t)(alignment) - 1ULL))

/**
 * @brief Build a table descriptor with an aligned next-level table address.
 * @param address Physical base address of the next-level table.
 * @param granule_size Translation granule size in bytes (4 KiB, 16 KiB, or 64 KiB).
 *
 * Add any required descriptor attributes to the returned value with bitwise OR.
 */
#define MMU_DESC_TABLE_ENTRY(address, granule_size)                                                \
	(((uint64_t)(address) & MMU_DESC_OUTPUT_ADDR_MSK(granule_size)) | MMU_DESC_TYPE_TABLE_PAGE)

/**
 * @brief Build a block descriptor with an aligned output address.
 * @param address Physical base address of the block.
 * @param block_size Block size in bytes for the selected granule and level.
 *
 * Block descriptors are valid at levels 1 and 2, not level 0 or level 3.
 * Add any required descriptor attributes to the returned value with bitwise OR.
 */
#define MMU_DESC_BLOCK_ENTRY(address, block_size)                                                  \
	(((uint64_t)(address) & MMU_DESC_OUTPUT_ADDR_MSK(block_size)) | MMU_DESC_TYPE_BLOCK)

/* Descriptor types permitted at each translation level. */
#define MMU_L0_DESC_TABLE_ENTRY_TYPE MMU_DESC_TYPE_TABLE_PAGE
#define MMU_L1_DESC_BLOCK_ENTRY_TYPE MMU_DESC_TYPE_BLOCK
#define MMU_L1_DESC_TABLE_ENTRY_TYPE MMU_DESC_TYPE_TABLE_PAGE
#define MMU_L2_DESC_BLOCK_ENTRY_TYPE MMU_DESC_TYPE_BLOCK
#define MMU_L2_DESC_TABLE_ENTRY_TYPE MMU_DESC_TYPE_TABLE_PAGE
#define MMU_L3_DESC_PAGE_ENTRY_TYPE  MMU_DESC_TYPE_TABLE_PAGE

#define MMU_BLOCK_DESC_NS (1ULL << 5)

#define MMU_BLOCK_DESC_AP_RW (1ULL << 5)
#define MMU_BLOCK_DESC_AP_WO (1ULL << 5)
#define MMU_BLOCK_DESC_AP_RO (1ULL << 6)
/** @} */

#ifdef __cplusplus
}
#endif

#endif // __MMU_H
