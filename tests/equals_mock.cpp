
#include "../msgpack2.hpp"

#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <tuple>
#include <utility>

template<typename T>
bool equals(const T& l, const T& r);

#define EQUALS_INTEGER(TYPE) \
template<> \
bool equals<TYPE>(const TYPE& l, const TYPE& r) { return l==r; }

EQUALS_INTEGER(uint8_t);
EQUALS_INTEGER(uint16_t);
EQUALS_INTEGER(uint32_t);
EQUALS_INTEGER(uint64_t);
EQUALS_INTEGER(int8_t);
EQUALS_INTEGER(int16_t);
EQUALS_INTEGER(int32_t);
EQUALS_INTEGER(int64_t);

#undef EQUALS_INTEGER

#define EQUALS_REAL(TYPE) \
template<> \
bool equals<TYPE>(const TYPE& l, const TYPE& r) { \
	TYPE d = l-r; \
	if(d < 0) d = -d; \
	return d <= 0.0000001; \
}

EQUALS_REAL(float);
EQUALS_REAL(double);
EQUALS_REAL(long double);

#undef EQUALS_REAL

template<>
bool equals<bool>(const bool& l, const bool& r) {
	return l==r;
}

template<>
bool equals<std::string>(const std::string& l, const std::string& r) {
	return l==r;
}

template<typename T>
bool equals(const std::vector<T>& l, const std::vector<T>& r) {
	if(l.size() != r.size())
		return false;
	for(size_t i=0; i<l.size(); ++i)
		if(equals(l[i], r[i]) == false)
			return false;
	return true;
}

template<typename K, typename V>
bool equals(const std::map<K, V>& l, const std::map<K, V>& r) {
	if(l.size() != r.size())
		return false;
	for(const auto& e : l) {
		auto it = r.find(e.first);
		if(it == r.end())
			return false;
		if(equals(it->second, e.second) == false)
			return false;
	}
	return true;
}



template<std::size_t I = 0, typename... Args>
inline typename std::enable_if<I == sizeof...(Args), bool>::type
for_equals_data(std::tuple<Args...>& t) {
	return true;
}

template<std::size_t I = 0, typename... Args>
inline typename std::enable_if<I < sizeof...(Args), bool>::type
for_equals_data(std::tuple<Args...>& t) {
	bool res = equals_data(std::get<I>(t));
	return for_equals_data<I + 1, Args...>(t) && res;
}

template<typename... Args>
bool equals_data(std::tuple<Args...>& tuple) {
	return for_equals_data(tuple);
}

