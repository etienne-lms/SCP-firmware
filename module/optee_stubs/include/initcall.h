/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2019, Linaro Limited
 */

#ifndef INITCALL_H
#define INITCALL_H

#include <compiler.h>

#define INITCALL(fn)	static void __unused *__##fn = (void *)(unsigned long)fn
#define service_init(fn)	INITCALL(fn)
#define service_init_late(fn)	INITCALL(fn)
#define driver_init(fn)		INITCALL(fn)
#define driver_init_late(fn)	INITCALL(fn)

#endif /* INITCALL_H */
