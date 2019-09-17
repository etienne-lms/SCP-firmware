/*
 * Arm SCP/MCP Software
 * Copyright (c) 2015-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <fwk_element.h>
#include <fwk_id.h>
#include <fwk_module.h>
#include <fwk_module_idx.h>
#include <mod_optee_mhu.h>
#include <spci_scmi.h>

// OP-TEE headers
#include <scmi.h>
#include <trace.h>

unsigned int mhu_config[] = {
	[SPCI_DEVICE_IDX_NS_H] = SPCI_SCMI_SERVICE_IDX_OSPM_0,
#ifndef ONLY_OSPM0_MAILBOX
	[SPCI_DEVICE_IDX_S] = SPCI_SCMI_SERVICE_IDX_PSCI,
	[SPCI_DEVICE_IDX_NS_L] = SPCI_SCMI_SERVICE_IDX_OSPM_1,
#endif
};

static const struct fwk_element mhu_element_table[] = {
#ifndef ONLY_OSPM0_MAILBOX
    [SPCI_DEVICE_IDX_S] = {
        .name = "SPCI secure mailbox for TEE",
        .sub_element_count = 1,
        .data = (void *)&mhu_config[SPCI_DEVICE_IDX_S],
    },
#endif
    [SPCI_DEVICE_IDX_NS_H] = {
        .name = "OSPM0 mailbox used for SCMI",
        .sub_element_count = 1,
        .data = (void *)&mhu_config[SPCI_DEVICE_IDX_NS_H],
    },
#ifndef ONLY_OSPM0_MAILBOX
    [SPCI_DEVICE_IDX_NS_L] = {
        .name = "OSPM1 mailbox used for SCMI",
        .sub_element_count = 1,
        .data = (void *)&mhu_config[SPCI_DEVICE_IDX_NS_L],
    },
#endif
    [SPCI_DEVICE_IDX_COUNT] = { 0 },
};

static const struct fwk_element *mhu_get_element_table(fwk_id_t module_id)
{
    return mhu_element_table;
}

struct fwk_module_config config_optee_mhu = {
    .get_element_table = mhu_get_element_table,
};

void scmi_server_raise_event(uint32_t id)
{
    DMSG("id %"PRIx32, id);
    optee_mhu_signal_smt_message(SPCI_DEVICE_IDX_NS_H, id);
}
