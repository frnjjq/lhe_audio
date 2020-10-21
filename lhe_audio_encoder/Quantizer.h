#pragma once

#include <stdint.h>
#include <iostream>

class Quantizer {
public:
	Quantizer();
	~Quantizer();
	void quantize(uint16_t* signal, uint32_t signal_size, uint8_t* symbols, uint16_t* quantized);
private:

	const uint8_t MIN_H1 = 30;//4;
	const uint8_t MAX_H1 = 80;//10;
	const uint8_t INIT_H1 = 40;//5;
	const uint8_t RANGE_H1 = MAX_H1 - MIN_H1 + 1;

	const uint8_t HOP_N4 = 0;
	const uint8_t HOP_N3 = 1;
	const uint8_t HOP_N2 = 2;
	const uint8_t HOP_N1 = 3;
	const uint8_t HOP_0 = 4;
	const uint8_t HOP_P1 = 5;
	const uint8_t HOP_P2 = 6;
	const uint8_t HOP_P3 = 7;
	const uint8_t HOP_P4 = 8;

	const int MINIMUM_VALUE = 0;
	const int MAXIMUM_VALUE = UINT16_MAX - MINIMUM_VALUE;
	
	const double MAXR = 4.0;
	const double MINR = 1.0;
	const double RANGE = 0.8;

	uint16_t* cache;

	void generateCache();
	uint8_t get_hop(uint16_t orig, uint16_t pred, uint8_t h1, uint16_t* result);
};