/*
 * Copyright (c) 2013-2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _utils_h_
#define _utils_h_

#include <string>
#include <vector>

namespace utils
{
//! @brief Split a string into tokens by delimiter.
std::vector<std::string> string_split(const std::string &s, char delim);

//! @brief Remove all except hex digits from a string.
std::string string_hex(const std::string &s);

//! @brief Check if a string is signed a number. If so, return number.
bool stringtoi(const std::string &s, int32_t &number);

//! @brief Check if a string is an unsigned number. If so, return number.
bool stringtoui(const std::string &s, uint32_t &number, int base = 0);

//! @brief Format bytes into GB, MB, KB, or bytes.
std::string scale_bytes(uint64_t sizeInBytes);

} // namespace utils

namespace nv_utils {
bool capture_vendor_product_id(const std::string& str, std::string& vidstr, std::string& pidstr);

bool capture_bus_device_interface(const std::string& str, std::string& bus, std::string& device, std::string& interface);

bool file_exists(std::string str);

//! @brief Check if a string is an unsigned number. If so, return number.
template <typename Tuint>
bool str_to_uint(const std::string& s, Tuint& number, int base = 0);

std::string uint_to_hex(uint64_t num, uint8_t fill_digits = 0);

}

#endif // _utils_h_

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
