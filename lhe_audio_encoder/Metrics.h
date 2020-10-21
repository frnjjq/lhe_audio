#pragma once
#include <stdint.h>

class Metrics {
public:
	static void printPSNR(uint16_t* orig, uint16_t* quantized, uint32_t size);
	static void printHopStats(uint8_t* symbols, uint32_t size);
};