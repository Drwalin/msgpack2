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

#ifndef MSGPACK2_HPP
#define MSGPACK2_HPP

#include <cinttypes>
#include <cmath>

#include <vector>
#include <bit>
#include <string>
#include <span>

#include "endian.hpp"

namespace msgpack2 {
	// uint8[] == int8[] = char[] = string
	enum Type {
		_uint7        = 0x00,  // 0x00:0x7F == 0x00:0x7F : [0x80]
		_string_short = 0x80,  // 0x80:0xBF -> 0x00:0x3F : [0x40]
		_array_short  = 0xC0,  // 0xC0:0xDF -> 0x00:0x1F : [0x20]

		_one_type_types = 0xEF,

		_array_types_begin = 0xEF,

		_array_long    = 0xEF,  // [ int size, any[size+0x20]   ]
		
		_string_long   = 0xF0,  // [ int size, uint16[size]     ]
		_array_uint16  = 0xF1,  // [ int size, uint16[size]     ]
		_array_uint32  = 0xF2,  // [ int size, uint32[size]     ]
		_array_uint64  = 0xF3,  // [ int size, uint64[size]     ]
		_array_real    = 0xF4,  // [ int size, (int, int)[size] ]

		_array_types_end = 0xF4,

		_true    = 0xF5,
		_false   = 0xF6,
		_real    = 0xF7,  // [ _real, int exponent, int mantissa ]
		_uint8   = 0xF8,
		_uint16  = 0xF9,
		_uint32  = 0xFA,
		_uint64  = 0xFB,
		_sint8   = 0xFC,
		_sint16  = 0xFD,
		_sint32  = 0xFE,
		_sint64  = 0xFF,
	};

	inline const static uint64_t _string_short_size = 0x40,
	inline const static uint64_t _array_short_size  = 0x20,

	inline const static uint64_t _uint7_min = 0x00,
	inline const static uint64_t _uint7_max = 0x7F,
	
	inline const static uint64_t _uint8_min  = 0x00;
	inline const static uint64_t _uint8_max  = 0xFF;
	inline const static uint64_t _uint16_min = _uint8_max+1;
	inline const static uint64_t _uint16_max = _uint8_max+(1<<16);
	inline const static uint64_t _uint32_min = _uint16_max+1;
	inline const static uint64_t _uint32_max = (uint64_t)_uint16_max+(uint64_t)(1ll<<32);
	inline const static uint64_t _uint64_min = 0;
	inline const static uint64_t _uint64_max = ~(uint64_t)0;
	
	inline const static int64_t _sint8_max  = -0x01;
	inline const static int64_t _sint8_min  = -0x100;
	inline const static int64_t _sint16_min = _sint8_min-(1ll<<16);
	inline const static int64_t _sint16_max = _sint8_min-1;
	inline const static int64_t _sint32_min = _sint16_min-(1ll<<32);
	inline const static int64_t _sint32_max = _sint16_min-1;
	inline const static int64_t _sint64_min = (1ll<<63);
	inline const static int64_t _sint64_max = (1ll<<63)-1;
	
	
	


	inline Type GetType(const uint8_t* buffer);
	inline void ReadTypeAndElementsCount(Type& type, uint64_t& size,
			const uint8_t*& buffer, const uint8_t* end);
	
	inline const uint64_t ReadInteger(const uint8_t*& buffer, const uint8_t* end);
	inline void Read(uint8_t& value, const uint8_t*& buffer, const uint8_t* end);
	inline void Read(uint16_t& value, const uint8_t*& buffer, const uint8_t* end);
	inline void Read(uint32_t& value, const uint8_t*& buffer, const uint8_t* end);
	inline void Read(uint64_t& value, const uint8_t*& buffer, const uint8_t* end);
	inline void Read(int8_t& value, const uint8_t*& buffer, const uint8_t* end);
	inline void Read(int16_t& value, const uint8_t*& buffer, const uint8_t* end);
	inline void Read(int32_t& value, const uint8_t*& buffer, const uint8_t* end);
	inline void Read(int64_t& value, const uint8_t*& buffer, const uint8_t* end);
	
	inline void Write(bool value, std::vector<uint8_t>& buffer);
	
	inline std::string_view ReadString(const uint8_t*& buffer, const uint8_t* end);
	inline void Read(std::string_view& value, const uint8_t*& buffer, const uint8_t* end);
	inline void Read(const char** value, uint64_t& size, const uint8_t*& buffer, const uint8_t* end);
	
	inline long double ReadReal(const uint8_t*& buffer, const uint8_t* end);
	inline void Read(long double& value, const uint8_t*& buffer, const uint8_t* end);
	inline void Read(double& value, const uint8_t*& buffer, const uint8_t* end);
	inline void Read(float& value, const uint8_t*& buffer, const uint8_t* end);
	
	inline bool ReadArrayHeader(uint64_t& size, const uint8_t*& buffer, const uint8_t* end);
	inline bool ReadMapHeader(uint64_t& size, const uint8_t*& buffer, const uint8_t* end);
	// TODO make Readers for arrays and maps
	// TODO make Readers for typed arrays
	
	


	inline void WriteInteger(int64_t value, std::vector<uint8_t>& buffer);
	inline void Write(uint8_t value, std::vector<uint8_t>& buffer);
	inline void Write(uint16_t value, std::vector<uint8_t>& buffer);
	inline void Write(uint32_t value, std::vector<uint8_t>& buffer);
	inline void Write(uint64_t value, std::vector<uint8_t>& buffer);
	inline void Write(int8_t value, std::vector<uint8_t>& buffer);
	inline void Write(int16_t value, std::vector<uint8_t>& buffer);
	inline void Write(int32_t value, std::vector<uint8_t>& buffer);
	inline void Write(int64_t value, std::vector<uint8_t>& buffer);
	
	inline void Write(bool value, std::vector<uint8_t>& buffer);

	inline void WriteString(std::string_view value, std::vector<uint8_t>& buffer);
	inline void Write(std::string_view value, std::vector<uint8_t>& buffer);
	
	inline void WriteReal(long double value, std::vector<uint8_t>& buffer);
	inline void Write(long double value, std::vector<uint8_t>& buffer);
	inline void Write(double value, std::vector<uint8_t>& buffer);
	inline void Write(float value, std::vector<uint8_t>& buffer);
	
	template<typename T>
		inline void WriteIntegerSpan(const std::span<T>& value,
				std::vector<uint8_t>& buffer);
	inline void Write(const std::span<uint8_t>& value, std::vector<uint8_t>& buffer);
	inline void Write(const std::span<uint16_t>& value, std::vector<uint8_t>& buffer);
	inline void Write(const std::span<uint32_t>& value, std::vector<uint8_t>& buffer);
	inline void Write(const std::span<uint64_t>& value, std::vector<uint8_t>& buffer);
	inline void Write(const std::span<int8_t>& value, std::vector<uint8_t>& buffer);
	inline void Write(const std::span<int16_t>& value, std::vector<uint8_t>& buffer);
	inline void Write(const std::span<int32_t>& value, std::vector<uint8_t>& buffer);
	inline void Write(const std::span<int64_t>& value, std::vector<uint8_t>& buffer);
	
	
	
	inline void InitArray(uint64_t elements, std::vector<uint8_t>& buffer);
	inline void InitMap(uint64_t elements, std::vector<uint8_t>& buffer);
	
	template<typename T>
		inline void WriteRealSpan(const std::span<T>& value, std::vector<uint8_t>& buffer);
	inline void Write(const std::span<long double>& value, std::vector<uint8_t>& buffer);
	inline void Write(const std::span<double>& value, std::vector<uint8_t>& buffer);
	inline void Write(const std::span<float>& value, std::vector<uint8_t>& buffer);
}

#ifndef MSGPACK2_READER_INL_CPP
#include"msgpack2_reader.inl.cpp"
#endif

#ifndef MSGPACK2_WRITER_INL_CPP
#include"msgpack2_writer.inl.cpp"
#endif

#endif

