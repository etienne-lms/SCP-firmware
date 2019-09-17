/*
 * Arm SCP/MCP Software
 * Copyright (c) 2015-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Description:
 *     Definitions for SCMI and SMT module configurations.
 */

#ifndef SPCI_SCMI_H
#define SPCI_SCMI_H

//#define ONLY_OSPM0_MAILBOX

/* SCMI agent identifiers */
enum spci_scmi_agent_id {
    /* 0 is reserved for the platform */
    SCMI_AGENT_ID_OSPM = 1,
#ifndef ONLY_OSPM0_MAILBOX
    SCMI_AGENT_ID_PSCI,
#endif
    SCMI_AGENT_ID_COUNT,
};

/* SCMI service indexes */
enum spci_scmi_service_idx {
#ifndef ONLY_OSPM0_MAILBOX
    SPCI_SCMI_SERVICE_IDX_PSCI,
#endif
    SPCI_SCMI_SERVICE_IDX_OSPM_0,
#ifndef ONLY_OSPM0_MAILBOX
    SPCI_SCMI_SERVICE_IDX_OSPM_1,
#endif
    SPCI_SCMI_SERVICE_IDX_COUNT,
};

enum spci_device_idx {
#ifndef ONLY_OSPM0_MAILBOX
    SPCI_DEVICE_IDX_S,
#endif
    SPCI_DEVICE_IDX_NS_H,
#ifndef ONLY_OSPM0_MAILBOX
    SPCI_DEVICE_IDX_NS_L,
#endif
    SPCI_DEVICE_IDX_COUNT
};

#endif /* SPCI_SCMI_H */
