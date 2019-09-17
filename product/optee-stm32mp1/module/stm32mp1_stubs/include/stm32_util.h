/*
 * Copyright (c) 2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OPTEE_STM32_UTIL_H
#define OPTEE_STM32_UTIL_H

unsigned long stm32_nsec_clock_get_rate(unsigned long int clock_id);
void stm32_nsec_clock_disable(unsigned long int clock_id);
void stm32_nsec_clock_enable(unsigned long int clock_id);
bool stm32_nsec_clock_is_enabled(unsigned long int clock_id);

bool stm32mp_reset_is_non_secure(unsigned long int clock_id);
void stm32_reset_assert(unsigned long int clock_id);
void stm32_reset_deassert(unsigned long int clock_id);

#endif /* OPTEE_STM32_UTIL_H */
