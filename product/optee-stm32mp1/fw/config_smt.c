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
#include <mod_optee_smt.h>
#include <scmi.h>
#include <spci_scmi.h>

#ifndef CFG_WITH_SPCI
#include <mm/core_memprot.h>
#endif

#define SCMI_PAYLOAD_SIZE       (128)

uint8_t scmi_psci_mb[SCMI_PAYLOAD_SIZE];
#ifdef CFG_WITH_SPCI
uint8_t scmi_ospm0_mb[SCMI_PAYLOAD_SIZE];
#else
uint8_t *scmi_ospm0_mb = (void *)(CFG_SHMEM_START + CFG_SHMEM_SIZE);
#endif
uint8_t scmi_ospm1_mb[SCMI_PAYLOAD_SIZE];

static struct fwk_element smt_element_table[] = {
#ifndef ONLY_OSPM0_MAILBOX
    [SPCI_SCMI_SERVICE_IDX_PSCI] = {
        .name = "PSCI",
        .data = &((struct mod_optee_smt_channel_config) {
            .policies = MOD_OPTEE_SMT_POLICY_INIT_MAILBOX |
                        MOD_OPTEE_SMT_POLICY_SECURE,
            .mailbox_address = (uintptr_t)scmi_psci_mb,
            .mailbox_size = SCMI_PAYLOAD_SIZE,
            .driver_id = FWK_ID_SUB_ELEMENT_INIT(FWK_MODULE_IDX_OPTEE_MHU,
                                                 SPCI_DEVICE_IDX_S, 0),
            .driver_api_id = FWK_ID_API_INIT(FWK_MODULE_IDX_OPTEE_MHU, 0),
        })
    },
#endif
    [SPCI_SCMI_SERVICE_IDX_OSPM_0] = {
        .name = "OSPM0",
        .data = &((struct mod_optee_smt_channel_config) {
            .policies = MOD_OPTEE_SMT_POLICY_INIT_MAILBOX,
#ifdef CFG_WITH_SPCI
            .mailbox_address = (uintptr_t)scmi_ospm0_mb,
#else
            .mailbox_address = (uintptr_t)(CFG_SHMEM_START + CFG_SHMEM_SIZE),
#endif
            .mailbox_size = SCMI_PAYLOAD_SIZE,
            .driver_id = FWK_ID_SUB_ELEMENT_INIT(FWK_MODULE_IDX_OPTEE_MHU,
                                                 SPCI_DEVICE_IDX_NS_H, 0),
            .driver_api_id = FWK_ID_API_INIT(FWK_MODULE_IDX_OPTEE_MHU, 0),
        })
    },
#ifndef ONLY_OSPM0_MAILBOX
    [SPCI_SCMI_SERVICE_IDX_OSPM_1] = {
        .name = "OSPM1",
        .data = &((struct mod_optee_smt_channel_config) {
            .policies = MOD_OPTEE_SMT_POLICY_INIT_MAILBOX,
            .mailbox_address = (uintptr_t)scmi_ospm1_mb,
            .mailbox_size = SCMI_PAYLOAD_SIZE,
            .driver_id = FWK_ID_SUB_ELEMENT_INIT(FWK_MODULE_IDX_OPTEE_MHU,
                                                 SPCI_DEVICE_IDX_NS_L, 0),
            .driver_api_id = FWK_ID_API_INIT(FWK_MODULE_IDX_OPTEE_MHU, 0),
        })
    },
#endif
    [SPCI_SCMI_SERVICE_IDX_COUNT] = { 0 },
};

static const struct fwk_element *smt_get_element_table(fwk_id_t module_id)
{
#ifndef CFG_WITH_SPCI
	const void *data = NULL;
	struct mod_optee_smt_channel_config *cfg = NULL;

	/* Set virtual address for mailbox buffers */
	data = smt_element_table[SPCI_SCMI_SERVICE_IDX_OSPM_0].data;
	cfg = (struct mod_optee_smt_channel_config *)data;

	cfg->mailbox_address = (vaddr_t)phys_to_virt(cfg->mailbox_address,
						     MEM_AREA_IO_NSEC);
#endif

	return (const struct fwk_element *)smt_element_table;
}

struct fwk_module_config config_optee_smt = {
    .get_element_table = smt_get_element_table,
};

#ifdef CFG_WITH_SPCI
void *spci_get_buffer_ospm0(void)
{
	const void *data = NULL;
	struct mod_optee_smt_channel_config *cfg = NULL;

	data = smt_element_table[SPCI_SCMI_SERVICE_IDX_OSPM_0].data;
	cfg = (struct mod_optee_smt_channel_config *)data;

	return (void *)cfg->mailbox_address;
}
#endif
