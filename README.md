# AmgExtract
Data converter for multi-channel 2-bits dump files.
It can extract several channels from output files of AmungoFx3Dumper
and convert data to usual _signed char_ format.

AmungoFx3Dumper is located here: https://github.com/amungo/AmungoFx3Dumper

## Usage

AmgExtractor input_file (0|1)  [0|1|2|3]
* 0|1 - Number of NT chip to extract
* input_file - file with 2 NT chips 4chan 2bit signal
* 0|1|2|3    - channels to extract, numbering from 0 to 3

## Example

* Extract two channels (0 and 2) from file 'dump.bin'
 * AmgExtractor dump.bin 0 2
* Extract all 4 channels from file 'dump.bin' from second NT chip
 * AmgExtractor dump.bin 1
