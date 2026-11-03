/**
 * @brief Contains all assembly routines for initialising
 *    the mmu for different exception levels.
 */

.global mmu_init_el3

.set CPUECTLR_SMPEN, #(1U << 6)
.set SCTLR_MMU_ENABLE, #(1U << 0)
.set SCTLR_DCACHE_ENABLE, #(1U << 2)
.set SCTLR_ICACHE_ENABLE, #(1U << 12)

/**
 * @brief convenient macro for creating multiple table entries
 *    of the same attributes.
 *
 * @param n          number of entries to create.
 * @param addr_start starting address.
 * @param addr_inc   address increment.
 * @param attr       upper and lower attribute.
 */
.macro tbl_entries(n, addr_start, addr_inc, attr)
  .set _addr, \addr_start
  .rept \n
    .quad (\attr | addr)
    .set _addr, (_addr + \inc)
  .endr
.endm // tbl_entries 

mmu_init:
	tlbi ALLE3             //; invalidate all tlb entries
	ic IALLU               //; Invalidate I cache to PoU
	bl invalidate_dcaches  //; invalidate D cache to PoU
	dsb	sy
	isb

  // set the translation config
  msr TCR_EL1, x1

  // set the l0 translation table
	ldr x1, =l0_table
	msr TTBR0_EL3, x1
	msr TTBR0_EL1, x1

  // enable the SMPEN
  mrs x0, s3_1_c15_c2_1
  orr x0, x0, CPUECTLR_SMPEN
  msr s3_1_c15_c2_1, x0

  // enable caches and mmu
  mrs x0, SCTLR_EL3
  orr x0, x0, SCTLR_DCACHE_ENABLE
  orr x0, x0, SCTLR_ICACHE_ENABLE
  orr x0, x0, SCTLR_MMU_ENABLE
  msr SCTLR_EL3, x0
  dsb sy
  isb

  ret

invalidate_dcaches:

	dmb     ISH
	mrs     x0, CLIDR_EL1          //; x0 = CLIDR
	ubfx    w2, w0, #24, #3        //; w2 = CLIDR.LoC
	cmp     w2, #0                 //; LoC is 0?
	b.eq    invalidate_dcaches_end //; No cleaning required and enable MMU
	mov     w1, #0                 //; w1 = level iterator

for_each_level:
	add     w3, w1, w1, lsl #1     //; w3 = w1 * 3 (right-shift for cache type)
	lsr     w3, w0, w3             //; w3 = w0 >> w3
	ubfx    w3, w3, #0, #3         //; w3 = cache type of this level
	cmp     w3, #2                 //; No cache at this level?
	b.lt    for_each_level

	lsl     w4, w1, #1
	msr     CSSELR_EL1, x4         //; Select current cache level in CSSELR
	isb                            //; ISB required to reflect new CSIDR
	mrs     x4, CCSIDR_EL1         //; w4 = CSIDR

	ubfx    w3, w4, #0, #3
	add    	w3, w3, #2             //; w3 = log2(line size)
	ubfx    w5, w4, #13, #15
	ubfx    w4, w4, #3, #10        //; w4 = Way number
	clz     w6, w4                 //; w6 = 32 - log2(number of ways)

for_each_set:
	mov     w8, w4                 //; w8 = Way number

for_each_way:
	lsl     w7, w1, #1             //; Fill level field
	lsl     w9, w5, w3
	orr     w7, w7, w9             //; Fill index field
	lsl     w9, w8, w6
	orr     w7, w7, w9             //; Fill way field
	dc      CISW, x7               //; Invalidate by set/way to point of coherency
	subs    w8, w8, #1             //; Decrement way
	b.ge    for_each_way
	subs    w5, w5, #1             //; Descrement set
	b.ge    for_each_set

for_each_level:
	add     w1, w1, #1             //; Next level
	cmp     w2, w1
	b.gt    for_each_level

invalidate_dcaches_end:
	ret
