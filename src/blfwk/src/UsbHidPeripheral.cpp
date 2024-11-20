/*
 * Copyright (c) 2013-2014 Freescale Semiconductor, Inc.
 * Copyright 2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <unordered_map>

#include "blfwk/UsbHidPeripheral.h"
#include "blfwk/format_string.h"
#include "blfwk/smart_ptr.h"
#include "blfwk/Logging.h"

#include "bootloader_hid_report_ids.h"

using namespace blfwk;

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

UsbHidPeripheral::UsbHidPeripheral(const nv::UsbPeripheralConfigData& cfg) : cfg(cfg) {
    const std::unordered_map<Logger::log_level_t, HID_API_LOG_LVL> blhost2hid_log_levels{
        {Logger::log_level_t::kError, HID_API_LOG_ERROR},
        {Logger::log_level_t::kDebug, HID_API_LOG_DEBUG},
        {Logger::log_level_t::kDebug2, HID_API_LOG_DEBUG2},
    };
    const auto blhostlvl = Log::getLogger()->getFilterLevel();

    hid_api_log_lvl = (blhost2hid_log_levels.count(blhostlvl) > 0)
                    ? blhost2hid_log_levels.at(blhostlvl)
                    : HID_API_LOG_ERROR;

    if (!init()) {
        throw std::runtime_error(format_string("ERROR: UsbHidPeripheral() cannot open USB HID device (%s)\n", cfg.formatted().c_str()));
    }

    hid_api_log_lvl = HID_API_LOG_ERROR;
}

// See UsbHidPeripheral.h for documentation of this method.
bool UsbHidPeripheral::init()
{
    hid_device_cfg hid_cfg;

    hid_cfg.bdi.bus = this->cfg.bdi.bus;
    hid_cfg.bdi.device = this->cfg.bdi.device;
    hid_cfg.bdi.interface = this->cfg.bdi.interface;
    hid_cfg.bdi.valid = this->cfg.bdi.valid;

    hid_cfg.id.vendor_id = this->cfg.usb_id.vendor_id;
    hid_cfg.id.product_id = this->cfg.usb_id.product_id;
    hid_cfg.id.valid = this->cfg.usb_id.valid;

    m_device = hid_open(&hid_cfg);
    if (!m_device)
    {
        return false;
    }

    return true;
}

// See UsbHidPeripheral.h for documentation of this method.
UsbHidPeripheral::~UsbHidPeripheral()
{
    if (m_device)
    {
        hid_close(m_device);
        /* Free static HIDAPI objects. */
        hid_exit();
    }
}

// See UsbHidPeripheral.h for documentation of this method.
status_t UsbHidPeripheral::read(uint8_t *buffer, uint32_t requestedBytes, uint32_t *actualBytes, uint32_t timeout)
{
    assert(buffer);

    // Read the requested number of bytes.
    int count = hid_read_timeout(m_device, buffer, requestedBytes, timeout);
    if (actualBytes)
    {
        *actualBytes = count;
    }

    if (Log::getLogger()->getFilterLevel() == Logger::kDebug2)
    {
        // Log bytes read in hex
        Log::debug2("<");
        for (int i = 0; i < (int)count; i++)
        {
            Log::debug2("%02x", buffer[i]);
            if (i != (count - 1))
            {
                Log::debug2(" ");
            }
        }
        Log::debug2(">\n");
    }

    // Bail if we got an error (-1), or if the number of bytes read was less than
    // the report header.
    if (count < (int)sizeof(bl_hid_header_t))
    {
        if (count == -1)
        {
            return kStatus_Fail;
        }
        else
        {
            return kStatus_Timeout;
        }
    }

    return kStatus_Success;
}

// See UsbHidPeripheral.h for documentation of this method.
status_t UsbHidPeripheral::write(const uint8_t *buffer, uint32_t byteCount, uint32_t timeoutMS)
{
    assert(buffer);

    if (Log::getLogger()->getFilterLevel() == Logger::kDebug2)
    {
        // Log bytes written in hex
        Log::debug2("[");
        for (uint32_t i = 0; i < byteCount; i++)
        {
            Log::debug2("%02x", buffer[i]);
            if (i != (byteCount - 1))
            {
                Log::debug2(" ");
            }
        }
        Log::debug2("]\n");
    }

    int count = hid_write_timeout(m_device, buffer, byteCount, timeoutMS);
    if (count < 0)
    {
        const wchar_t *errorMessage = hid_error(m_device);
        if (errorMessage)
        {
            int len = wcslen(errorMessage);
            char *msg = new char[len + 1];
            wcstombs(msg, errorMessage, len + 1);
            Log::error("%s", msg);
            delete[] msg;
        }
        return kStatus_Fail;
    }

    return kStatus_Success;
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
