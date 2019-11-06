/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 *
 *  Stubs from OP-TEE resources when built outside OP-TEE source tree
 */
#ifndef TRACE_H
#define TRACE_H

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>

#define TRACE_LEVEL		3

#define TRACE_ERROR		1
#define TRACE_INFO		2
#define TRACE_DEBUG		3
#define TRACE_FLOW		4

__attribute__ ((__format__ (__printf__, 5, 0)))
void trace_vprintf(const char *func, int line, int level, bool level_ok,
		   const char *fmt, va_list args);

__attribute__ ((__format__ (__printf__, 5, 6)))
void trace_printf(const char *func, int line, int level, bool level_ok,
		  const char *fmt, ...);

#define trace_printf_helper(level, level_ok, ...) \
	trace_printf(__func__, __LINE__, (level), (level_ok), \
		     __VA_ARGS__)

#define MSG(...)   trace_printf_helper(0, false, __VA_ARGS__)
#define EMSG(...)   trace_printf_helper(TRACE_ERROR, true, __VA_ARGS__)
#define IMSG(...)   trace_printf_helper(TRACE_INFO, true, __VA_ARGS__)
#define DMSG(...)   trace_printf_helper(TRACE_DEBUG, true, __VA_ARGS__)
#define FMSG(...)   trace_printf_helper(TRACE_FLOW, true, __VA_ARGS__)

#endif /* TRACE_H */
