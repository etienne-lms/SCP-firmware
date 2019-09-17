/*
 * Arm SCP/MCP Software
 * Copyright (c) 2015-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Description:
 *     OP-TEE mailbox buffer layer
 */

#ifndef MOD_OPTEE_SMT_H
#define MOD_OPTEE_SMT_H

#include <stddef.h>
#include <stdint.h>
#include <fwk_id.h>

/*!
 * \brief Message header layout
 *
 * \details Define the memory mayout of a message mailbox
 */
struct __attribute((packed)) mod_smt_memory {
    uint32_t reserved0;
    uint32_t status;
    uint64_t reserved1;
    uint32_t flags;
    uint32_t length; /* message_header + payload */
    uint32_t message_header;
    uint32_t payload[];
};

/*!
 * \name Channel policies
 *
 * \details These policies define attributes that affect how the channel is
 * treated by the SMT component.
 *
 * @{
 */

/*! No policies */
#define MOD_OPTEE_SMT_POLICY_NONE         ((uint32_t)0)

/*! This channel is secure */
#define MOD_OPTEE_SMT_POLICY_SECURE       ((uint32_t)(1 << 0))

/*! The mailbox for this channel requires initialization */
#define MOD_OPTEE_SMT_POLICY_INIT_MAILBOX ((uint32_t)(1 << 1))

/*!
 * @}
 */

/*!
 * \brief Channel config.
 */
struct mod_optee_smt_channel_config {
    /*! Channel policies */
    uint32_t policies;

    /*! Shared mailbox address */
    uintptr_t mailbox_address;

    /*! Shared mailbox size in bytes */
    size_t mailbox_size;

    /*! Identifier of the driver */
    fwk_id_t driver_id;

    /*! Identifier of the driver API to bind to */
    fwk_id_t driver_api_id;

    /*! Identifier of the power domain that this channel depends on */
    fwk_id_t pd_source_id;
};

/*!
 * \brief Driver API
 */
struct mod_optee_smt_driver_api {
    /*!
     * \brief Raise an interrupt on the receiver
     *
     * \param device_id Device identifier
     *
     * \retval FWK_SUCCESS The operation succeeded
     * \retval FWK_E_PARAM The device_id parameter is invalid
     * \return One of the standard error codes for implementation-defined
     * errors
     */
    int (*raise_interrupt)(fwk_id_t device_id);
};

/*!
 * \brief Driver input API (Implemented by SMT)
 *
 * \details Interface used for driver -> SMT communication.
 */
struct mod_optee_smt_driver_input_api {
    /*!
     * \brief Signal an incoming message in the mailbox
     *
     * \param device_id Channel identifier
     *
     * \retval FWK_SUCCESS The operation succeeded.
     * \return One of the standard error codes for implementation-defined
     * errors.
     */
    int (*signal_message)(fwk_id_t channel_id);
    struct mod_smt_memory * (*get_memory)(fwk_id_t channel_id);

};

/*!
 * \brief Type of the interfaces exposed by the power domain module.
 */
enum mod_optee_smt_api_idx {
    MOD_OPTEE_SMT_API_IDX_DRIVER_INPUT,
    MOD_OPTEE_SMT_API_IDX_SCMI_TRANSPORT,
    MOD_OPTEE_SMT_API_IDX_COUNT,
};

// FIXME: to remove. legacy with vincent work.
#define MOD_SMT_API_IDX_DRIVER_INPUT	MOD_OPTEE_SMT_API_IDX_DRIVER_INPUT
#define MOD_SMT_API_IDX_SCMI_TRANSPORT	MOD_OPTEE_SMT_API_IDX_SCMI_TRANSPORT
#define MOD_HOST_MBX_API_IDX_COUNT	MOD_OPTEE_SMT_API_IDX_COUNT

#endif /* MOD_SGM775_SYSTEM_H */
