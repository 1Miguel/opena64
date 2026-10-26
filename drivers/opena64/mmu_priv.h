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

#define IPA_4KB_MAX_BIT_POS  (38U)
#define IPA_64KB_MAX_BIT_POS (40U)

#define MMU_DESC_TYPE_TBL   (3U)
#define MMU_DESC_TYPE_BLOCK (1U)
#define MMU_DESC_TYPE_PAGE  (3U)

#define MMU_PAGE_BLOCK_DESC_ATTR_MEMTYPE(x)   (x << 2)
#define MMU_PAGE_BLOCK_DESC_ATTR_NS           (1ULL << 5)
#define MMU_PAGE_BLOCK_DESC_ATTR_AP_ELx       (1ULL << 6)
#define MMU_PAGE_BLOCK_DESC_ATTR_AP_EL_HIGHER (0ULL << 6)
#define MMU_PAGE_BLOCK_DESC_ATTR_AP_RO        (1ULL << 7)
#define MMU_PAGE_BLOCK_DESC_ATTR_AP_RW        (0ULL << 7)
#define MMU_PAGE_BLOCK_DESC_ATTR_NON_SHARE    (0ULL << 8)
#define MMU_PAGE_BLOCK_DESC_ATTR_OUTER_SHARE  (2ULL << 8)
#define MMU_PAGE_BLOCK_DESC_ATTR_INNER_SHARE  (3ULL << 8)
#define MMU_PAGE_BLOCK_DESC_ATTR_AF           (1ULL << 10)
#define MMU_PAGE_BLOCK_DESC_ATTR_NG           (1ULL << 11)
#define MMU_PAGE_BLOCK_DESC_ATTR_GP           (1ULL << 50)
#define MMU_PAGE_BLOCK_DESC_ATTR_PXN          (1ULL << 53)
#define MMU_PAGE_BLOCK_DESC_ATTR_UXN          (1ULL << 54)

#ifdef __cplusplus
}
#endif

/* @} */

#endif // __MMU_PRIV__H
