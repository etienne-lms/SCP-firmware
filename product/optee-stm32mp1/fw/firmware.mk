#
# Arm SCP/MCP Software
# Copyright (c) 2019, Arm Limited and Contributors. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
# The order of the modules in the BS_FIRMWARE_MODULES list is the order in which
# the modules are initialized, bound, started during the pre-runtime phase.
#

BS_ARCH_CPU := cortex-a7
export BS_ARCH_CPU

BS_FIRMWARE_CPU := optee
BS_FIRMWARE_HAS_MULTITHREADING := no
BS_FIRMWARE_HAS_NOTIFICATION := no

BS_FIRMWARE_MODULE_HEADERS_ONLY := \
    power_domain

BS_FIRMWARE_MODULES := \
    log optee_mhu optee_smt scmi \
    scmi_clock clock stm32_clock \
    scmi_reset_domain reset_domain stm32_reset

BS_FIRMWARE_SOURCES := \
    config_log.c \
    config_mhu_smt.c \
    config_scmi.c \
    config_clock.c \
    config_reset_domain.c

include $(BS_DIR)/lib-firmware.mk
