#include <math.h>

#include "Quantizer.h"

Quantizer::Quantizer() {
	cache = new uint16_t[(UINT16_MAX + 1) * RANGE_H1 * 3];
	generateCache();
}

Quantizer::~Quantizer() {
	delete[] cache;
}

void Quantizer::quantize(uint16_t* signal, uint32_t signal_size, uint8_t* symbols, uint16_t* quantized) {
	int  h1 = INIT_H1;
	bool small_hop, last_small_hop = true;
	uint16_t pred;

	for (uint32_t i = 0; i < signal_size; i++) {
		if (i == 0)
			pred = UINT16_MAX / 2;
		else
			pred = quantized[i - 1];

		symbols[i] = get_hop(signal[i], pred, h1, &(quantized[i]));

		small_hop = symbols[i] <= HOP_P1 && symbols[i] >= HOP_N1 ? true : false;
		h1 = small_hop && last_small_hop && h1 > MIN_H1 ? h1 - 1 : MAX_H1;
		last_small_hop = small_hop;
	}
}

void Quantizer::generateCache()
{
	double rpos, rneg;
	int quant;

	for (int pred = 0; pred <= UINT16_MAX; pred++) {
		for (int h1 = MIN_H1; h1 <= MAX_H1; h1++) {
			rpos = pow(RANGE * ((MAXIMUM_VALUE - pred) / h1), 1.0 / 3.0);
			rpos = rpos > MAXR ? MAXR : rpos < MINR ? MINR : rpos;
			rneg = pow(RANGE * ((pred - MINIMUM_VALUE) / h1), 1.0 / 3.0);
			rneg = rneg > MAXR ? MAXR : rneg < MINR ? MINR : rneg;

			quant = pred - (int)round(h1 * rneg * rneg * rneg);
			quant = quant > MAXIMUM_VALUE ? MAXIMUM_VALUE : quant < MINIMUM_VALUE ? MINIMUM_VALUE : quant;
			cache[pred * RANGE_H1 * 3 + (h1 - MIN_H1) * 3] = quant;

			quant = pred - (int)round(h1 * rneg * rneg);
			quant = quant > MAXIMUM_VALUE ? MAXIMUM_VALUE : quant < MINIMUM_VALUE ? MINIMUM_VALUE : quant;
			cache[pred * RANGE_H1 * 3 + (h1 - MIN_H1) * 3 + 1] = quant;

			quant = pred - (int)round(h1 * rneg);
			quant = quant > MAXIMUM_VALUE ? MAXIMUM_VALUE : quant < MINIMUM_VALUE ? MINIMUM_VALUE : quant;
			cache[pred * RANGE_H1 * 3 + (h1 - MIN_H1) * 3 + 2] = quant;
		}
	}
}

uint8_t Quantizer::get_hop(uint16_t orig, uint16_t pred, uint8_t h1, uint16_t* result)
{
	int min_error, next_error, next_quantum;
	uint8_t hop;
	uint16_t quantum;

	hop = HOP_0;
	quantum = pred;
	min_error = abs(orig - pred);

	if (min_error > h1 / 2) {
		if (orig >= pred) { // Hops positivos
			if (pred + h1 <= UINT16_MAX) {
				next_quantum = pred + h1;
				next_error = abs(orig - next_quantum);
				if (next_error < min_error) {
					hop = HOP_P1;
					quantum = next_quantum;
					min_error = next_error;
					next_quantum = UINT16_MAX - cache[(UINT16_MAX - pred) * RANGE_H1 * 3 + (h1 - MIN_H1) * 3 + 2];
					next_error = abs(orig - next_quantum);

					if (next_error < min_error) {
						hop = HOP_P2;
						quantum = next_quantum;
						min_error = next_error;
						next_quantum = UINT16_MAX - cache[(UINT16_MAX - pred) * RANGE_H1 * 3 + (h1 - MIN_H1) * 3 + 1];
						next_error = abs(orig - next_quantum);

						if (next_error < min_error) {
							hop = HOP_P3;
							quantum = next_quantum;
							min_error = next_error;
							next_quantum = UINT16_MAX - cache[(UINT16_MAX - pred) * RANGE_H1 * 3 + (h1 - MIN_H1) * 3 + 0];
							next_error = abs(orig - next_quantum);

							if (next_error < min_error) {
								hop = HOP_P4;
								quantum = next_quantum;
								min_error = next_error;
							}
						}
					}
				}
			}
		}
		else if (orig < pred) {// Hops negativos
			if (pred - h1 >= 0) {
				next_quantum = pred - h1;
				next_error = abs(orig - next_quantum);
				if (next_error < min_error) {
					hop = HOP_N1;
					quantum = next_quantum;
					min_error = next_error;
					next_quantum = cache[pred * RANGE_H1 * 3 + (h1 - MIN_H1) * 3 + 2];
					next_error = abs(orig - next_quantum);

					if (next_error < min_error) {
						hop = HOP_N2;
						quantum = next_quantum;
						min_error = next_error;
						next_quantum = cache[pred * RANGE_H1 * 3 + (h1 - MIN_H1) * 3 + 1];
						next_error = abs(orig - next_quantum);

						if (next_error < min_error) {
							hop = HOP_N3;
							quantum = next_quantum;
							min_error = next_error;
							next_quantum = cache[pred * RANGE_H1 * 3 + (h1 - MIN_H1) * 3 + 0];
							next_error = abs(orig - next_quantum);

							if (next_error < min_error) {
								hop = HOP_N4;
								quantum = next_quantum;
								min_error = next_error;
							}
						}
					}
				}
			}
		}
	}
	quantum = quantum > MAXIMUM_VALUE ? MAXIMUM_VALUE : quantum < MINIMUM_VALUE ? MINIMUM_VALUE : quantum;
	*result = quantum;
	return hop;
}


