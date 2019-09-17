/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 * Copyright (c) 2019, Linaro Limited
 */

#ifndef COMPILER_H
#define COMPILER_H

/*
 * Macros that should be used instead of using __attribute__ directly to
 * ease portability and make the code easier to read.
 *
 * Some of the defines below is known to sometimes cause conflicts when
 * this file is included from xtest in normal world. It is assumed that
 * the conflicting defines has the same meaning in that environment.
 * Surrounding the troublesome defines with #ifndef should be enough.
 */
#ifndef __noreturn
#define __noreturn      __attribute__((noreturn))
#endif
#ifndef __unused
#define __unused        __attribute__((unused))
#endif
#define __maybe_unused  __attribute__((unused))
#ifndef __used
#define __used          __attribute__((__used__))
#endif

#endif /*COMPILER_H*/
