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

#ifndef MSGPACK2_MSGPACK2_HPP
#define MSGPACK2_MSGPACK2_HPP

#include <cstring>
#include <cinttypes>
#include <cmath>
#include <limits>
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

		_one_type_types = 0xE9,

		_array_types_begin = 0xE9,

		_map_long_8     = 0xE9,
		_map_long_16    = 0xEA,
		_map_long_32    = 0xEB,
		_map_long_64    = 0xEC,
		_string_long_8  = 0xED,
		_string_long_16 = 0xEE,
		_string_long_32 = 0xEF,
		_string_long_64 = 0xF0,
		_array_long_8   = 0xF1,
		_array_long_16  = 0xF2,
		_array_long_32  = 0xF3,
		_array_long_64  = 0xF4,

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
	
	struct HeaderValueSimplified {
		enum _Type {
			INVALID,
			ARRAY,
			INT,
			UINT,
			REAL,
			BOOL,
			STRING,
			MAP
		};
		union {
			double __double;
			bool __bool;
			
			uint64_t __uint64;
			uint32_t __uint32;
			uint16_t __uint16;
			uint8_t  __uint8;
			
			int64_t  __int64;
			int32_t  __int32;
			int16_t  __int16;
			int8_t   __int8;
			
			uint64_t __elements;
		};
		uint64_t size_of;
		_Type type;
	};

	inline const static uint64_t _string_short_size = 0x40;
	inline const static uint64_t _array_short_size  = 0x20;

	inline const static uint64_t _uint7_min = 0x00;
	inline const static uint64_t _uint7_max = 0x7F;
	
	inline const static uint64_t _uint8_min  = std::numeric_limits<uint8_t>::min();
	inline const static uint64_t _uint8_max  = std::numeric_limits<uint8_t>::max();
	inline const static uint64_t _uint16_min = std::numeric_limits<uint16_t>::min();
	inline const static uint64_t _uint16_max = std::numeric_limits<uint16_t>::max();
	inline const static uint64_t _uint32_min = std::numeric_limits<uint32_t>::min();
	inline const static uint64_t _uint32_max = std::numeric_limits<uint32_t>::max();
	inline const static uint64_t _uint64_min = std::numeric_limits<uint64_t>::min();
	inline const static uint64_t _uint64_max = std::numeric_limits<uint64_t>::max();
	
	inline const static int64_t _sint8_max  = std::numeric_limits<int8_t>::min();
	inline const static int64_t _sint8_min  = std::numeric_limits<int8_t>::max();
	inline const static int64_t _sint16_min = std::numeric_limits<int16_t>::min();
	inline const static int64_t _sint16_max = std::numeric_limits<int16_t>::max();
	inline const static int64_t _sint32_min = std::numeric_limits<int32_t>::min();
	inline const static int64_t _sint32_max = std::numeric_limits<int32_t>::max();
	inline const static int64_t _sint64_min = std::numeric_limits<int64_t>::min();
	inline const static int64_t _sint64_max = std::numeric_limits<int64_t>::max();
	
	
	
	
	
	template<typename WriterT>
		inline void WriteBytes(WriterT& writer, const void* data,
				uint64_t bytes);
	template<typename WriterT, typename T>
		inline void WritePush(WriterT& writer, uint8_t byte);
	template<typename WriterT, typename T>
		inline void WriteFixedBody(WriterT& writer, const T value);
	
	template<typename WriterT>
		inline void WriteHeaderArray(WriterT& writer, size_t elements);
	template<typename WriterT>
		inline void WriteHeaderMap(WriterT& writer, size_t elements);
	template<typename WriterT>
		inline void WriteHeaderString(WriterT& writer, size_t elements);

	template<typename WriterT, typename T>
		inline void Write(WriterT& writer, const T& value);
	
	
	
	inline HeaderValueSimplified ReadHeader(const uint8_t*& data,
			const uint8_t* end);
	
	template<typename T>
	inline bool Read(T& value, const uint8_t*& data, const uint8_t* end);
	
	
	
	
	
	template<typename WriterT>
	inline void WriteBytes(WriterT& writer, const void* data, uint64_t bytes) {
		memcpy(writer.make_space(bytes), data, bytes);
	}

	template<typename WriterT, typename T>
	inline void WritePush(WriterT& writer, uint8_t byte) {
		*writer.make_space(1) = byte;
	}
	
	template<typename WriterT, typename T>
	inline void WriteFixedBody(WriterT& writer, const T value) {
		write_big_endian(value, writer.make_space(sizeof(T)));
	}
	
	template<typename WriterT>
	inline void WriteHeaderArray(WriterT& writer, size_t elements) {
		if(elements < _array_short_size) {
			writer.push(_array_short+elements);
		} else if(((uint8_t)elements) == elements) {
			writer.push(_array_long_8);
			WriteFixedBody(writer, (uint8_t)elements);
		} else if(((uint16_t)elements) == elements) {
			writer.push(_array_long_16);
			WriteFixedBody(writer, (uint16_t)elements);
		} else if(((uint32_t)elements) == elements) {
			writer.push(_array_long_32);
			WriteFixedBody(writer, (uint32_t)elements);
		} else {
			writer.push(_array_long_64);
			WriteFixedBody(writer, (uint64_t)elements);
		}
	}
	
	template<typename WriterT>
	inline void WriteHeaderMap(WriterT& writer, size_t elements) {
		if(((uint8_t)elements) == elements) {
			writer.push(_map_long_8);
			writer.push((uint8_t)elements);
		} else if(((uint16_t)elements) == elements) {
			writer.push(_map_long_16);
			write_big_endian((uint16_t)elements, writer.make_space(2));
		} else if(((uint32_t)elements) == elements) {
			writer.push(_map_long_32);
			write_big_endian((uint32_t)elements, writer.make_space(4));
		} else {
			writer.push(_map_long_64);
			write_big_endian((uint64_t)elements, writer.make_space(8));
		}
	}
	
	template<typename WriterT>
	inline void WriteHeaderString(WriterT& writer, size_t elements) {
		if(elements < _string_short_size) {
			writer.push(_string_short+elements);
		} else if(((uint8_t)elements) == elements) {
			writer.push(_string_long_8);
			writer.push((uint8_t)elements);
		} else if(((uint16_t)elements) == elements) {
			writer.push(_string_long_16);
			write_big_endian((uint16_t)elements, writer.make_space(2));
		} else if(((uint32_t)elements) == elements) {
			writer.push(_string_long_32);
			write_big_endian((uint32_t)elements, writer.make_space(4));
		} else {
			writer.push(_string_long_64);
			write_big_endian((uint64_t)elements, writer.make_space(8));
		}
	}



	template<typename WriterT, typename T,
		template<typename> typename array>
	inline void Write(WriterT& writer, const array<T>& value) {
		WriteHeaderArray(writer, value.size());
		for(auto e : value)
			Write(writer, e);
	}
	
	template<typename WriterT, typename K, typename V,
		template<typename, typename> typename map>
	inline void PackAdd(WriterT& writer, const map<K, V>& value) {
		WriteMapArray(writer, value.size());
		for(auto e : value) {
			Write(writer, e.first);
			Write(writer, e.second);
		}
	}
	
	template<typename WriterT, template<typename> typename array>
	inline void Write(WriterT& writer, const array<uint8_t>& value) {
		WriteHeaderString(writer, value.size());
		WriteBytes(writer, value.data(), value.size());
	}
	
	template<typename WriterT, template<typename> typename array>
	inline void Write(WriterT& writer, const array<int8_t>& value) {
		WriteHeaderString(writer, value.size());
		WriteBytes(writer, value.data(), value.size());
	}
	
	template<typename WriterT>
	inline void Write(WriterT& writer, const std::string& value) {
		WriteHeaderString(writer, value.size());
		WriteBytes(writer, value.data(), value.size());
	}
	
	template<typename WriterT>
	inline void Write(WriterT& writer, const std::string_view& value) {
		WriteHeaderString(writer, value.size());
		WriteBytes(writer, value.data(), value.size());
	}
	
	template<typename T>
	inline void Write(std::vector<uint8_t>& writer, const T& value) {
		struct VectorWriter {
			std::vector<uint8_t>& vector;
			inline uint8_t* make_space(size_t bytes) {
				size_t s = vector.size();
				vector.resize(s + bytes);
				return vector.data()+s;
			}
		};
		Write(VectorWriter{writer}, value);
	}
	
	
	
	template<typename WriterT>
	inline void Write(WriterT& writer, uint64_t value) {
		if(value < _uint7_max) {
			writer.push(_uint7+value);
		} else if(((uint8_t)value) == value) {
			writer.push(_uint8);
			WriteFixedBody(writer, (uint8_t)value);
		} else if(((uint16_t)value) == value) {
			writer.push(_uint16);
			WriteFixedBody(writer, (uint16_t)value);
		} else if(((uint32_t)value) == value) {
			writer.push(_uint32);
			WriteFixedBody(writer, (uint32_t)value);
		} else {
			writer.push(_uint64);
			WriteFixedBody(writer, (uint64_t)value);
		}
	}
	
	template<typename WriterT>
	inline void Write(WriterT& writer, int64_t value) {
		if(((int8_t)value) == value) {
			writer.push(_sint8);
			WriteFixedBody(writer, (uint8_t)value);
		} else if(((int16_t)value) == value) {
			writer.push(_sint16);
			WriteFixedBody(writer, (uint16_t)value);
		} else if(((int32_t)value) == value) {
			writer.push(_sint32);
			WriteFixedBody(writer, (uint32_t)value);
		} else {
			writer.push(_sint64);
			WriteFixedBody(writer, (uint64_t)value);
		}
	}
	
	template<typename WriterT>
	inline void Write(WriterT& writer, bool value) {
		WritePush(writer, value ? _true : _false);
	}
	
	
	
	template<typename WriterT>
	inline void WriteReal(WriterT& writer, long double value) {
		WritePush(writer, _real);
		int exp = 0;
		double mantissaDouble = frexp(value, &exp);
		mantissaDouble = ldexp(mantissaDouble, 62);
		exp += 62;
		int64_t mantissa = lround(mantissaDouble);
		if(mantissa != 0) {
			while((mantissa&1) == 0) {
				mantissa >>= 1;
				exp -= 1;
			}
		}
		Write(writer, exp);
		Write(writer, mantissa);
	}
	
	template<typename WriterT>
	inline void Write(WriterT& writer, float value) {
		WriteReal(writer, value);
	}
	
	template<typename WriterT>
	inline void Write(WriterT& writer, double value) {
		WriteReal(writer, value);
	}
	
	template<typename WriterT>
	inline void Write(WriterT& writer, long double value) {
		WriteReal(writer, value);
	}
	
	
	
	
	
	template<typename T>
	inline bool Read(T& value, const uint8_t*& data, const uint8_t* end);
	
	inline HeaderValueSimplified ReadHeader(const uint8_t*& data,
			const uint8_t* end) {
		if(data != end) {
			uint8_t head = *data;
			++data;
			if((head&_uint7_max) == head) {		// UIN7
				return {
					.__uint8 = head,
					.size_of = 1,
					.type = HeaderValueSimplified::UINT,
				};
			} else if(head < _string_short + _string_short_size) {		// SHORT STRING
				return {
					.__elements = (uint64_t)head - _string_short,
					.type = HeaderValueSimplified::STRING,
				};
			} else if(head < _array_types_begin) {		// SHORT ARRAY
				return {
					.__elements = (uint64_t)head - _array_short,
					.type = HeaderValueSimplified::ARRAY,
				};
			} else if(head <= _array_types_end) {		// LONG MAP / STRING / ARRAY
				HeaderValueSimplified h;
				const static HeaderValueSimplified::_Type arr_types[3] = {
					HeaderValueSimplified::MAP,
					HeaderValueSimplified::STRING,
					HeaderValueSimplified::ARRAY
				};
				h.type = arr_types[(head - _array_types_begin)>>2];
				
				uint64_t s = (head - _array_types_begin) & 0x3;
				h.size_of = 1<<s;
				
				if(data+h.size_of > end)
					return {.type = HeaderValueSimplified::INVALID};
				
				h.__elements = read_big_endian_bytes_size(h.size_of, data);
				data += h.size_of;
				return h;
			} else {		// PRIMITIVE TYPES
				switch(head) {
					case _true:
						return {
							.__bool = true,
							.type = HeaderValueSimplified::BOOL
						};
					case _false:
						return {
							.__bool = false,
							.type = HeaderValueSimplified::BOOL
						};
						
					case _real:
						break;
						
					case _uint8:
					case _uint16:
					case _uint32:
					case _uint64:
					case _sint8:
					case _sint16:
					case _sint32:
					case _sint64: {
						HeaderValueSimplified h;
						if(head >= _sint8)
							h.type = HeaderValueSimplified::INT;
						else
							h.type = HeaderValueSimplified::UINT;
						

						uint64_t s = (head - _uint8) & 0x3;
						h.size_of = 1<<s;

						if(data+h.size_of > end)
							return {.type = HeaderValueSimplified::INVALID};

						switch(s) {
							case 0:
								h.__uint8 = read_big_endian<uint8_t>(data);
								break;
							case 1:
								h.__uint16 = read_big_endian<uint16_t>(data);
								break;
							case 2:
								h.__uint32 = read_big_endian<uint32_t>(data);
								break;
							case 3:
								h.__uint64 = read_big_endian<uint64_t>(data);
								break;
						}
						data += h.size_of;
						return h;
					}
				}
			}
		}
		return {.type = HeaderValueSimplified::INVALID};
	}

	
#define READ_PRIMITIVE(BITS) \
	inline bool Read(uint##BITS##_t& value, const uint8_t*& data, \
			const uint8_t* end) { \
		HeaderValueSimplified head = ReadHeader(data, end); \
		if(head.type == HeaderValueSimplified::UINT) { \
			if(head.size_of <= sizeof(uint##BITS##_t)) { \
				value = head.__uint##BITS; \
				return true; \
			} \
		} \
		return false; \
	} \
	inline bool Read(int##BITS##_t& value, const uint8_t*& data, \
			const uint8_t* end) { \
		HeaderValueSimplified head = ReadHeader(data, end); \
		if(head.type == HeaderValueSimplified::INT) { \
			if(head.size_of <= sizeof(uint##BITS##_t)) { \
				value = head.__int##BITS; \
				return true; \
			} \
		} \
		return false; \
	}
	READ_PRIMITIVE(8);
	READ_PRIMITIVE(16);
	READ_PRIMITIVE(32);
	READ_PRIMITIVE(64);
#undef READ_PRIMITIVE
	
	inline bool Read(std::string_view& value, const uint8_t*& data,
			const uint8_t* end) {
		HeaderValueSimplified head = ReadHeader(data, end);
		if(head.type == HeaderValueSimplified::STRING) {
			if(data+head.__elements > end)
				return false;
			value = std::string_view((const char*)data, head.__elements);
			data += head.__elements;
			return true;
		}
		return false;
	}
	
	inline bool Read(std::string& value, const uint8_t*& data,
			const uint8_t* end) {
		std::string_view str_view;
		if(Read(str_view, data, end)) {
			value = str_view;
			return true;
		}
		return false;
	}
	
	inline bool Read(std::vector<uint8_t>& value, const uint8_t*& data,
			const uint8_t* end) {
		std::string_view str_view;
		if(Read(str_view, data, end)) {
			value.clear();
			value.insert(value.begin(), str_view.begin(), str_view.end());
			return true;
		}
		return false;
	}
	
	inline bool Read(std::vector<int8_t>& value, const uint8_t*& data,
			const uint8_t* end) {
		std::string_view str_view;
		if(Read(str_view, data, end)) {
			value.clear();
			value.insert(value.begin(), str_view.begin(), str_view.end());
			return true;
		}
		return false;
	}
	
	inline bool Read(const uint8_t*& buffer, uint64_t& elements,
			const uint8_t*& data, const uint8_t* end) {
		std::string_view str_view;
		if(Read(str_view, data, end)) {
			buffer = (const uint8_t*)str_view.data();
			elements = str_view.size();
			return true;
		}
		return false;
	}
	
	inline bool Read(const int8_t*& buffer, uint64_t& elements,
			const uint8_t*& data, const uint8_t* end) {
		std::string_view str_view;
		if(Read(str_view, data, end)) {
			buffer = (const int8_t*)str_view.data();
			elements = str_view.size();
			return true;
		}
		return false;
	}
	
	template<typename T, template<typename> typename array>
	inline bool Read(array<T>& value, const uint8_t*& data,
			const uint8_t* end) {
		HeaderValueSimplified head = ReadHeader(data, end);
		if(head.type == HeaderValueSimplified::ARRAY) {
			value.clear();
			value.resize(head.__elements);
			for(T& elem : value) {
				if(Read(elem, data, end) == false)
					return false;
			}
			return true;
		}
		return false;
	}
	
	template<typename K, typename V,
		template<typename, typename> typename map>
	inline bool Read(map<K, V>& value, const uint8_t*& data,
			const uint8_t* end) {
		HeaderValueSimplified head = ReadHeader(data, end);
		if(head.type == HeaderValueSimplified::ARRAY) {
			value.clear();
			for(uint64_t i=0; i<head.__elements; ++i) {
				K key;
				if(Read(key, data, end) == false)
					return false;
				if(Read(value[key], data, end) == false)
					return false;
			}
			return true;
		}
		return false;
	}
	
	inline bool Read(long double& value, const uint8_t*& data,
			const uint8_t* end) {
		HeaderValueSimplified head = ReadHeader(data, end);
		if(head.type == HeaderValueSimplified::REAL) {
			int64_t mantissa, exponential;
			if(Read(mantissa, data, end) == false)
				return false;
			if(Read(exponential, data, end) == false)
				return false;
			value = ldexpl(mantissa, exponential);
			return true;
		}
		return false;
	}
	
	inline bool Read(double& value, const uint8_t*& data,
			const uint8_t* end) {
		long double v;
		if(Read(v, data, end) == false)
			return false;
		value = v;
		return true;
	}
	
	inline bool Read(float& value, const uint8_t*& data,
			const uint8_t* end) {
		long double v;
		if(Read(v, data, end) == false)
			return false;
		value = v;
		return true;
	}
}

#endif

