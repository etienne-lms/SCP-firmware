#
# Arm SCP/MCP Software
# Copyright (c) 2015-2019, Arm Limited and Contributors. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
# The order of the modules in the BS_FIRMWARE_MODULES list is the order in which
# the modules are initialized, bound, started during the pre-runtime phase.
#

BS_FIRMWARE_CPU := optee
BS_FIRMWARE_HAS_MULTITHREADING := no
BS_FIRMWARE_HAS_NOTIFICATION := no

OPTEE_PLATFORM := vexpress

DEFINES += CFG_WITH_SCMI

BS_FIRMWARE_MODULE_HEADERS_ONLY := \
    stubs \
    power_domain

ifeq ($(OPTEE_PLATFORM),juno)
BS_FIRMWARE_MODULE_HEADERS_ONLY += \
    css_clock \
    pik_clock
endif

BS_FIRMWARE_MODULES_GENERIC := \
    log scmi spci hmbx

BS_FIRMWARE_MODULES-juno := \
    scmi_clock clock vpll

BS_FIRMWARE_MODULES-vexpress := \
    scmi_clock clock dummy_clock

BS_FIRMWARE_MODULES-stm32mp1 := \
    scmi_clock clock stm32_clock

BS_FIRMWARE_MODULES := \
    $(BS_FIRMWARE_MODULES_GENERIC) \
    $(BS_FIRMWARE_MODULES-$(OPTEE_PLATFORM))

BS_FIRMWARE_SOURCES_GENERIC := \
    config_log.c \
    config_hmbx.c \
    config_scmi.c

BS_FIRMWARE_SOURCES-juno := \
    config_vpll.c \
    config_clock.c \
    config_scmi_clock.c

BS_FIRMWARE_SOURCES-vexpress := \
    config_vexpress.c

BS_FIRMWARE_SOURCES-stm32mp& := \
    config_stm32mp1.c

BS_FIRMWARE_SOURCES := \
    $(BS_FIRMWARE_SOURCES_GENERIC) \
    $(BS_FIRMWARE_SOURCES-$(OPTEE_PLATFORM))

include $(BS_DIR)/lib-firmware.mk
