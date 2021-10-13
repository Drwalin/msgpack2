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

#ifndef MSGPACK2_WRITER_INL_CPP
#define MSGPACK2_WRITER_INL_CPP

#include "msgpack2.hpp"

#include <cstring>

namespace msgpack2 {

	inline void WriteInteger(int64_t value,
			std::vector<uint8_t>& buffer) {
		if(value < 0) {
			value = -value;
			if(value < 0x100) {
				buffer.emplace_back(_sint8);
				buffer.emplace_back(value);
			} else if(value < 0x10000) {
				buffer.emplace_back(_sint16);
				buffer.resize(buffer.size()+2);
				endian::WriteBigEndiand<uint16_t>(value,
						buffer.data()+buffer.size()-2);
			} else if(value < 0x10000000) {
				buffer.emplace_back(_sint32);
				buffer.resize(buffer.size()+4);
				endian::WriteBigEndiand<uint64_t>(value,
						buffer.data()+buffer.size()-4);
			} else {
				buffer.emplace_back(_sint64);
				buffer.resize(buffer.size()+8);
				endian::WriteBigEndiand<uint64_t>(value,
						buffer.data()+buffer.size()-8);
			}
		} else {
			if(value <= _uint7_max) {
				buffer.emplace_back(value + _uint7);
			} else {
				value -= _uint7_max+1;
				if(value < 0x10000) {
					buffer.emplace_back(_uint16);
					buffer.resize(buffer.size()+2);
					endian::WriteBigEndiand<uint16_t>(value,
							buffer.data()+buffer.size()-2);
				} else if(value < 0x10000000) {
					buffer.emplace_back(_uint32);
					buffer.resize(buffer.size()+4);
					endian::WriteBigEndiand<uint64_t>(value,
							buffer.data()+buffer.size()-4);
				} else {
					buffer.emplace_back(_uint64);
					buffer.resize(buffer.size()+8);
					endian::WriteBigEndiand<uint64_t>(value,
							buffer.data()+buffer.size()-8);
				}
			}
		}
	}

	inline void Write(uint8_t value, std::vector<uint8_t>& buffer) {
		WriteInteger(value, buffer);
	}
	
	inline void Write(uint16_t value, std::vector<uint8_t>& buffer) {
		WriteInteger(value, buffer);
	}
	
	inline void Write(uint32_t value, std::vector<uint8_t>& buffer) {
		WriteInteger(value, buffer);
	}
	
	inline void Write(uint64_t value, std::vector<uint8_t>& buffer) {
		WriteInteger(value, buffer);
	}
	
	inline void Write(int8_t value, std::vector<uint8_t>& buffer) {
		WriteInteger(value, buffer);
	}
	
	inline void Write(int16_t value, std::vector<uint8_t>& buffer) {
		WriteInteger(value, buffer);
	}
	
	inline void Write(int32_t value, std::vector<uint8_t>& buffer) {
		WriteInteger(value, buffer);
	}
	
	inline void Write(int64_t value, std::vector<uint8_t>& buffer) {
		WriteInteger(value, buffer);
	}

	
	
	inline void Write(bool value, std::vector<uint8_t>& buffer) {
		buffer.emplace_back(value ? _true : _false);
	}

	
	
	inline void WriteString(std::string_view value,
			std::vector<uint8_t>& buffer) {
		if(value.size() < _string_short_size)
			buffer.emplace_back(_string_short + value.size());
		else
			Write(value.size()-_string_short_size, buffer);
		buffer.insert(buffer.end(), value.begin(), value.end());
	}

	inline void Write(std::string_view value, std::vector<uint8_t>& buffer) {
		WriteString(std::string_view(value), buffer);
	}

	inline void Write(const char* str, std::vector<uint8_t>& buffer) {
		WriteString(std::string_view(str, str+strlen(str)), buffer);
	}

	
	
	// TODO optimise space taken by mantissa (least significant zero-value bits)
	inline void WriteRealComponents(long double value,
			std::vector<uint8_t>& buffer) {
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
		Write(exp, buffer);
		Write(mantissa, buffer);
	}
	
	inline void WriteReal(long double value, std::vector<uint8_t>& buffer) {
		buffer.emplace_back(_real);
		WriteRealComponents(value, buffer);
	}
	
	inline void Write(long double value, std::vector<uint8_t>& buffer) {
		WriteReal(value, buffer);
	}
	
	inline void Write(double value, std::vector<uint8_t>& buffer) {
		WriteReal(value, buffer);
	}
	
	inline void Write(float value, std::vector<uint8_t>& buffer) {
		WriteReal(value, buffer);
	}
	
	
	
	template<typename T>
		inline void WriteIntegerSpan(const std::span<T>& value,
				std::vector<uint8_t>& buffer) {
			Type type;
			switch(sizeof(T)) {
				case 1:
					WriteString(std::string_view((const char*)value.data(),
								((const char*)value.data())+value.size()),
							buffer);
					return;
				case 2:
					type = _array_uint16;
					break;
				case 4:
					type = _array_uint32;
					break;
				case 8:
					type = _array_uint64;
					break;
				default:
					return;
			}
			buffer.emplace_back(type);
			Write(value.size(), buffer);
			uint8_t* buf = &(buffer[0])+buffer.size();
			buffer.resize(buffer.size() + value.size()*sizeof(T));
			for(uint64_t i=0; i<value.size(); ++i, buf+=sizeof(T))
				endian::WriteBigEndiand(value[i], buf);
		}
	
	inline void Write(const std::span<uint8_t>& value,
			std::vector<uint8_t>& buffer) {
		WriteIntegerSpan(value, buffer);
	}
	
	inline void Write(const std::span<uint16_t>& value,
			std::vector<uint8_t>& buffer) {
		WriteIntegerSpan(value, buffer);
	}
	
	inline void Write(const std::span<uint32_t>& value,
			std::vector<uint8_t>& buffer) {
		WriteIntegerSpan(value, buffer);
	}
	
	inline void Write(const std::span<uint64_t>& value,
			std::vector<uint8_t>& buffer) {
		WriteIntegerSpan(value, buffer);
	}
	
	inline void Write(const std::span<int8_t>& value,
			std::vector<uint8_t>& buffer) {
		WriteIntegerSpan(value, buffer);
	}

	inline void Write(const std::span<int16_t>& value,
			std::vector<uint8_t>& buffer) {
		WriteIntegerSpan(value, buffer);
	}

	inline void Write(const std::span<int32_t>& value,
			std::vector<uint8_t>& buffer) {
		WriteIntegerSpan(value, buffer);
	}

	inline void Write(const std::span<int64_t>& value,
			std::vector<uint8_t>& buffer) {
		WriteIntegerSpan(value, buffer);
	}
	
	
	
	inline void InitMap(uint64_t elements, std::vector<uint8_t>& buffer) {
		if(elements < _map_short_size) {
			buffer.emplace_back(_map_short + elements);
		} else {
			buffer.emplace_back(_map_long);
			Write(elements, buffer);
		}
	}

	inline void InitArray(uint64_t elements, std::vector<uint8_t>& buffer) {
		if(elements < _array_short_size) {
			buffer.emplace_back(_array_short + elements);
		} else {
			buffer.emplace_back(_array_long);
			Write(elements, buffer);
		}
	}
	
	
	
	template<typename T>
		inline void WriteRealSpan(const std::span<T>& value,
				std::vector<uint8_t>& buffer) {
			InitArray(value.size(), buffer);
			for(uint64_t i=0; i<value.size(); ++i)
				WriteReal(value[i], buffer);
		}
	
	inline void Write(const std::span<long double>& value, std::vector<uint8_t>& buffer) {
		WriteRealSpan(value, buffer);
	}
	
	inline void Write(const std::span<double>& value, std::vector<uint8_t>& buffer) {
		WriteRealSpan(value, buffer);
	}
	
	inline void Write(const std::span<float>& value, std::vector<uint8_t>& buffer) {
		WriteRealSpan(value, buffer);
	}
}

#endif

