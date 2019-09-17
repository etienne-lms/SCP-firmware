/*
 * Copyright (c) 2015-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * OPTEE product in SCP-firmware builds a SCMI server stack for the
 * OP-TEE core firmware. SCMI server comes with fwk and its embedded
 * modules into OP-TEE core.
 */

#include <fwk_mm.h>
#include <initcall.h>
#include <internal/fwk_module.h>
#include <internal/fwk_thread.h>
#include <kernel/panic.h>
#include <malloc.h>
#include <scmi.h>
#include <trace.h>

void *fwk_mm_alloc(size_t num, size_t size)
{
    return malloc(num * size);
}

void *fwk_mm_calloc(size_t num, size_t size)
{
    return calloc(num, size);
}

void scmi_server_run(void)
{
	__fwk_run_event();
}

static TEE_Result init_scmi(void)
{
	int rc = __fwk_module_init();


	if (rc == FWK_SUCCESS)
		return TEE_SUCCESS;

	EMSG("SCMI server init failed with %d/%s", rc, fwk_err2str(rc));
	panic();

}
driver_init(init_scmi);
