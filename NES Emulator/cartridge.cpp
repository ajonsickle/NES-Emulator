#include "cartridge.h"
#include <string>
#include <iostream>
#include <fstream>

cartridge::cartridge(const std::string& romfile) {
	/*
	iNES:
	Header (16 bytes)
	Trainer, if present (0 or 512 bytes)
	PRG ROM data (16384 * x bytes)
	CHR ROM data, if present (8192 * y bytes)
	PlayChoice INST-ROM, if present (0 or 8192 bytes)
	PlayChoice PROM, if present (16 bytes Data, 16 bytes CounterOut) (this is often missing, see PC10 ROM-Images for details)

	- header format where 1 integer is 1 byte:
	0-3: Constant $4E $45 $53 $1A ("NES" followed by MS-DOS end-of-file)
	4: Size of PRG ROM in 16 KB units
	5: Size of CHR ROM in 8 KB units (Value 0 means the board uses CHR RAM)
	6: Flags 6 - Mapper, mirroring, battery, trainer
	7: Flags 7 - Mapper, VS/Playchoice, NES 2.0
	8: Flags 8 - PRG-RAM size (rarely used extension)
	9: Flags 9 - TV system (rarely used extension)
	10: Flags 10 - TV system, PRG-RAM presence (unofficial, rarely used extension)
	11-15: Unused padding (should be filled with zero, but some rippers put their name across bytes 7-15)*/
	struct header {
		// char is 1 byte
		std::string filenames[4];
		uint8_t prgromsize;
		uint8_t chrromsize;
		uint8_t flag6;
		uint8_t flag7;
		uint8_t flag8;
		uint8_t flag9;
		uint8_t flag10;
		std::string padding[5];
	} headerr;

	std::ifstream streamreader;
	streamreader.open(romfile, std::ifstream::binary);
	if (streamreader.is_open()) {
		// extracts n characters from the stream and stores them in the array pointed to by s, n is the length of the header and s is a char pointer to the memory location of header
		streamreader.read((char*)&headerr, sizeof(header));
		// if trainer present
		if (headerr.flag6 & 0x04) {
			// offset 512 from current position in the stream
			streamreader.seekg(512, std::ios_base::cur);
		}
		// determine whether mapper 1 or 2 is being used. low nibble of flag 6 contains mapper number so it must be shifted left by 4, upper nibble of flag 7 contains mapper number so it stays as is
		mapper = (headerr.flag7) | (headerr.flag6 << 4);
		// nes roms have multiple file formats
		uint8_t filetype = 1;
		if (filetype == 0) {

		} 
		if (filetype == 1) {
			// take the number of banks of the PRG and CHR ROM and resize the PRG and CHR vector accordingly, then read from the rom file to write into the vector
			prgsections = headerr.prgromsize;
			prgdata.resize(prgsections * 16 * 1024);
			streamreader.read((char*)prgdata.data(), prgdata.size());

			chrsections = headerr.chrromsize;
			chrdata.resize(chrsections * 8 * 1024);
			streamreader.read((char*)chrdata.data(), chrdata.size());
		}
		if (filetype == 2) {

		}
		//ignoring playchoice stuff
		streamreader.close();
	}
	
}
cartridge::~cartridge() {

}
// doesn't actually execute any reading or writing, just returns whether or not the cartridge is handling this specific read/write
bool cartridge::read(uint16_t address, uint8_t & data) {

}
bool cartridge::write(uint16_t address, uint8_t data) {

}
bool cartridge::ppuRead(uint16_t address, uint8_t& data) {

}
bool cartridge::ppuWrite(uint16_t address, uint8_t data) {

}