#ifndef UTILS_H
#define UTILS_H

#include <cstdint>
#include <string>
#include <fstream>

#define NUT4_GYRO_DATA_PACKET_SIZE 16384
#define NUT4_GYRO_HEADER_SIZE   18 // (6 bytes 3xADXRS450 + 12 bytes ADXL357)

int64_t get_file_size( const char* fname );
int64_t get_file_size( std::ifstream& fs  );
void decode_file( std::ifstream& fin, std::ofstream& fout,  std::ofstream& fout_gyro, int chip, int ch, size_t buf_size = NUT4_GYRO_DATA_PACKET_SIZE );
std::string add_chan_idx( const std::string& fname, int ch );

#endif // UTILS_H
