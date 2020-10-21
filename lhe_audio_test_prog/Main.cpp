
#include <stdint.h>
#include <iostream>

#include "AudioFile.h"
#include "LheAudioEncoder.h"
#include "Metrics.h"

int main(int argc, char* argv[]);
uint16_t inline audio_float2uint(double sample);
double inline audio_uint2float(uint16_t sample);

int main(int argc, char* argv[]) {

	AudioFile<double> audioFile;
	audioFile.load(argv[1]);

	int numSamples = audioFile.getNumSamplesPerChannel();
	int numChannels = audioFile.getNumChannels();

	LheAudioEncoder lhe(numSamples);

	if (numChannels == 1) {
		uint16_t* samples = new  uint16_t[numSamples];
		for (int sample = 0; sample < numSamples; sample++) {
			samples[sample] = audio_float2uint(audioFile.samples[0][sample]);
		}
		uint32_t bitstreamSize;
		uint8_t* bitstream = lhe.encode(samples, numSamples, &bitstreamSize);
		std::cout << "Original size -> " << numSamples * numChannels * 2 / 1000.0 << " Kb" << std::endl;
		std::cout << "Resulted size -> " << bitstreamSize / 1000.0 << " Kb" << std::endl;

		uint32_t quantizedSize;
		uint16_t* quantized = lhe.getQuantizedSignal(&quantizedSize);
		for (int sample = 0; sample < numSamples; sample++) {
			audioFile.samples[0][sample] = audio_uint2float(quantized[sample]);
		}
		//Metrics::printPSNR(samples, outSamples, numSamples);
		Metrics::printPSNR(samples, quantized, numSamples);
		delete[] samples;
	}
	else if (numChannels == 2) {
		uint16_t* samples1 = new  uint16_t[numSamples];
		uint16_t* samples2 = new  uint16_t[numSamples];

		for (int sample = 0; sample < numSamples; sample++) {
			samples1[sample] = audio_float2uint(audioFile.samples[0][sample]);
			samples2[sample] = audio_float2uint(audioFile.samples[1][sample]);
		}
		uint32_t bitstreamSize1;
		uint32_t bitstreamSize2;
		uint8_t* bitstream = lhe.encode(samples1, numSamples, &bitstreamSize1);

		uint32_t quantizedSize;
		uint16_t* quantized = lhe.getQuantizedSignal(&quantizedSize);
		for (int sample = 0; sample < numSamples; sample++) {
			audioFile.samples[0][sample] = audio_uint2float(quantized[sample]);
		}

		bitstream = lhe.encode(samples1, numSamples, &bitstreamSize2);
		quantized = lhe.getQuantizedSignal(&quantizedSize);
		for (int sample = 0; sample < numSamples; sample++) {
			audioFile.samples[1][sample] = audio_uint2float(quantized[sample]);
		}
		delete[] samples1;
		delete[] samples2;
	}

	audioFile.save(argv[2]);
	return 0;
}

uint16_t inline audio_float2uint(double sample) {
	return static_cast<uint16_t>(((sample + 1) / 2) * UINT16_MAX);
}

double inline audio_uint2float(uint16_t sample) {
	return (2.0 * sample / UINT16_MAX) - 1.0;
}