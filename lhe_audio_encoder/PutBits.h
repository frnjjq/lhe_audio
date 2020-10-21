#pragma once
#include <stdint.h>

class PutBits {
public:
	PutBits(uint8_t* bitstream, uint32_t bitstreamSize);
	void putBits(uint32_t bits, uint8_t numBits);
	uint32_t finishBitstream();
	static void unit_test();
private:

	void flushCurrentBuffer();
	uint8_t* bitstream;
	uint32_t bitstreamSize;
	uint32_t bitstreamPos;
	uint32_t bufferLeft;
	uint32_t buffer;
};
