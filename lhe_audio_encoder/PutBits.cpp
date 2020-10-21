#include "PutBits.h"

PutBits::PutBits(uint8_t* bitstream, uint32_t bitstreamSize) : 
	bitstream(bitstream), 
	bitstreamSize(bitstreamSize), 
    bufferLeft(32),
	buffer(0), 
    bitstreamPos(0)
{}

void PutBits::putBits(uint32_t bits, uint8_t numBits) {
    if (numBits < bufferLeft) {
        buffer = (buffer << numBits) | bits;
        bufferLeft -= numBits;
    }
    else {
        uint32_t prevStored;
        buffer <<= bufferLeft;
        buffer |= bits >> (numBits - bufferLeft);
        prevStored = bufferLeft;
        flushCurrentBuffer();
        bufferLeft -= (numBits - prevStored);
        buffer = bits;
    }
}
uint32_t PutBits::finishBitstream() {
    uint8_t prevBufferLeft = bufferLeft;
    buffer <<= bufferLeft;
    flushCurrentBuffer();
    return bitstreamPos - prevBufferLeft/8;
}

void PutBits::flushCurrentBuffer() {
    if (bitstreamPos < bitstreamSize) {
        bitstream[bitstreamPos] = buffer >> 24;
        bitstreamPos++;
    }
    if (bitstreamPos < bitstreamSize) {
        bitstream[bitstreamPos] = buffer >> 16;
        bitstreamPos++;
    }
    if (bitstreamPos < bitstreamSize) {
        bitstream[bitstreamPos] = buffer >> 8;
        bitstreamPos++;
    }
    if (bitstreamPos < bitstreamSize) {
        bitstream[bitstreamPos] = buffer;
        bitstreamPos++;
    }
    buffer = 0;
    bufferLeft = 32;
}

void PutBits::unit_test() {
    uint8_t bitstream[600] = {0};
    PutBits pb(bitstream, 600);

    pb.putBits(128, 32);
    pb.putBits(1, 7);
    int bitstreamRes= pb.finishBitstream();
}