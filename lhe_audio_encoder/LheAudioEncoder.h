#pragma once
#include <stdint.h>

#include "Quantizer.h"
#include "EntropicEncoder.h"

class LheAudioEncoder {
public:
	LheAudioEncoder(uint32_t maxSamples = DEFAULT_MAX_SAMPLES);
	~LheAudioEncoder();
	uint8_t* encode(uint16_t* data, uint32_t size, uint32_t* bitstream_size);
	uint16_t* getQuantizedSignal(uint32_t * size);
private:

	void resizeBuffers(uint32_t maxSamples);
	void freeBuffers();

	const static uint32_t DEFAULT_MAX_SAMPLES = 44100;

	Quantizer quant;
	EntropicEncoder entropic;
	uint32_t sampleMaxSize;
	uint32_t lastDataSize;
	uint8_t* symbols;
	uint16_t* quantized;
	uint8_t* bitstream;
};