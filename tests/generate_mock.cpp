
#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <tuple>
#include <utility>
#include <cmath>

uint64_t randll() {
	union {
		uint64_t v;
		struct {
			uint16_t r[4];
		} r;
	};
	for(uint16_t& w : r.r)
		w = rand();
	return v;
}

int mock_generate_data_level = 0;
class assure_level {
public:
	assure_level() { mock_generate_data_level++; }
	~assure_level() { mock_generate_data_level--; }
};

size_t get_array_size() {
	return (randll()%123457) / ((mock_generate_data_level*13)+1);
}

template<typename T>
void mock_generate_data(T& object);

#define MOCK_INTEGER(TYPE) \
void mock_generate_data(TYPE& v) { \
	assure_level o; \
	v = randll(); \
	for(int i=1; i<sizeof(TYPE) && v; ++i) \
		v = randll()%v; \
}

MOCK_INTEGER(uint8_t);
MOCK_INTEGER(uint16_t);
MOCK_INTEGER(uint32_t);
MOCK_INTEGER(uint64_t);
MOCK_INTEGER(int8_t);
MOCK_INTEGER(int16_t);
MOCK_INTEGER(int32_t);
MOCK_INTEGER(int64_t);

#undef MOCK_INTEGER

void mock_generate_data(std::string& str) {
	assure_level o;
	size_t size = get_array_size();
	str.resize(size);
	for(size_t i=0; i<size; ++i)
		mock_generate_data(str[i]);
}

template<typename T>
void mock_generate_data(std::vector<T>& arr) {
	assure_level o;
	size_t size = get_array_size();
	arr.resize(size);
	for(size_t i=0; i<size; ++i)
		mock_generate_data(arr[i]);
}

template<typename K, typename V>
void mock_generate_data(std::map<K, V>& map) {
	assure_level o;
	size_t size = get_array_size();
	for(size_t i=0; i<size; ++i) {
		K k;
		mock_generate_data(k);
		mock_generate_data(map[k]);
	}
}

void mock_generate_data(bool& v) {
	assure_level o;
	v = rand() & 1;
}

#define MOCK_REAL(TYPE) \
void mock_generate_data(TYPE& v) { \
	assure_level o; \
	v = randll(); \
	if(rand() & 1) v = -v; \
	v = powl(2, (randll()%123)-(randll()%123)); \
}

MOCK_REAL(float);
MOCK_REAL(double);
MOCK_REAL(long double);

#undef MOCK_REAL



template<std::size_t I = 0, typename... Args>
inline typename std::enable_if<I == sizeof...(Args), void>::type
for_mock_generate_data(std::tuple<Args...>& t) {
}

template<std::size_t I = 0, typename... Args>
inline typename std::enable_if<I < sizeof...(Args), void>::type
for_mock_generate_data(std::tuple<Args...>& t) {
	mock_generate_data(std::get<I>(t));
	for_mock_generate_data<I + 1, Args...>(t);
}

template<typename... Args>
void mock_generate_data(std::tuple<Args...>& tuple) {
	for_mock_generate_data(tuple);
}

