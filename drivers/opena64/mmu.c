#include "arm64.h"
#include "mmu_priv.h"

/**
 * @brief A conveniant macro for invalidating all tlb cache
 *    on a given exception level.
 *
 * @param _el exception level.
 */
#define __invalidate_tlb_all(_el)                                                                  \
	{__asm__ volatile("dsb ishst; tlbi alle" #_el "; dsb ish; isb" : : : "memory")}
#define invalidate_tlb_all_el3 __invalidate_tlb_all(3)
#define invalidate_tlb_all_el1 __invalidate_tlb_all(1)

void mmu_init(void)
{
}
