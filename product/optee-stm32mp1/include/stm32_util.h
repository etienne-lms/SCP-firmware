/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2018-2019, STMicroelectronics
 */

#ifndef STM32_UTIL_H
#define STM32_UTIL_H

typedef uint32_t TEE_Result;

/*
 * Util for clock gating and to get clock rate for stm32 and platform drivers
 * @id: Target clock ID, ID used in clock DT bindings
 */
void stm32_clock_enable(unsigned long id);
void stm32_clock_disable(unsigned long id);
unsigned long stm32_clock_get_rate(unsigned long id);
bool stm32_clock_is_enabled(unsigned long id);

/* Return true if and only if non-secure world use the resource */
bool stm32mp_nsec_can_access_clock(unsigned long id);

/*
 * Util for reset signal assertion/desassertion for stm32 and platform drivers
 * @id: Target peripheral ID, ID used in reset DT bindings
 * @to_us: Timeout out in microsecond, or 0 if not waiting signal state
 */
TEE_Result stm32_reset_assert(unsigned int id, unsigned int timeoutout_us);
TEE_Result stm32_reset_deassert(unsigned int id, unsigned int timeoutout_us);

/* Return true if and only if @reset_id relates to a non-secure peripheral */
bool stm32mp_nsec_can_access_reset(unsigned int reset_id);

/*!
 * \brief Synchornise clock states after product initialization.
 */
void stm32_scmi_clock_late_init(void);

#endif /* STM32_UTIL_H */
