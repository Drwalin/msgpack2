
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

#include "generate_mock.cpp"
#include "equals_mock.cpp"

using type1 = std::vector<
		std::tuple<
			std::string, std::vector<uint8_t>, std::vector<uint16_t>, std::vector<uint32_t>, std::vector<uint64_t>, std::vector<int8_t>, std::vector<int16_t>, std::vector<int32_t>, std::vector<int64_t>, std::vector<int>, std::vector<char>, std::vector<long>, std::vector<long long>, std::vector<float>, std::vector<double>, std::vector<long double>, bool
			, std::map<
				std::string,
				std::tuple<
					std::string, uint8_t, uint16_t, uint32_t, uint64_t, int8_t, int16_t, int32_t, int64_t, int, char, long, long long, float, double, long double, bool
				>
			>
		>
	>;


template<typename T>
bool test_suite() {
	T original, decoded;
	
	printf("\n\n generating mock data...");
	mock_generate_data(original);
	printf(" done\n");
	
	std::vector<uint8_t> buffer;
	
	printf(" writing mock data...");
	msgpack2::WriteBuffer(buffer, original);
	printf(" done\n");
	
	printf(" decoding data...");
	const uint8_t* b = buffer.data();
	msgpack2::Read(decoded, b, buffer.data()+buffer.size());
	printf(" done\n");
	
	printf(" comparing mock and decoded data...");
	bool result = equals(original, decoded);
	printf(" done\n");
	
	if(result) {
		printf(" test failed!\n");
	} else {
		printf(" test correct!\n");
	}
	
	return result;
}

int main() {
	bool random_predictable_default_srand = true;
	if(!random_predictable_default_srand)
		srand(time(NULL));
	
	test_suite<type1>();
	
	return false;
}

