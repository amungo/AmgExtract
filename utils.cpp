#include <stdexcept>
#include <iostream>
#include <string>

#include "utils.h"
#include "decoder.h"

using namespace std;

int64_t get_file_size(const char *fname) {
    FILE* f = fopen(fname, "rb");
    if ( f ) {
        int64_t sz = get_file_size( f );
        fclose(f);
        return sz;
    } else {
        throw runtime_error( "get_file_size(): File IO error - " + string(fname) );
    }
    return 0;
}

int64_t get_file_size(FILE *f) {
    if ( f ) {
        int64_t old_pos = ftell(f);
        _fseeki64( f, 0, SEEK_END );
        int64_t sz = _ftelli64(f);
        _fseeki64( f, old_pos, SEEK_SET );
        return sz;
    } else {
        throw runtime_error( "get_file_size(): File ptr is null" );
    }
}

void decode_file(FILE *fin, FILE *fout, int ch, size_t buf_size) {
    if ( !fin || !fout ) {
        throw runtime_error( "decode_file() file ptr is null" );
    }
    int64_t all_size = get_file_size( fin );
    fseek( fin, 0, SEEK_SET );

    vector<uint8_t> inbuf;
    vector<int8_t> outbuf;

    inbuf.resize( buf_size );
    outbuf.resize( buf_size );

    int64_t to_go = all_size;
    double last_percent = 0.0;
    double cur_percent = 0.0;
    while ( to_go >= inbuf.size() ) {
        size_t res = fread( inbuf.data(), sizeof(uint8_t), inbuf.size(), fin );
        if ( res != inbuf.size() ) {
            throw runtime_error( "decode_file() read error. Read " + to_string( res ) );
        }
        decode8_vector( inbuf, outbuf, ch );
        res = fwrite( outbuf.data(), sizeof(int8_t), outbuf.size(), fout );
        if ( res != outbuf.size() ) {
            throw runtime_error( "decode_file() write error. Written " + to_string( res ) );
        }
        to_go -= res;

        cur_percent = ( 1.0 - (double)to_go / (double)all_size ) * 100.0;
        if ( cur_percent - last_percent > 1.0 ) {
            cout << "\r";
            cout << (int) cur_percent << "% complete  ";
        }
    }
    cout << endl;
}

std::string add_chan_idx( const string& fn, int ch ) {
    string str( fn );
    str.insert( fn.find_last_of("."), string( "_ch" ) + to_string( ch ) );
    return str;
}
