#pragma once

#include <stdint.h>

class EntropicEncoder {
public:
	EntropicEncoder();
	~EntropicEncoder();
	void encode(uint8_t* symbols, uint32_t size, uint8_t* bitstream, uint32_t* bitstream_size);
	static void unit_test();
private:
	uint8_t hop_size[9] = { 8,7,5,3,1,2,4,6,8 };
	uint8_t hop_value[9] = { 1,1,1,1,1,1,1,1,0 };
};
