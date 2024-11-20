/*
 * Copyright (c) 2013-2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <sstream>
#include <string>
#include <vector>
#include <stdint.h>
#include "blfwk/format_string.h"
#ifdef LINUX
#include <stdlib.h>
#endif


namespace utils
{
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems)
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim))
    {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> string_split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

std::string string_hex(const std::string &s)
{
    std::string hex;
    std::string::const_iterator it = s.begin();
    for (; it != s.end(); ++it)
    {
        if (isxdigit(*it))
        {
            hex.append(1, *it);
        }
    }

    return hex;
}

bool stringtoi(const std::string &s, int32_t &number)
{
    if (s.empty())
        return false;

    char *p;
    int64_t temp;
    temp = strtoll(s.c_str(), &p, 0);
    if ((temp > INT32_MAX) || (temp < INT32_MIN))
    {
        return false;
    }
    number = static_cast<int32_t>(temp);
    return (p != NULL) && (*p == 0);
}

bool stringtoui(const std::string &s, uint32_t &number, int base = 0)
{
    if (s.empty())
        return false;
    if (s[0] == '-')
        return false;

    char *p;
    uint64_t temp;
    temp = strtoull(s.c_str(), &p, base);
    if (temp > UINT32_MAX)
    {
        return false;
    }
    number = static_cast<uint32_t>(temp);
    return (p != NULL) && (*p == 0);
}

// GB, MB, KB, bytes
std::string scale_bytes(uint64_t sizeInBytes)
{
    double originalSize = (double)sizeInBytes;
    double scaledSize = 0;

    scaledSize = originalSize / (1024 * 1024 * 1024);
    if (scaledSize >= 1.0) // GB
    {
        if (sizeInBytes % (1024 * 1024 * 1024))
            return format_string("%.3f GB", scaledSize);
        else
            return format_string("%.f GB", scaledSize);
    }

    scaledSize = originalSize / (1024 * 1024);
    if (scaledSize >= 1.0) // MB
    {
        if (sizeInBytes % (1024 * 1024))
            return format_string("%.3f MB", scaledSize);
        else
            return format_string("%.f MB", scaledSize);
    }

    scaledSize = originalSize / 1024;
    if (scaledSize >= 1.0) // KB
    {
        if (sizeInBytes % 1024)
            return format_string("%.3f KB", scaledSize);
        else
            return format_string("%.f KB", scaledSize);
    }
    else // bytes
    {
        return format_string("%d bytes", sizeInBytes);
    }

} // ScaleBytes()

} // namespace utils

/// -------------------------------------------------- NV changes

#include <fstream>
#include <limits>
#include <regex>
#include "Logging.h"

namespace nv_utils {
// (?:0x)? matches 0 or 1 of `0x` without capturing it
// [A-Fa-f0-9]+ matches a hexadecimal literal with at least one digit
const std::string matches_hex_literal{"(?:0x)?[A-Fa-f0-9]+"};

// [:,]{1} matches 1 of : or ,
const std::string matches_separator_captured{"([:,]{1})"};

bool capture_vendor_product_id(const std::string& str, std::string& vidstr, std::string& pidstr) {
    std::stringstream sstream;
    sstream << matches_hex_literal << matches_separator_captured << matches_hex_literal;

    // In all, this will match any of the following: `0x1fc9:0x0158`, `0x1fc9,0x0158` where 0x is optional
    const std::regex vendor_product_regex(sstream.str());
    std::smatch match;

    if (!std::regex_match(str, match, vendor_product_regex)) {
        return false;
    }

    const std::string separator = match[1].str();

    auto id_parts = utils::string_split(str, separator[0]);
    vidstr = id_parts[0];
    pidstr = id_parts[1];

    return true;
}

bool capture_bus_device_interface(const std::string& str, std::string& bus, std::string& device, std::string& interface) {
    std::stringstream sstream;

    // Matches [hex]:[hex]
    sstream << matches_hex_literal << matches_separator_captured << matches_hex_literal;
    const std::regex bdi_without_interface_regex(sstream.str());

    // matches [hex]:[hex].[hex]
    sstream << "." << matches_hex_literal;
    const std::regex bdi_with_iface_regex(sstream.str());

    std::smatch match;

    // Try to match the interface first, since it is an optional suffix ([bus]:[device].[interface])
    interface = "";
    if (std::regex_match(str, match, bdi_with_iface_regex)) {
       auto bdi_parts = utils::string_split(str, '.');
       interface = bdi_parts[1];
    }

    // Fail if bus/device not present since those are NOT optional
    if (!std::regex_match(str, match, bdi_without_interface_regex)) {
        return false;
    }

    const std::string separator = match[1].str();

    // The string optionally contains the interface, so we use the match instead to parse the bus:device
    auto bdi_parts = utils::string_split(match[0].str(), separator[0]);
    bus = bdi_parts[0];
    device = bdi_parts[1];

    return true;
}

bool file_exists(std::string str) {
    std::ifstream stream(str);
    return stream.good();
}

template <typename Tuint>
bool str_to_uint(const std::string &s, Tuint &number, int base = 0) {
    uint32_t u32num = 0;

    bool res = utils::stringtoui(s, u32num, base);

    if (u32num > std::numeric_limits<Tuint>::max()) {
        return false;
    }

    number = static_cast<Tuint>(u32num);
    return res;
}

template bool str_to_uint<uint16_t>(const std::string &s, uint16_t& number, int base);
template bool str_to_uint<uint8_t>(const std::string &s, uint8_t& number, int base);

std::string uint_to_hex(uint64_t num, uint8_t fill_digits) {
    std::stringstream stream;
    stream << std::hex << num;

    std::string raw_hex_str{stream.str()};

    std::string res_hex_str{"0x"};
    size_t actual_fill = fill_digits;
    if (raw_hex_str.size() < actual_fill) {
        actual_fill -= raw_hex_str.size();
        for (size_t i = 0; i < actual_fill; ++i) {
            res_hex_str.append("0");
        }
    }

    res_hex_str.append(raw_hex_str);

    return res_hex_str;

}

} // namespace nv_utils


////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
