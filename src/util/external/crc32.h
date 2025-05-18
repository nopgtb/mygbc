/*
Copyright (c) 2021-2024 Games on Whales https://github.com/games-on-whales/

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/*
Original from: https://github.com/games-on-whales/inputtino/blob/stable/src/uhid/include/crc32.hpp
Edited by namespacing the code.
*/

#ifndef CRC32_H
#define CRC32_H

#include <array>
#include <cstddef>
#include <cstdint>

namespace external{

    static constexpr auto generate_table(std::uint32_t polynomial = 0xEDB88320) {
        std::array<std::uint32_t, 256> table{};
        for (std::uint32_t i = 0; i < 256; i++) {
            std::uint32_t c = i;
            for (std::size_t j = 0; j < 8; j++) {
            if (c & 1) {
                c = polynomial ^ (c >> 1);
            } else {
                c >>= 1;
            }
            }
            table[i] = c;
        }
        return table;
    }

    // Static lookup table generated at compile time
    static constexpr auto lookup_table = generate_table();

    /**
     * Calculate the CRC32 of a buffer
     */
    static constexpr uint32_t CRC32(const unsigned char *buffer, uint32_t length, uint32_t seed = 0) {
        uint32_t c = seed ^ 0xFFFFFFFF;
        for (size_t i = 0; i < length; ++i) {
            c = lookup_table[(c ^ buffer[i]) & 0xFF] ^ (c >> 8);
        }
        return c ^ 0xFFFFFFFF;
    }
}

#endif