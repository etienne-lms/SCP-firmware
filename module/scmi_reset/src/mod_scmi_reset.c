/*
 * Arm SCP/MCP Software
 * Copyright (c) 2015-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Description:
 *     SCMI power domain management protocol support.
 */

#include <string.h>
#include <fwk_assert.h>
#include <fwk_element.h>
#include <fwk_errno.h>
#include <fwk_id.h>
#include <fwk_macros.h>
#include <fwk_module.h>
#include <fwk_module_idx.h>
#include <internal/scmi.h>
#include <internal/scmi_reset.h>
#include <mod_log.h>
#include <mod_reset.h>
#include <mod_scmi.h>


struct scmi_reset_ctx {
    /* Number of power domains */
    unsigned int domain_count;
    /* Log module API */
    const struct mod_log_api *log_api;
    /* SCMI module API */
    const struct mod_scmi_from_protocol_api *scmi_api;
    /* Reset module API */
    const struct mod_reset_drv_api *reset_api;
};

static int protocol_version_handler(fwk_id_t service_id,
                                    const uint32_t *payload);

static int protocol_attributes_handler(fwk_id_t service_id,
                                       const uint32_t *payload);

static int protocol_message_attributes_handler(fwk_id_t service_id,
                                               const uint32_t *payload);

static int reset_attributes_handler(fwk_id_t service_id,
                                    const uint32_t *payload);

static int reset_request_handler(fwk_id_t service_id,
                                 const uint32_t *payload);

static int reset_notify_handler(fwk_id_t service_id,
                                const uint32_t *payload);

/*
 * Internal variables
 */

static struct scmi_reset_ctx scmi_reset_ctx;

static int (*msg_handler_table[])(fwk_id_t, const uint32_t *) = {
    [SCMI_PROTOCOL_VERSION] = protocol_version_handler,
    [SCMI_PROTOCOL_ATTRIBUTES] = protocol_attributes_handler,
    [SCMI_PROTOCOL_MESSAGE_ATTRIBUTES] = protocol_message_attributes_handler,
    [SCMI_RESET_DOMAIN_ATTRIBUTES] = reset_attributes_handler,
    [SCMI_RESET_REQUEST] = reset_request_handler,
    [SCMI_RESET_NOTIFY] = reset_notify_handler,
};

static unsigned int payload_size_table[] = {
    [SCMI_PROTOCOL_VERSION] = 0,
    [SCMI_PROTOCOL_ATTRIBUTES] = 0,
    [SCMI_PROTOCOL_MESSAGE_ATTRIBUTES] =
                       sizeof(struct scmi_protocol_message_attributes_a2p),
    [SCMI_RESET_DOMAIN_ATTRIBUTES] = sizeof(struct scmi_reset_attributes_a2p),
    [SCMI_RESET_REQUEST] = sizeof(struct scmi_reset_request_a2p),
    [SCMI_RESET_NOTIFY] = sizeof(struct scmi_reset_notify_a2p),
};

/*
 * Reset domain management protocol implementation
 */

static int protocol_version_handler(fwk_id_t service_id,
                                    const uint32_t *payload)
{
    struct scmi_protocol_version_p2a outmsg = {
        .status = SCMI_SUCCESS,
        .version = SCMI_PROTOCOL_VERSION_RESET_DOMAIN,
    };

    scmi_reset_ctx.scmi_api->respond(service_id, &outmsg, sizeof(outmsg));

    return FWK_SUCCESS;
}

static int protocol_attributes_handler(fwk_id_t service_id,
                                       const uint32_t *payload)
{
    struct scmi_reset_protocol_attributes_p2a outmsg = {
        .status = SCMI_SUCCESS,
    };

    outmsg.attributes = scmi_reset_ctx.domain_count;

    scmi_reset_ctx.scmi_api->respond(service_id, &outmsg, sizeof(outmsg));

    return FWK_SUCCESS;
}

static int protocol_message_attributes_handler(fwk_id_t service_id,
                                               const uint32_t *payload)
{
    struct scmi_protocol_message_attributes_p2a outmsg = {
        .status = SCMI_NOT_FOUND,
    };
    size_t outmsg_size = sizeof(outmsg.status);
    struct scmi_protocol_message_attributes_a2p params;

    params = *(const struct scmi_protocol_message_attributes_a2p *)payload;

    if ((params.message_id < FWK_ARRAY_SIZE(msg_handler_table)) &&
        msg_handler_table[params.message_id]) {
        outmsg.status = SCMI_SUCCESS;
        outmsg_size = sizeof(outmsg);
    }

    scmi_reset_ctx.scmi_api->respond(service_id, &outmsg, outmsg_size);

    return FWK_SUCCESS;
}

static int reset_attributes_handler(fwk_id_t service_id,
                                    const uint32_t *payload)
{
    struct scmi_reset_attributes_a2p params;
    struct scmi_reset_attributes_p2a outmsg = {
        .status = SCMI_GENERIC_ERROR,
    };
    size_t outmsg_size = sizeof(outmsg.status);
    fwk_id_t reset_id;
    unsigned int agent_id;
    enum scmi_agent_type agent_type;
    int status = FWK_SUCCESS;

    params = *(const struct scmi_reset_attributes_a2p *)payload;

    if (params.domain_id > UINT16_MAX) {
        outmsg.status = SCMI_NOT_FOUND;
        goto exit;
    }

    reset_id = FWK_ID_ELEMENT(FWK_MODULE_IDX_RESET, params.domain_id);
    if (!fwk_module_is_valid_element_id(reset_id)) {
        outmsg.status = SCMI_NOT_FOUND;
        goto exit;
    }

    /*
     * TODO: get Async/Sync support info from module reset
     * Currently: no support for async reset, no support for notification
     */
    outmsg.flags &= ~SCMI_RESET_DOMAIN_ATTR_ASYNC;
    outmsg.flags &= ~SCMI_RESET_DOMAIN_ATTR_NOTIF;

    /*
     * TODO: Get reset latency from reset module configuration
     * Currently: set to unknown value
     */
    outmsg.latency = SCMI_RESET_DOMAIN_ATTR_UNK_LAT;

    strncpy((char *)outmsg.name, fwk_module_get_name(reset_id),
            sizeof(outmsg.name) - 1);

    outmsg.status = SCMI_SUCCESS;
    outmsg_size = sizeof(outmsg);

exit:
    scmi_reset_ctx.scmi_api->respond(service_id, &outmsg, outmsg_size);

    return status;
}

static int reset_request_handler(fwk_id_t service_id,
                                 const uint32_t *payload)
{
    int status = FWK_SUCCESS;
    struct scmi_reset_request_a2p params;
    struct scmi_reset_request_p2a outmsg = {
        .status = SCMI_NOT_FOUND
    };
    size_t outmsg_size = sizeof(outmsg.status);
    fwk_id_t reset_id;
    bool async_reset = false;
    bool assert_reset = false;
    bool deassert_reset = false;
    const struct mod_reset_drv_api *reset_api = NULL;

    params = *(const struct scmi_reset_request_a2p *)payload;

    if (params.domain_id >= SCMI_RESET_DOMAIN_COUNT_MASK)
        goto exit;

    reset_id = FWK_ID_ELEMENT(FWK_MODULE_IDX_RESET, params.domain_id);
    if (!fwk_module_is_valid_element_id(reset_id))
        goto exit;

    async_reset = params.flags & SCMI_RESET_DOMAIN_ASYNC;
    assert_reset = params.flags &
	           (SCMI_RESET_DOMAIN_ASSERT | SCMI_RESET_DOMAIN_AUTO);
    deassert_reset = (params.flags & SCMI_RESET_DOMAIN_AUTO) ||
                     !(params.flags & SCMI_RESET_DOMAIN_ASSERT);

    if (async_reset) {
        outmsg.status = SCMI_NOT_SUPPORTED;
	goto exit;
    }

    reset_api = scmi_reset_ctx.reset_api;

    if (assert_reset)
	status = reset_api->reset_assert(reset_id, params.reset_state);
    if (status != FWK_SUCCESS)
        goto exit;

    if (deassert_reset)
	status = reset_api->reset_deassert(reset_id);
    if (status != FWK_SUCCESS)
        goto exit;

    outmsg.status = SCMI_SUCCESS;
    outmsg_size = sizeof(outmsg);

exit:
    scmi_reset_ctx.scmi_api->respond(service_id, &outmsg, outmsg_size);

    return status;
}

static int reset_notify_handler(fwk_id_t service_id,
                                const uint32_t *payload)
{
    struct scmi_reset_notify_p2a outmsg = {
        .status = SCMI_NOT_SUPPORTED
    };
    size_t outmsg_size = sizeof(outmsg.status);

    scmi_reset_ctx.scmi_api->respond(service_id, &outmsg, outmsg_size);

    return FWK_SUCCESS;
}

/*
 * SCMI module -> SCMI power module interface
 */
static int scmi_reset_get_scmi_protocol_id(fwk_id_t protocol_id,
                                           uint8_t *scmi_protocol_id)
{
    int status;

    status = fwk_module_check_call(protocol_id);
    if (status != FWK_SUCCESS)
        return status;

    *scmi_protocol_id = SCMI_PROTOCOL_ID_RESET_DOMAIN;

    return FWK_SUCCESS;
}

static int scmi_reset_message_handler(fwk_id_t protocol_id,
				      fwk_id_t service_id,
                                      const uint32_t *payload,
				      size_t payload_size,
				      unsigned int message_id)
{
    int status;
    int32_t return_value;

    status = fwk_module_check_call(protocol_id);
    if (status != FWK_SUCCESS)
        return status;

    static_assert(FWK_ARRAY_SIZE(msg_handler_table) ==
        FWK_ARRAY_SIZE(payload_size_table),
        "[SCMI] Power domain management protocol table sizes not consistent");

    assert(payload != NULL);

    if (message_id >= FWK_ARRAY_SIZE(msg_handler_table)) {
        return_value = SCMI_NOT_SUPPORTED;
        goto error;
    }

    if (payload_size != payload_size_table[message_id]) {
        return_value = SCMI_PROTOCOL_ERROR;
        goto error;
    }

    return msg_handler_table[message_id](service_id, payload);

error:
    scmi_reset_ctx.scmi_api->respond(service_id,
                                     &return_value, sizeof(return_value));
    return FWK_SUCCESS;
}

static struct mod_scmi_to_protocol_api scmi_reset_mod_scmi_to_protocol_api = {
    .get_scmi_protocol_id = scmi_reset_get_scmi_protocol_id,
    .message_handler = scmi_reset_message_handler
};

/*
 * Framework handlers
 */

static int scmi_reset_init(fwk_id_t module_id, unsigned int element_count,
                           const void *unused)
{
    if (element_count != 0)
        return FWK_E_SUPPORT;

    scmi_reset_ctx.domain_count =
	    fwk_module_get_element_count(fwk_module_id_reset);

    if (scmi_reset_ctx.domain_count <= 1 ||
        scmi_reset_ctx.domain_count <= UINT16_MAX)
        return FWK_E_SUPPORT;

    return FWK_SUCCESS;
}

static int scmi_reset_bind(fwk_id_t id, unsigned int round)
{
    int status;

    if (round == 1)
        return FWK_SUCCESS;

    status = fwk_module_bind(FWK_ID_MODULE(FWK_MODULE_IDX_LOG),
                             FWK_ID_API(FWK_MODULE_IDX_LOG, 0),
			     &scmi_reset_ctx.log_api);
    if (status != FWK_SUCCESS)
        return status;

    return fwk_module_bind(FWK_ID_MODULE(FWK_MODULE_IDX_SCMI),
                           FWK_ID_API(FWK_MODULE_IDX_SCMI,
				      MOD_SCMI_API_IDX_PROTOCOL),
                           &scmi_reset_ctx.scmi_api);
}

static int scmi_reset_process_bind_request(fwk_id_t source_id, fwk_id_t target_id,
                                           fwk_id_t api_id, const void **api)
{
    if (!fwk_id_is_equal(source_id, FWK_ID_MODULE(FWK_MODULE_IDX_SCMI)))
        return FWK_E_ACCESS;

    *api = &scmi_reset_mod_scmi_to_protocol_api;

    return FWK_SUCCESS;
}

/* SCMI Reset Domain Management Protocol Definition */
const struct fwk_module module_scmi_reset = {
    .name = "SCMI Reset Domain Management Protocol",
    .api_count = 1,
    .type = FWK_MODULE_TYPE_PROTOCOL,
    .init = scmi_reset_init,
    .bind = scmi_reset_bind,
    .process_bind_request = scmi_reset_process_bind_request,
};

/* No elements, no module configuration data */
//struct fwk_module_config config_scmi_reset = { 0 };
