#include <iostream>
#include <string>
#include <set>

#include "utils.h"

using namespace std;

const int CH_NUM = 4;
const int CH_IDX_START = 2;
const int ARG_MAX_CNT = 6;

int main(int argc, char *argv[])
{
    if ( argc < 2 ) {
        cout << "Usage:" << endl;
        cout << "AmgExtractor input_file [0|1|2|3]" << endl;
        cout << "    input_file - file with 4chan 2bit signal" << endl;
        cout << "    0|1|2|3    - channels to extract" << endl;
        cout << endl;
        cout << "Example:" << endl;
        cout << "    Extract two channels (0 and 2) from file 'dump.bin'" << endl;
        cout << "    AmgExtractor dump.bin 0 2" << endl;
        cout << endl;
        cout << "    Extract all 4 channels from file 'dump.bin'" << endl;
        cout << "    AmgExtractor dump.bin" << endl;
        cout << endl;
        return 0;
    }

    string fname = argv[1];
    set<int> chans_idx;
    int max_argc = argc > ARG_MAX_CNT ? ARG_MAX_CNT : argc;
    for ( int i = CH_IDX_START; i < max_argc; i++ ) {
        try {
            int idx = stoi( string(argv[i]) );
            if ( idx < 0 || idx >= CH_NUM ) {
                throw invalid_argument( "Bad channel index " + to_string(idx) );
            }
            chans_idx.insert( idx );
        } catch ( invalid_argument& e ) {
            cout << e.what() << endl;
            cout << "Use indexes from 0 to " << CH_NUM - 1 << ". Space separated";
            return 0;
        }
    }

    for ( int idx : chans_idx ) {
        cout << "extracting channel " << idx << endl;
        try {
            FILE* fin  = fopen( fname.c_str(), "rb" );
            FILE* fout = fopen( add_chan_idx( fname, idx ).c_str(), "wb" );

            decode_file( fin, fout, idx );

            fclose(fin);
            fclose(fout);
        } catch ( std::exception& e ) {
            cout << e.what();
            return 1;
        }
    }

    return 0;
}
