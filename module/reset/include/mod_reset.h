/*
 * Arm SCP/MCP Software
 * Copyright (c) 2015-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Description:
 *     Power domain management support.
 */

#ifndef MOD_RESET_DOMAIN_H
#define MOD_RESET_DOMAIN_H

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <fwk_element.h>
#include <fwk_id.h>
#include <fwk_module_idx.h>

/*!
 * \addtogroup GroupModules Modules
 * @{
 */

/*!
 * \defgroup GroupResetDomain Reset Domains
 *
 * \details Support for setting the state of reset domains.
 *
 * @{
 */

/*!
 * \brief Reset module configuration data.
 */
struct mod_reset_config {
    /*!
     * \brief Identifier of a notification to subscribe reset domain devices to
     *     in order to receive notifications of domain reset that have already
     *     occurred.
     *
     * \note May be \ref FWK_ID_NONE to disable this functionality for all
     *     elements.
     *
     *  TODO: this is not yet implemented in the driver.
     */
    const fwk_id_t notification_id;
};

/*!
 * \brief Reset domain element configuration data.
 */
struct mod_reset_dev_config {
    /*! Reference to the device element within the associated driver module */
    const fwk_id_t driver_id;

    /*! Reference to the API provided by the device driver module */
    const fwk_id_t api_id;
};

/*!
 * \brief Reset domain driver interface.
 *
 * \details The interface the reset domain module relies on to perform
 *      actions on a reset domain.
 */

struct mod_reset_drv_api {
    /*! Name of the driver */
    const char *name;

    /*!
     * \brief Target reset domain identified by \p dev_id enters reset state.
     *
     * \param dev_id Driver identifier of the reset domain.
     * \param state Target reset state to reach.
     *
     * \retval FWK_SUCCESS or one of FWK_E_* error codes.
     */
    int (*reset_assert)(fwk_id_t dev_id, unsigned int state);

    /*!
     * \brief Target reset domain identified by \p dev_id exits reset state.
     *
     * \param dev_id Driver identifier of the reset domain.
     *
     * \retval FWK_SUCCESS or one of FWK_E_* error codes.
     */
    int (*reset_deassert)(fwk_id_t dev_id);
};

/*!
 * @}
 */

/*!
 * @}
 */

#endif /* MOD_RESET_DOMAIN_H */
