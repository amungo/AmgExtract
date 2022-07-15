#include <stdexcept>
#include <string>
#include "decoder.h"

template< typename T_IN, typename T_OUT, int ch >
void decode_vector(const std::vector<T_IN> &src, std::vector<T_OUT> &out, int chip, int header) {
    if (  out.size() != ((src.size() - NUT4_GYRO_HEADER_SIZE) / 2)  ) {
        throw std::runtime_error( "decode_vector(): src size > out size" );
    }
    const uint8_t* pSrc = src.data() + header;
    if(chip)
        pSrc++;

    for ( size_t i = 0; i < out.size(); ++i ) {
        out[ i ] = decode2b<T_IN, T_OUT, ch>( *pSrc );
        pSrc += 2;
    }
}

void decode8_vector(const std::vector<uint8_t> &src, std::vector<int8_t> &out, int chip, int out_chan, int header) {
    switch ( out_chan ) {
    case 0: decode_vector<uint8_t, int8_t, 0>( src, out, chip, header); break;
    case 1: decode_vector<uint8_t, int8_t, 1>( src, out, chip, header); break;
    case 2: decode_vector<uint8_t, int8_t, 2>( src, out, chip, header); break;
    case 3: decode_vector<uint8_t, int8_t, 3>( src, out, chip, header); break;
    default: throw std::runtime_error("decode8_vector(): invalid channel " + std::to_string(out_chan) );
    }
}

std::vector<double> decoder_stat(int ch) {
    double overall = 0.0;
    for ( int i = 0; i < 4; i++ ) {
        overall += global_decoder_statistic[ ch ][ i ];
    }
    if ( overall < 0.1 ) {
        overall = 1.0;
    }
    std::vector<double> statistic;
    statistic.resize(4);
    for ( int i = 0; i < 4; i++ ) {
        statistic[ i ] = global_decoder_statistic[ ch ][ i ] / overall;
    }
    return statistic;

}
