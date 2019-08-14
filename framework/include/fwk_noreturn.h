/*
 * Arm SCP/MCP Software
 * Copyright (c) 2018-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
*
 * Description:
 *      Provides <stdnoreturn.h> features missing in certain standard library
 *      implementations.
 */

#ifndef FWK_NORETURN_H
#define FWK_NORETURN_H

#ifdef BUILD_OPTEE
/* OP-TEE relies on __noretrun as attribute */
#include <compiler.h>
#else
#ifdef __ARMCC_VERSION
#   define __noreturn _Noreturn
#else
#   include <stdnoreturn.h>
#endif
#endif //BUILD_OPTEE

#endif /* FWK_NORETURN_H */
