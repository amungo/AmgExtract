#include <stdexcept>
#include <string>
#include "decoder.h"


template< typename T_IN, typename T_OUT, int ch >
void decode_vector(const std::vector<T_IN> &src, std::vector<T_OUT> &out) {
    if ( out.size() < src.size() ) {
        throw std::runtime_error( "decode_vector(): src size > out size" );
    }

    for ( size_t i = 0; i < src.size(); ++i ) {
        out[ i ] = decode2b<T_IN, T_OUT, ch>( src[ i ] );
    }
}

void decode8_vector(const std::vector<uint8_t> &src, std::vector<int8_t> &out, int out_chan) {
    switch ( out_chan ) {
    case 0: decode_vector<uint8_t, int8_t, 0>( src, out ); break;
    case 1: decode_vector<uint8_t, int8_t, 1>( src, out ); break;
    case 2: decode_vector<uint8_t, int8_t, 2>( src, out ); break;
    case 3: decode_vector<uint8_t, int8_t, 3>( src, out ); break;
    default: throw std::runtime_error("decode8_vector(): invalid channel " + std::to_string(out_chan) );
    }
}
