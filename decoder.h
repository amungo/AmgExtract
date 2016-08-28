#ifndef DECODER_H
#define DECODER_H

#include <cstdint>
#include <vector>

void decode8_vector( const std::vector<uint8_t>& src, std::vector<int8_t>& out, int out_chan );

template< typename T_IN, typename T_OUT, int ch >
void decode_vector( const std::vector<T_IN>& src, std::vector<T_OUT>& out );

template< typename T_IN, typename T_OUT, int ch >
T_OUT decode2b( T_IN incode ) {
    static T_OUT table[ 4 ] = { (T_OUT)1, (T_OUT)3, (T_OUT)-1, (T_OUT)-3};
    return table[ ( ( incode >> ( ch * 2 ) ) & 0x03 ) ];
}



#endif // DECODER_H
