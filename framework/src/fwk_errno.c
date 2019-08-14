/*
 * Arm SCP/MCP Software
 * Copyright (c) 2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Description:
 *     Framework error code helper.
 */

#include <fwk_errno.h>
#include <fwk_macros.h>

static const char * const errstr[] = {
    [FWK_SUCCESS]        = "SUCCESS",
    [-FWK_E_PARAM]       = "E_PARAM",
    [-FWK_E_ALIGN]       = "E_ALIGN",
    [-FWK_E_SIZE]        = "E_SIZE",
    [-FWK_E_HANDLER]     = "E_HANDLER",
    [-FWK_E_ACCESS]      = "E_ACCESS",
    [-FWK_E_RANGE]       = "E_RANGE",
    [-FWK_E_TIMEOUT]     = "E_TIMEOUT",
    [-FWK_E_NOMEM]       = "E_NOMEM",
    [-FWK_E_PWRSTATE]    = "E_PWRSTATE",
    [-FWK_E_SUPPORT]     = "E_SUPPORT",
    [-FWK_E_DEVICE]      = "E_DEVICE",
    [-FWK_E_BUSY]        = "E_BUSY",
    [-FWK_E_OS]          = "E_OS",
    [-FWK_E_DATA]        = "E_DATA",
    [-FWK_E_STATE]       = "E_STATE",
    [-FWK_E_INIT]        = "E_INIT",
    [-FWK_E_OVERWRITTEN] = "E_OVERWRITTEN",
    [-FWK_E_PANIC]       = "E_PANIC",
};

const char *fwk_err2str(int errno)
{
    static const char unknown[] = "<unknown>";
    static const char unknown_positive[] = "<unknown-positive>";
    unsigned int idx = (unsigned int)(-errno);

    if (errno > 0)
	return unknown_positive;

    if (idx <= FWK_ARRAY_SIZE(errstr))
        return errstr[idx];

    return unknown;
}
