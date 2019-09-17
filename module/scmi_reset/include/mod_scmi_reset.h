/*
 * Arm SCP/MCP Software
 * Copyright (c) 2015-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Description:
 *      SCMI Reset Domain Protocol Support.
 */

#ifndef MOD_SCMI_RESET_H
#define MOD_SCMI_RESET_H

#include <stdint.h>

/*!
 * \ingroup GroupModules Modules
 * \defgroup GroupSCMI_RESET SCMI Reset Domain Management Protocol
 * \{
 */

/*!
 * \brief Permission flags governing the ability to use certain SCMI commands to
 *      interact with a clock.
 *
 * \details Setting a permission flag for a clock enables the corresponding
 *      functionality for any agent that has visibilty of the clock
 *      through its clock device table.
 */
enum mod_scmi_reset_permissions {
    /*! No permissions (at least one must be granted) */
    MOD_SCMI_RESET_PERM_INVALID = 0,

    /*! The reset's attributes can be queried */
    MOD_SCMI_RESET_PERM_ATTRIBUTES = (1 << 0),

    /*! The reset can be asserted/deassert */
    MOD_SCMI_RESET_PERM_RESET = (1 << 1),

    /*! The reset's state change can be notified */
    MOD_SCMI_RESET_PERM_NOTIFY = (1 << 2),
};

/*!
 * \brief Reset domain device.
 *
 * \details Reset domain device structures are used in per-agent reset device
 *      tables. Each contains an identifier of an element that will be bound
 *      to in order to use the reset device.
 */
struct mod_scmi_reset_device {
    /*!
     * \brief Reset element identifier.
     *
     * \details The module that owns the element must implement the Reset API
     *      that is defined by the \c reset module.
     */
    fwk_id_t element_id;
    /*! Mask of permission flags defined by \ref mod_scmi_clock_permissions */
    uint8_t permissions;
};

/*!
 * \brief Agent descriptor.
 *
 * \details Describes an agent that uses the SCMI Reset Domain protocol.
 *      Provides a pointer to the agent's reset device table and the number of
 *      devices within the table.
 */
struct mod_scmi_reset_agent {
    /*! Pointer to a table of clock devices that are visible to the agent */
    const struct mod_scmi_reset_device *device_table;

    /*!
     * \brief The number of \c mod_scmi_reset_device structures in the table
     *      pointed to by \c device_table.
     */
    uint8_t device_count;
};

/*!
 * \brief Module configuration.
 */
struct mod_scmi_reset_config {
    /*! Max supported number of pending, asynchronous reset domain changes */
    uint8_t max_pending_transactions;

    /*!
     * \brief Pointer to the table of agent descriptors, used to provide
     *      per-agent views of reset in the system.
     */
    const struct mod_scmi_reset_agent *agent_table;

    /*! Number of agents in \ref agent_table */
    size_t agent_count;
};

/*!
 * \}
 */

#endif /* MOD_SCMI_RESET_H */
