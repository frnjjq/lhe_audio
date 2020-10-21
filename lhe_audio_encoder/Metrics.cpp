#include<cmath>
#include <iostream>
#include "Metrics.h"

void Metrics::printPSNR(uint16_t* orig, uint16_t* quantized, uint32_t size) {

	double mse = 0;
	double psnr = 0;

	for (unsigned int i = 0; i < size; i++) {
		mse += pow(quantized[i] - (int)orig[i], 2);
	}
	mse /= size;
	if (mse == 0.0)
		std::cout << "PSNR = Inf dB" << std::endl;
	else {
		psnr = 10 * log(pow(UINT16_MAX, 2) / mse) / log(10);
		std::cout << "PSNR = " << psnr << " dB" << std::endl;
	}
}

void Metrics::printHopStats(uint8_t * symbols, uint32_t size) {
	double hop_count[9] = { 0 };

	for (unsigned int i = 0; i < size; i++) {
		if (symbols[i] >= 9)
			std::cout << "Weird hop detected while getting stats" << symbols[i] << std::endl;
		else
			hop_count[symbols[i]]++;
	}

	std::cout << "Printing hops distribution:" << std::endl;
	std::cout << "Hop,probability" << std::endl;
	for (unsigned int i = 0; i < 9; i++) {
		std::cout << i <<"," << hop_count[i]/ size << std::endl;
	}
}
