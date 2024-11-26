/*
 * Copyright (c) 2013-2014 Freescale Semiconductor, Inc.
 * Copyright 2020 - 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _Peripheral_h_
#define _Peripheral_h_

#include <sstream>
#include <string>
#include <string.h>
#include "BusPal.h"
#if defined(LPCUSBSIO)
#include "LpcUsbSio.h"
#endif
#include "bootloader_common.h"

#include "blfwk/utils.h"

//! @addtogroup host_peripherals
//! @{

namespace nv {
// bus:device.interface representation
struct NV_UsbBDI {
    uint8_t bus;
    uint8_t device;
    uint8_t interface = 0;
    bool valid = false; // no std::optional :(

    std::string formatted() const {
        constexpr size_t len = sizeof("xxx:xxx.xx");
        char cstr[len];

        snprintf(cstr, len, "%03u:%03u.%02u", bus, device, interface);
        return std::string{cstr};
    }
};

// vendor_id:product_id
struct NV_UsbID {
    uint16_t vendor_id;
    uint16_t product_id;
    bool valid = false;

    std::string formatted() const {
        constexpr size_t len = sizeof("xxxx:xxxx");
        char cstr[len];

        snprintf(cstr, len, "%04x:%04x", vendor_id, product_id);

        return std::string{cstr};
    }
};

struct UsbPeripheralConfigData {
    NV_UsbBDI bdi;
    NV_UsbID usb_id;

    std::string formatted() const {
        std::stringstream sstream;

        sstream << "USB Info: ";
        if (bdi.valid) {
            sstream << "bus:device.interface = " << bdi.formatted();
        }

        if (usb_id.valid) {
            if (bdi.valid) sstream << ", ";
            sstream << "vendor:device = " << usb_id.formatted();
        }

        return sstream.str();
    }
};

} // namespace nv

namespace blfwk
{
/*!
 * @brief Represents a peripheral.
 *
 * Interface class for objects that provide the source for commands or sink for responses.
 */
class Peripheral
{
public:
    enum _host_peripheral_types
    {
        kHostPeripheralType_None,
        kHostPeripheralType_UART,
        kHostPeripheralType_BUSPAL_UART,
        kHostPeripheralType_LPCUSBSIO,
        kHostPeripheralType_USB_HID,
        kHostPeripheralType_SIM,
        kHostPeripheralType_I2C,
        kHostPeripheralType_SPI,
    };

    // Why is this defined within Peripheral???? makes no sense...
    struct PeripheralConfigData
    {
        _host_peripheral_types peripheralType;
        bool ping;
        std::string comPortName;
        long comPortSpeed;
        uint32_t packetTimeoutMs;
        nv::UsbPeripheralConfigData usb_cfg;
#if defined(LINUX) && defined(__ARM__)
        unsigned char i2cAddress;
        unsigned char spiPolarity;
        unsigned char spiPhase;
        unsigned char spiSequence;
#endif // #if defined(LINUX) && defined(__ARM__)
        BusPal::BusPalConfigData busPalConfig;
#if defined(LPCUSBSIO)
        LpcUsbSio::LpcUsbSioConfigData lpcUsbSioConfig;
#endif

        std::string str() const {
            std::string str{"PeripheralConfigData"};

            switch (this->peripheralType) {
            case _host_peripheral_types::kHostPeripheralType_USB_HID: {
                std::stringstream stream;
                stream << "(" << this->usb_cfg.formatted() << ")";
                str.append(stream.str());
            }
                break;
            default:
                break;
            }

            return str;
        }

    };

    virtual ~Peripheral(){};

    //! @brief Read bytes.
    //!
    //! @param buffer Pointer to buffer
    //! @param requestedBytes Number of bytes to read
    //! @param timeoutMs Time in milliseconds to wait for read to complete.
    //! @param actualBytes Number of bytes actually read.
    virtual status_t read(uint8_t *buffer, uint32_t requestedBytes, uint32_t *actualBytes, uint32_t timeout) = 0;

    //! @brief Write bytes.
    virtual status_t write(const uint8_t *buffer, uint32_t byteCount) = 0;

    //! @brief Return peripheral Type
    virtual _host_peripheral_types get_type(void) = 0;
};

} // namespace blfwk

//! @}

#endif // _Peripheral_h_

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
