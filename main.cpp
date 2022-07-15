#include <iostream>
#include <string>
#include <set>
#include <vector>

#include "utils.h"
#include "decoder.h"

using namespace std;

const int CHIP_NUM = 2;
const int CH_NUM = 4;
const int CH_IDX_START = 3; //2;
const int ARG_MAX_CNT = 7; //6;

int main(int argc, char *argv[])
{
    if ( argc < 2 ) {
        cout << "Usage:" << endl;
        cout << "AmgExtractor input_file [0|1|2|3]" << endl;
        cout << "   input_file - file with 4chan 2bit signal" << endl;
        cout << "   0|1 - number of NT chip";
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
    int chip_idx = 0;
    string fname_gyro = fname + ".gyro";


    if ( argc == 2 ) {
        chans_idx.insert( 0 );
        chans_idx.insert( 1 );
        chans_idx.insert( 2 );
        chans_idx.insert( 3 );
    } else {
        int max_argc = argc > ARG_MAX_CNT ? ARG_MAX_CNT : argc;
        chip_idx = stoi(string(argv[2]));
        if(chip_idx < 0 || chip_idx >= CHIP_NUM) {
            throw invalid_argument( "Bad chip index " + to_string(chip_idx) );
        }
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
    }

    for ( int idx : chans_idx ) {
        cout << "extracting channel " << idx << endl;
        try {
            std::ifstream fin  ( fname.c_str(), std::ifstream::binary );
            std::ofstream fout ( add_chan_idx( fname, idx ).c_str(), std::ifstream::binary );
            std::ofstream fout_gyro (fname_gyro, std::ifstream::binary );

            decode_file( fin, fout, fout_gyro, chip_idx, idx );

            fin.close();
            fout.close();
        } catch ( std::exception& e ) {
            cout << e.what();
            return 1;
        }
    }

    cout << "Statistica:" << endl;
    printf( "        %6d%6d%6d%6d\n", -3, -1, 1, 3 );
    for ( int idx : chans_idx ) {
        cout << "[ " << idx << " ] ";
        std::vector<double> stats = decoder_stat( idx );
        printf( "   %6.2f%6.2f%6.2f%6.2f\n", stats[3], stats[2], stats[0], stats[1] );
    }

    return 0;
}
