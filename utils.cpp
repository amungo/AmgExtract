#include <stdexcept>
#include <iostream>
#include <fstream>
#include <string>

#include "utils.h"
#include "decoder.h"

using namespace std;

int64_t get_file_size(const char *fname) {
    std::ifstream f (fname, std::ifstream::binary);

    if ( f ) {
        int64_t sz = get_file_size( f );
        f.close();
        return sz;
    } else {
        throw runtime_error( "get_file_size(): File IO error - " + string(fname) );
    }
    return 0;
}

int64_t get_file_size(std::ifstream& f) {
    if ( f.is_open() ) {
        std::streampos old_pos = f.tellg();
        f.seekg(0, f.end);
        int64_t sz = f.tellg();
        f.seekg( old_pos, f.beg );
        return sz;
    } else {
        throw runtime_error( "get_file_size(): File error" );
    }
}

void decode_file(std::ifstream& fin, std::ofstream& fout, std::ofstream& fout_gyro, int chip, int ch, size_t buf_size)
{
    if ( !fin.is_open() || !fout.is_open() ) {
        throw runtime_error( "decode_file() file was not opened" );
    }
    int64_t all_size = get_file_size( fin );
    fin.seekg( 0 , fin.beg );

    vector<uint8_t> inbuf;
    vector<int8_t> outbuf;

    inbuf.resize( buf_size );
    outbuf.resize((buf_size - NUT4_GYRO_HEADER_SIZE) / 2);

    int64_t to_go = all_size;
    double last_percent = 0.0;
    double cur_percent = 0.0;
    while ( to_go >= (int64_t)inbuf.size() ) {
        fin.read( (char*)inbuf.data(), sizeof(uint8_t) * inbuf.size() );
        std::streamsize res = fin.gcount();
        if ( res != (int64_t)inbuf.size() ) {
            throw runtime_error( "decode_file() read error. Read " + to_string( res ) );
        }

        decode8_vector( inbuf, outbuf, chip, ch );
        fout.write( (const char*)outbuf.data(), sizeof(int8_t) * outbuf.size() );
        if ( fout.bad() ) {
            throw runtime_error( "decode_file() write error." );
        }

        // Write sensors data
        fout_gyro.write((const char*)inbuf.data(), NUT4_GYRO_HEADER_SIZE);

        //to_go -= outbuf.size();
        to_go -= inbuf.size();

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
