/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2016, Linaro Limited
 * Copyright (c) 2014, STMicroelectronics International N.V.
 */
#ifndef CORE_MEMPROT_H
#define CORE_MEMPROT_H

enum teecore_memtypes {
        MEM_AREA_END = 0,
        MEM_AREA_NSEC_SHM,
        MEM_AREA_RAM_NSEC,
        MEM_AREA_RAM_SEC,
        MEM_AREA_IO_NSEC,
        MEM_AREA_IO_SEC,
        MEM_AREA_SHM_VASPACE,
};

/*
 * Translate physical address to virtual address using specified mapping
 * Returns NULL on failure or a valid virtual address on success.
 */
void *phys_to_virt(uintptr_t pa, enum teecore_memtypes m);

/*
 * Translate virtual address to physical address
 * Returns 0 on failure or a valid physical address on success.
 */
uintptr_t virt_to_phys(void *va);

#endif /* CORE_MEMPROT_H */
