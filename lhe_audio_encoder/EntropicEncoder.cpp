#include <iostream>

#include "EntropicEncoder.h"
#include "PutBits.h"
EntropicEncoder::EntropicEncoder() {}
EntropicEncoder::~EntropicEncoder() {}


void EntropicEncoder::encode(uint8_t* symbols, uint32_t size, uint8_t* bitstream, uint32_t* bitstream_size) {

	PutBits pb(bitstream, size);
	for (unsigned int i = 0; i < size; i++) {
		uint8_t hop = symbols[i];
		pb.putBits(hop_value[hop], hop_size[hop]);
	}
	*bitstream_size = pb.finishBitstream();
	return;
}


void EntropicEncoder::unit_test() {
	EntropicEncoder enc;

	uint8_t symbols[16] = { 0, 1, 2,3,4,5,6,7,8,8,8,8,0,4,6,7 };
	uint8_t expected[16] = { 0x01, 0x02, 0x13, 0x44, 0x10, 0x00, 0x00, 0x00, 0x00, 0x18, 0x82 };
	uint8_t bitstream[16] = { 0 };
	uint32_t resSize;

	enc.encode(symbols, 16, bitstream, &resSize);

	if (resSize != 11)
		std::cout << "Error en size pa empezar" << resSize << std::endl;

	for (unsigned int i = 0; i < resSize; i++)
		if (expected[i] != bitstream[i])
			std::cout << "Bitstream no es igual" << i << " " << (int)expected[i] << " " << (int)bitstream[i] << std::endl;
}
