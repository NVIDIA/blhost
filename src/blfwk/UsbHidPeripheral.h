/*
 * Copyright (c) 2013-2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _UsbHidPeripheral_h_
#define _UsbHidPeripheral_h_

#include "Peripheral.h"
#include "hidapi.h"

//! @addtogroup host_usb_hid_peripheral
//! @{

namespace blfwk
{
/*!
 * @brief Represents a USB HID peripheral.
 *
 * Interface class for objects that provide the source for commands or sink for responses.
 */
class UsbHidPeripheral : public Peripheral
{
public:
    //! @brief Constants
    enum _usbhid_contants
    {
        kDefault_Vid = 0x15a2, //!< Freescale VID
        kDefault_Pid = 0x0073, //!< PID for KL25Z48M
        //kK32H_Pid = 0x0083    //!< PID for K32H (Ultra)
        kK32H_Pid = 0x0052    //!< test pid (mx50)
    };

public:
    UsbHidPeripheral(const nv::UsbPeripheralConfigData& cfg);

    //! @brief Destructor.
    virtual ~UsbHidPeripheral();

    //! @brief Read bytes.
    //!
    //! @param buffer Pointer to buffer
    //! @param requestedBytes Number of bytes to read
    //! @param timeoutMs Time in milliseconds to wait for read to complete.
    //! @param actualBytes Number of bytes actually read.
    virtual status_t read(uint8_t *buffer, uint32_t requestedBytes, uint32_t *actualBytes, uint32_t timeoutMS);

    //! @brief Write bytes. This is a do nothing function implemented here to satisfy abstract base class
    //! requirements. This function is not used. The write(buffer, count, timeout) function is used
    //! in this child class instead of the write(buffer, cout) function declared in the base class.
    virtual status_t write(const uint8_t *buffer, uint32_t byteCount) { return kStatus_Success; }

    //! @brief Return peripheral Type
    virtual _host_peripheral_types get_type(void) { return kHostPeripheralType_USB_HID; }

    //! @brief Write bytes.
    //!
    //! @param buffer Pointer to buffer to write
    //! @param byteCount Number of bytes to write
    //! @param timeoutMs Time in milliseconds to wait for write to complete.
    status_t write(const uint8_t *buffer, uint32_t byteCount, uint32_t timeoutMS);
private:
    //! @brief Initialize.
    //!
    //! Opens the HID device.
    bool init();


    nv::UsbPeripheralConfigData cfg;
    hid_device *m_device; //!< Device handle.
};

} // namespace blfwk

//! @}

#endif // _UsbHidPeripheral_h_

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
