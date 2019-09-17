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

unsigned int spci_config[] = {
	[SPCI_DEVICE_IDX_S] = SPCI_SCMI_SERVICE_IDX_PSCI,
	[SPCI_DEVICE_IDX_NS_H] = SPCI_SCMI_SERVICE_IDX_OSPM_0,
	[SPCI_DEVICE_IDX_NS_L] = SPCI_SCMI_SERVICE_IDX_OSPM_1
};

static const struct fwk_element spci_element_table[] = {
    [SPCI_DEVICE_IDX_S] = {
        .name = "SPCI TEE",
        .sub_element_count = 1,
        .data = (void *)&spci_config[SPCI_DEVICE_IDX_S],
    },
    [SPCI_DEVICE_IDX_NS_H] = {
        .name = "SPCI OSPM0",
        .sub_element_count = 1,
        .data = (void *)&spci_config[SPCI_DEVICE_IDX_NS_H],
    },
    [SPCI_DEVICE_IDX_NS_L] = {
        .name = "SPCI OSPM1",
        .sub_element_count = 1,
        .data = (void *)&spci_config[SPCI_DEVICE_IDX_NS_L],
    },
    [SPCI_DEVICE_IDX_COUNT] = { 0 },
};

static const struct fwk_element *spci_get_element_table(fwk_id_t module_id)
{
    return spci_element_table;
}

struct fwk_module_config config_optee_mhu = {
    .get_element_table = spci_get_element_table,
};

void scmi_server_raise_event(uint32_t id)
{
    DMSG("id %"PRIx32, id);
    optee_mhu_signal_smt_message(SPCI_DEVICE_IDX_NS_H, id);
}
