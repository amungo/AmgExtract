#ifndef UTILS_H
#define UTILS_H

#include <cstdint>
#include <string>
#include <fstream>

int64_t get_file_size( const char* fname );
int64_t get_file_size( std::ifstream& fs  );
void decode_file( std::ifstream& fin, std::ofstream& fout, int ch, size_t buf_size = 1024 * 1024 );
std::string add_chan_idx( const std::string& fname, int ch );

#endif // UTILS_H
