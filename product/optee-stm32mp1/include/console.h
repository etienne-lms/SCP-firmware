/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2014, Linaro Limited
 */

#ifndef CONSOLE_H
#define CONSOLE_H

void console_init(void);
void console_putc(int ch);
void console_flush(void);

#endif /* CONSOLE_H */

