#pragma once

// Implementation API was changed a bit, but following
// license is applied as well
/*
   base64.cpp and base64.h

   Copyright (C) 2004-2008 René Nyffenegger

   This source code is provided 'as-is', without any express or implied
   warranty. In no event will the author be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this source code must not be misrepresented; you must not
      claim that you wrote the original source code. If you use this source code
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original source code.

   3. This notice may not be removed or altered from any source distribution.

   René Nyffenegger rene.nyffenegger@adp-gmbh.ch

*/

// C++ STL
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

// ByteArray
#include <bytearray_processor.hpp>

namespace HG::Utils::Base64
{
// Anon
namespace
{
using namespace std::literals;

const auto base64_chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/"sv;

template <typename BYTE>
inline bool is_base64(BYTE c)
{
    return (isalnum(c) || (c == '+') || (c == '/'));
}
} // namespace

/**
 * @brief Function for encoding some sequence of bytes
 * to base64.
 * @tparam Byte Byte type. It can be `int8_t`, `uint8_t`, `std::byte`.
 * @param source Pointer to raw byte array.
 * @param sourceSize Size of raw byte array.
 * @param target String target.
 */
template <typename Byte>
typename std::enable_if<sizeof(Byte) == 1>::type Encode(const Byte* source, std::size_t sourceSize, std::string& target)
{
    std::size_t i = 0;
    std::size_t j = 0;

    std::uint8_t char_array_3[3];
    std::uint8_t char_array_4[4];

    while (sourceSize--)
    {
        char_array_3[i++] = static_cast<std::uint8_t>(*(source++));

        if (i == 3)
        {
            char_array_4[0] = (char_array_3[0] & 0xFC) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xF0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0F) << 2) + ((char_array_3[2] & 0xC0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3F;

            for (i = 0; i < 4; ++i)
            {
                target += base64_chars[char_array_4[i]];
            }

            i = 0;
        }
    }

    if (i)
    {
        for (j = i; j < 3; ++j)
        {
            char_array_3[j] = '\0';
        }

        char_array_4[0] = (char_array_3[0] & 0xFC) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xF0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0F) << 2) + ((char_array_3[2] & 0xC0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3F;

        for (j = 0; j < (i + 1); ++j)
        {
            target += base64_chars[char_array_4[j]];
        }

        while (i++ < 3)
        {
            target += '=';
        }
    }
}

/**
 * @brief Function for encoding some sequence of bytes
 * to base64.
 * @tparam Byte Byte type. It can be `int8_t`, `std::uint8_t`, `std::byte`.
 * @param source Pointer to raw byte array.
 * @param sourceSize Size of raw byte array.
 * @return Encoded byte array to base64 string.
 */
template <typename Byte>
typename std::enable_if<sizeof(Byte) == 1, std::string>::type Encode(const Byte* source, std::size_t sourceSize)
{
    std::string target;

    Encode<Byte>(source, sourceSize, target);

    return target;
}

/**
 * @brief Function for encoding bytearray to base64.
 * @tparam Byte Byte type. It can be `int8_t`, `std::uint8_t`, `std::byte`.
 * @tparam Allocator Bytearray allocator type.
 * @param processor Byte array processor.
 * @param result String target.
 */
template <typename Byte, typename Allocator>
typename std::enable_if<sizeof(Byte) == 1>::type Encode(const bytearray_processor<Byte, Allocator>& processor,
                                                        std::string& result)
{
    Encode<Byte>(processor.container().data(), processor.container().size(), result);
}

/**
 * @brief Function for encoding bytearray to base64.
 * @tparam Byte Byte type. It can be `int8_t`, `std::uint8_t`, `std::byte`.
 * @tparam Allocator Bytearray allocator type.
 * @param processor Byte array processor.
 * @return Base64 encoded string.
 */
template <typename Byte, typename Allocator>
typename std::enable_if<sizeof(Byte) == 1, std::string>::type
Encode(const bytearray_processor<Byte, Allocator>& processor)
{
    return Encode<Byte>(processor.container().data(), processor.container().size());
}

/**
 * @brief Function for decoding base64 encoded string
 * to array of bytes.
 * @tparam InByte Input byte type.
 * @tparam OutByte Output byte type.
 * @param base64 Base64 encoded array of bytes (characters)
 * @param in_size Size of base64 encoded array of bytes (characters)
 * @param target Target vector.
 */
template <typename InByte, typename OutByte>
typename std::enable_if<sizeof(InByte) == 1 && sizeof(OutByte) == 1>::type Decode(const InByte* base64,
                                                                                  std::size_t in_size,
                                                                                  std::vector<OutByte>& target)
{
    std::size_t i              = 0;
    std::size_t j              = 0;
    std::string::size_type in_ = 0;

    std::uint8_t char_array_3[3];
    std::uint8_t char_array_4[4];

    while (in_size-- && (base64[in_] != '=') && is_base64(base64[in_]))
    {
        char_array_4[i++] = base64[in_++];

        if (i == 4)
        {
            for (i = 0; i < 4; ++i)
            {
                char_array_4[i] = base64_chars.find(char_array_4[i]);
            }

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0x0F) << 4) + ((char_array_4[2] & 0x3C) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x03) << 6) + char_array_4[3];

            for (i = 0; i < 3; ++i)
            {
                target.push_back(static_cast<OutByte>(char_array_3[i]));
            }

            i = 0;
        }
    }

    if (i)
    {
        for (j = i; j < 4; ++j)
        {
            char_array_4[j] = 0;
        }

        for (j = 0; j < 4; ++j)
        {
            char_array_4[j] = base64_chars.find(char_array_4[j]);
        }

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0x0F) << 4) + ((char_array_4[2] & 0x3C) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x03) << 6) + char_array_4[3];

        for (j = 0; j < (i - 1); ++j)
        {
            target.push_back(static_cast<OutByte>(char_array_3[j]));
        }
    }
}

/**
 * @brief Function for decoding base64 encoded string
 * to array of bytes.
 * @tparam OutByte Output byte type.
 * @tparam InByte Input byte type.
 * @param base64 Base64 encoded array of bytes (characters)
 * @param in_size Size of base64 encoded array of bytes (characters)
 * @return Decoded array.
 */
template <typename OutByte, typename InByte>
typename std::enable_if<sizeof(InByte) == 1 && sizeof(OutByte) == 1, std::vector<OutByte>>::type
Decode(const InByte* base64, std::size_t in_size)
{
    std::vector<OutByte> target;

    Decode<InByte, OutByte>(base64, in_size, target);

    return target;
}

/**
 * @brief Function for decoding base64 encoded byte array
 * to some target.
 * @tparam Byte Byte type. It can be `int8_t`, `std::uint8_t`, `std::byte`.
 * @param base64 Base64 encoded string.
 * @param target Target.
 */
template <typename Byte>
typename std::enable_if<sizeof(Byte) == 1>::type Decode(const std::string& base64, std::vector<Byte>& target)
{
    Decode<std::string::value_type, Byte>(base64.c_str(), base64.length(), target);
}

/**
 * @brief Function for decoding base64 encoded byte array
 * to some target.
 * @tparam Byte Byte type. It can be `int8_t`, `std::uint8_t`, `std::byte`.
 * @param base64 Base64 encoded string.
 * @return Decoded vector.
 */
template <typename Byte>
typename std::enable_if<sizeof(Byte) == 1, std::vector<Byte>>::type Decode(const std::string& base64)
{
    std::vector<Byte> target;

    Decode<std::string::value_type, Byte>(base64.c_str(), base64.length(), target);

    return target;
}

/**
 * @brief Function for decoding base64 encoded byte array
 * to some target.
 * @tparam Byte Byte type. It can be `int8_t`, `std::uint8_t`, `std::byte`.
 * @param base64 Base64 encoded string.
 * @param target Target.
 */
template <typename Byte>
typename std::enable_if<sizeof(Byte) == 1>::type Decode(const std::string_view& base64, std::vector<Byte>& target)
{
    Decode<std::string_view::value_type, Byte>(base64.data(), base64.length(), target);
}

/**
 * @brief Function for decoding base64 encoded byte array
 * to some target.
 * @tparam Byte Byte type. It can be `int8_t`, `std::uint8_t`, `std::byte`.
 * @param base64 Base64 encoded string.
 * @return Decoded base64 byte array.
 */
template <typename Byte>
typename std::enable_if<sizeof(Byte) == 1, std::vector<Byte>>::type Decode(const std::string_view& base64)
{
    std::vector<Byte> target;

    Decode<std::string_view::value_type, Byte>(base64.data(), base64.length(), target);

    return target;
}
} // namespace HG::Utils::Base64
