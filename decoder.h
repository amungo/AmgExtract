#ifndef DECODER_H
#define DECODER_H

#include <cstdint>
#include <vector>
#include "utils.h"

void decode8_vector( const std::vector<uint8_t>& src, std::vector<int8_t>& out, int chip, int out_chan, int header = NUT4_GYRO_HEADER_SIZE);

template< typename T_IN, typename T_OUT, int ch >
void decode_vector( const std::vector<T_IN>& src, std::vector<T_OUT>& out, int chip);

static double global_decoder_statistic[4][4] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

template< typename T_IN, typename T_OUT, int ch >
T_OUT decode2b( T_IN incode ) {
    static T_OUT table[ 4 ] = { (T_OUT)1, (T_OUT)3, (T_OUT)-1, (T_OUT)-3};
    const int idx = ( incode >> ( ch * 2 ) ) & 0x03;
    global_decoder_statistic[ch][idx] += 1.0;
    return table[ idx ];
}

std::vector<double> decoder_stat( int ch );

#endif // DECODER_H
