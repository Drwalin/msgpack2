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

#ifndef MSGPACK2_READER_INL_CPP
#define MSGPACK2_READER_INL_CPP

#include "msgpack2.hpp"

namespace msgpack2 {
	
	inline Type GetType(const uint8_t* buffer) {
		uint8_t v = *buffer;
		if(v >= _one_type_types)
			return (Type)v;
		else if(v >= _array_short)
			return _array_short;
		else if(v >= _string_short)
			return _string_short;
		else
			return _uint7;
	}

	inline void ReadTypeAndElementsCount(Type& type, uint64_t& size,
			const uint8_t*& buffer, const uint8_t* end) {
		uint8_t v = *buffer;
		if(v >= _one_type_types) {
			type = (Type)v;
			buffer++;
			if(v <= _array_types_end) {
				Read(size, buffer, end);
				switch(type) {
					case _string_long:
						size += _string_short_size;
						break;
					case _array_long:
						size += _array_short_size;
						break;
					default:
						break;
				}
			} else {
				size = 0;
			}
			return;
		} else {
			buffer++;
			if(v >= _array_short) {
				type = _array_short;
			} else if(v >= _string_short) {
				type = _string_short;
			} else {
				type = _uint7;
				if(buffer > end) {
					buffer = NULL;
					return;
				}
				size = 0;
				return;
			}
			size = v - type;
			if(buffer > end)
				buffer = NULL;
			return;
		}
		buffer = NULL;
	}
	
	
	
	inline const uint64_t ReadInteger(const uint8_t*& buffer,
			const uint8_t* end) {
		uint64_t value;
		if(buffer >= end || buffer == NULL) {
			buffer = NULL;
			return 0;
		}
		uint8_t v = *buffer;
		size_t mod;
		switch(v) {
			case _uint8:
				value = buffer[1];
				mod = 2;
				break;
			case _uint16:
				value = endian::ReadBigEndiand<uint16_t>(buffer+1)+_uint16_min;
				mod = 3;
				break;
			case _uint32:
				value = endian::ReadBigEndiand<uint32_t>(buffer+1)+_uint32_min;
				mod = 5;
				break;
			case _uint64:
				value = endian::ReadBigEndiand<uint64_t>(buffer+1);
				mod = 9;
				break;
			case _sint8:
				value = (-*(int8_t*)(buffer+1))+_sin8_min;
				mod = 2;
				break;
			case _sint16:
				value = -endian::ReadBigEndiand<int16_t>(buffer+1);
				mod = 3;
				break;
			case _sint32:
				value = -endian::ReadBigEndiand<int32_t>(buffer+1);
				mod = 5;
				break;
			case _sint64:
				value = -endian::ReadBigEndiand<int64_t>(buffer+1);
				break;
				mod = 9;
			default:
				if(v >= _uint7) {
					value = v - _uint7;
					mod = 1;
				} else {
					buffer = NULL;
					return 0;
				}
		}
		buffer += mod;
		if(buffer > end) {
			buffer = NULL;
			return 0;
		}
		return value;
	}
	
	
	
	inline void Read(uint8_t& value, const uint8_t*& buffer,
			const uint8_t* end) {
		value = ReadInteger(buffer, end);
	}
	
	inline void Read(uint16_t& value, const uint8_t*& buffer,
			const uint8_t* end) {
		value = ReadInteger(buffer, end);
	}
	
	inline void Read(uint32_t& value, const uint8_t*& buffer,
			const uint8_t* end) {
		value = ReadInteger(buffer, end);
	}
	
	inline void Read(uint64_t& value, const uint8_t*& buffer,
			const uint8_t* end) {
		value = ReadInteger(buffer, end);
	}
	
	inline void Read(int8_t& value, const uint8_t*& buffer,
			const uint8_t* end) {
		value = ReadInteger(buffer, end);
	}
	
	inline void Read(int16_t& value, const uint8_t*& buffer,
			const uint8_t* end) {
		value = ReadInteger(buffer, end);
	}
	
	inline void Read(int32_t& value, const uint8_t*& buffer,
			const uint8_t* end) {
		value = ReadInteger(buffer, end);
	}
	
	inline void Read(int64_t& value, const uint8_t*& buffer,
			const uint8_t* end) {
		value = ReadInteger(buffer, end);
	}
	
	

	inline void Read(bool& value, const uint8_t*& buffer,
			const uint8_t* end) {
		if(*buffer == _true)
			value = true;
		else if(*buffer == _false)
			value = false;
		else
			return;
		buffer += 1;
	}

	
	
	inline long double ReadReal(const uint8_t*& buffer, const uint8_t* end) {
		if(buffer >= end)
			return 0;
		if(*buffer == _real) {
			int64_t exponential=0, mantissa=0;
			buffer++;
			Read(mantissa, buffer, end);
			Read(exponential, buffer, end);
			if(buffer > end) {
				buffer = NULL;
				return 0;
			}
			return ldexpl(mantissa, exponential);
		}
		buffer = NULL;
		return 0;
	}
	
	inline void Read(long double& value, const uint8_t*& buffer,
			const uint8_t* end) {
		value = ReadReal(buffer, end);
	}
	
	inline void Read(double& value, const uint8_t*& buffer,
			const uint8_t* end) {
		value = ReadReal(buffer, end);
	}
	
	inline void Read(float& value, const uint8_t*& buffer,
			const uint8_t* end) {
		value = ReadReal(buffer, end);
	}
	
	

	inline std::string_view ReadString(const uint8_t*& buffer,
			const uint8_t* end) {
		Type type;
		uint64_t size=0;
		std::string_view value;
		ReadTypeAndElementsCount(type, size, buffer, end);
		if((type == _string_long || type == _string_short) && buffer+size <= end) {
			value = std::string_view((char*const)buffer,
					(char*const)buffer+size);
			buffer += size;
			return value;
		} else {
			buffer = NULL;
			return std::string_view();
		}
	}

	inline void Read(std::string_view& value, const uint8_t*& buffer,
			const uint8_t* end) {
		value = ReadString(buffer, end);
	}

	inline void Read(const char** str, uint64_t& size, const uint8_t*& buffer,
			const uint8_t* end) {
		std::string_view value = ReadString(buffer, end);
		*str = value.data();
		size = value.size();
	}

	void Read(std::string& value, const uint8_t*& buffer,
			const uint8_t* end) = delete;
	



	
	
	
}

#endif

