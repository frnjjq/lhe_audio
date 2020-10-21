

#include "LheAudioEncoder.h"
#include "Metrics.h"

LheAudioEncoder::LheAudioEncoder(uint32_t maxSamples) {
	resizeBuffers(maxSamples);
}

LheAudioEncoder::~LheAudioEncoder() {
	freeBuffers();
}

void LheAudioEncoder::resizeBuffers(uint32_t maxSamples) {
	freeBuffers();
	sampleMaxSize = maxSamples;
	symbols = new uint8_t[sampleMaxSize];
	quantized = new uint16_t[sampleMaxSize];
	bitstream = new uint8_t[sampleMaxSize];
}

void LheAudioEncoder::freeBuffers() {
	sampleMaxSize = 0;
	if (symbols != nullptr)
		delete[] symbols;
	if (quantized != nullptr)
		delete[] quantized;
	if (bitstream != nullptr)
		delete[] bitstream;
}

uint8_t* LheAudioEncoder::encode(uint16_t* data, uint32_t size, uint32_t* bitstream_size) {
	if (size > sampleMaxSize)
		resizeBuffers(size);
	lastDataSize = size;

	quant.quantize(data, size, symbols, quantized);
	Metrics::printHopStats(symbols, size);
	entropic.encode(symbols, size, bitstream, bitstream_size);

	return bitstream;
}

uint16_t* LheAudioEncoder::getQuantizedSignal(uint32_t* size) {
	*size = lastDataSize;
	return quantized;
}
