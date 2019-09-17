/*
 * Arm SCP/MCP Software
 * Copyright (c) 2015-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Description:
 *     Resource reset management support.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <fwk_assert.h>
#include <fwk_element.h>
#include <fwk_errno.h>
#include <fwk_id.h>
#include <fwk_macros.h>
#include <fwk_mm.h>
#include <fwk_module.h>
#include <fwk_module_idx.h>
#include <fwk_thread.h>
#include <fwk_multi_thread.h>
#include <fwk_notification.h>
#include <mod_log.h>
#include <mod_reset.h>

/*
 * Module and devices contexts for Reset Domain
 * TODO: notification
 */

/* Device context */
struct reset_dev_ctx {
    const struct mod_reset_dev_config *config;
    struct mod_reset_drv_api *driver_api;
};

/* Module context */
struct mod_reset_ctx {
    const struct mod_reset_config *config;
    struct reset_dev_ctx *dev_ctx_table;
    unsigned int dev_count;
    const struct mod_log_api *log_api;
};

/*
 * Internal variables
 */
static struct mod_reset_ctx module_ctx;

/*
 * API functions
 */
static int reset_assert(fwk_id_t reset_id, unsigned int state)
{
    int status = 0;
    struct reset_dev_ctx *reset_ctx = NULL;

    status = fwk_module_check_call(reset_id);
    if (status != FWK_SUCCESS)
        return status;

    reset_ctx = &module_ctx.dev_ctx_table[fwk_id_get_element_idx(reset_id)];

    return reset_ctx->driver_api->reset_assert(reset_ctx->config->driver_id,
					       state);
}

static int reset_deassert(fwk_id_t reset_id)
{
    int status = 0;
    struct reset_dev_ctx *reset_ctx = NULL;

    status = fwk_module_check_call(reset_id);
    if (status != FWK_SUCCESS)
        return status;

    reset_ctx = &module_ctx.dev_ctx_table[fwk_id_get_element_idx(reset_id)];

    return reset_ctx->driver_api->reset_deassert(reset_ctx->config->driver_id);
}

static const struct mod_reset_drv_api reset_api = {
    .reset_assert = reset_assert,
    .reset_deassert = reset_deassert,
};

/*
 * Framework handlers
 */
static int reset_init(fwk_id_t module_id, unsigned int dev_count,
		      const void *data)
{
    module_ctx.dev_count = dev_count;

    module_ctx.config = (struct mod_reset_config *)data;
    module_ctx.dev_ctx_table = fwk_mm_calloc(dev_count,
                                             sizeof(struct reset_dev_ctx));
    if (module_ctx.dev_ctx_table == NULL)
        return FWK_E_NOMEM;

    return FWK_SUCCESS;
}

static int reset_elt_init(fwk_id_t elt_id, unsigned int dev_count,
		          const void *data)
{
    struct reset_dev_ctx *reset_ctx = NULL;

    reset_ctx = &module_ctx.dev_ctx_table[fwk_id_get_element_idx(elt_id)];
    reset_ctx->config = (const struct mod_reset_dev_config *)data;

    return FWK_SUCCESS;
}


static int reset_bind(fwk_id_t id, unsigned int round)
{
    struct reset_dev_ctx *reset_ctx = NULL;

    /* Nothing to do but during the first round of calls */
    if (round != 0) {
        return FWK_SUCCESS;
    }

    if (fwk_id_is_type(id, FWK_ID_TYPE_MODULE)) {
        return fwk_module_bind(FWK_ID_MODULE(FWK_MODULE_IDX_LOG),
                               FWK_ID_API(FWK_MODULE_IDX_LOG, 0),
			       &module_ctx.log_api);
    }

    if (!fwk_id_is_type(id, FWK_ID_TYPE_ELEMENT)) {
        return FWK_SUCCESS;
    }

    reset_ctx = module_ctx.dev_ctx_table + fwk_id_get_element_idx(id);

    return fwk_module_bind(reset_ctx->config->driver_id,
			   reset_ctx->config->api_id,
			   &reset_ctx->driver_api);
}

static int reset_process_bind_request(fwk_id_t source_id, fwk_id_t target_id,
                                      fwk_id_t api_id, const void **api)
{
    if (fwk_id_get_api_idx(api_id) != MOD_RESET_API_TYPE_HAL) {
        return FWK_E_ACCESS;
    }

    *api = &reset_api;

    return FWK_SUCCESS;
}

const struct fwk_module module_reset = {
    .name = "Reset domain",
    .type = FWK_MODULE_TYPE_HAL,
    .api_count = MOD_RESET_API_COUNT,
    .init = reset_init,
    .element_init = reset_elt_init,
    .bind = reset_bind,
    .process_bind_request = reset_process_bind_request,
};
