#
# Arm SCP/MCP Software
# Copyright (c) 2015-2019, Arm Limited and Contributors. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
# The order of the modules in the BS_FIRMWARE_MODULES list is the order in which
# the modules are initialized, bound, started during the pre-runtime phase.
#

BS_ARCH_CPU := cortex-a53
export BS_ARCH_CPU

BS_FIRMWARE_CPU := optee
BS_FIRMWARE_HAS_MULTITHREADING := no
BS_FIRMWARE_HAS_NOTIFICATION := no

BS_FIRMWARE_MODULE_HEADERS_ONLY := \
    optee_stubs \
    power_domain \
    css_clock \
    pik_clock

BS_FIRMWARE_MODULES := \
    log optee_mhu optee_smt scmi \
    vpll clock scmi_clock

BS_FIRMWARE_SOURCES := \
    config_log.c \
    config_mhu.c \
    config_smt.c \
    config_scmi.c \
    config_vpll.c \
    config_clock.c \
    config_scmi_clock.c \
    config_power_domain.c

include $(BS_DIR)/lib-firmware.mk
