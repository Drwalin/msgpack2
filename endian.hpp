/*
 *  This file is part of msgpack2. Please see README for details.
 *  Copyright (C) 2021 Marek Zalewski aka Drwalin
 *
 *  ICon3 is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  ICon3 is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef ENDIAN_HPP
#define ENDIAN_HPP

#include <bit>

#include <cinttypes>

namespace endian {
	template<typename T>
		inline T byteswap(T value) {
			if constexpr(sizeof(T) == 1)
				return value;
			union {
				T v;
				uint8_t b[sizeof(T)];
			} src, dst;
			src.v = value;
			if constexpr(sizeof(T) == 2) {
				dst.b[0] = src.b[1];
				dst.b[1] = src.b[0];
				return dst.v;
			} else if constexpr(sizeof(T) == 4) {
				dst.b[0] = src.b[3];
				dst.b[1] = src.b[2];
				dst.b[2] = src.b[1];
				dst.b[3] = src.b[0];
				return dst.v;
			} else if constexpr(sizeof(T) == 8) {
				dst.b[0] = src.b[7];
				dst.b[1] = src.b[6];
				dst.b[2] = src.b[5];
				dst.b[3] = src.b[4];
				dst.b[4] = src.b[3];
				dst.b[5] = src.b[2];
				dst.b[6] = src.b[1];
				dst.b[7] = src.b[0];
				return dst.v;
			}
		}

	template<typename T>
		inline T ReadBigEndiand(const uint8_t* buffer) {
			if constexpr (std::endian::native == std::endian::big)
				return *(const T*)buffer;
			else
				return byteswap(*(const T*)buffer);
		}

	template<typename T>
		inline void WriteBigEndiand(T value, uint8_t* buffer) {
			if constexpr (std::endian::native == std::endian::big)
				*(T*)buffer = value;
			else
				*(T*)buffer = byteswap(value);
		}
}

#endif

