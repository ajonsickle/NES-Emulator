#include "cartridge.h"
#include <string>
#include <iostream>
#include <fstream>
#include "mapper000.h"
#include "mapper001.h"


cartridge::cartridge(const std::string& romfile) {
	/*
	https://wiki.nesdev.org/w/index.php/INES
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
		char filenames[4];
		uint8_t prgromsize;
		uint8_t chrromsize;
		uint8_t flag6;
		uint8_t flag7;
		uint8_t flag8;
		uint8_t flag9;
		uint8_t flag10;
		char padding[5];
	};
	header headerr;


	std::ifstream streamreader;
	streamreader.open(romfile, std::ifstream::binary);
	if (streamreader.is_open()) {
		// istream& read (char* s, streamsize n);
		// extracts n characters from the stream and stores them in the array pointed to by s, n is the length of the header and s is a char pointer to the memory location of header
		streamreader.read((char*)&headerr, sizeof(header));
		// if trainer present
		if ((headerr.flag6 & 0x04) == 1) {
			// offset 512 from current position in the stream cuz trainer is useless and doesnt exist on a cartridge 
			streamreader.seekg(512, std::ios_base::cur);
		}
		// determine which mapper is being used. first 4 bits of flag 6 contain lower nibble of mapper number so it must be shifted right by 4, first 4 bits of flag 7 contain high nibble of mapper number so it stays as is. OR them together to get a full mapper ID
		mapperid = (headerr.flag7) | (headerr.flag6 >> 4);
		// last bit of flag 6 contains mirror mode 
		if ((headerr.flag6 & 0x01) == 1) {
			mirrorMode = "vertical";
		}
		else {
			mirrorMode = "horizontal";
		}

		// take the number of banks of the PRG and CHR ROM and resize the PRG and CHR vector accordingly, then read from the rom file to write into the vector
			prgsections = headerr.prgromsize;
			if (prgsections == 0) {
				prgdata.resize(16 * 1024);
			}
			else {
				prgdata.resize(prgsections * 16 * 1024);
			}
			
			streamreader.read((char*)prgdata.data(), prgdata.size());

			chrsections = headerr.chrromsize;
			if (chrsections == 0) {
				chrdata.resize(8 * 1024);
			}
			else {
				chrdata.resize(chrsections * 8 * 1024);
			}

			streamreader.read((char*)chrdata.data(), chrdata.size());

		// select mapper
		switch (mapperid) {
		case 0:
			//mapper 000
			mapperptr = std::make_shared<mapper000>(prgsections, chrsections);
			break;
		case 1:
			// mapper 001
			mapperptr = std::make_shared<mapper001>(prgsections, chrsections);
			break;
		}

		streamreader.close();
	}

}

void cartridge::reset() {
	mapperptr->reset();
}

// data is passed as a ref so that it can be edited in the function
bool cartridge::read(uint16_t address, uint8_t& data) {
	// cartridge gets first priority on cpu reads so it can handle program rom if the mapper being used needs to carry out a read 
	uint32_t mapped_address = 0;
	if (mapperptr->mapperRead(address, mapped_address, data)) {
		// check that the mapper has not already set the address 
		if (mapped_address != 0b1111111111111111) {
			data = prgdata[mapped_address];
		}
		return true;
	}
	else return false;
}
bool cartridge::write(uint16_t address, uint8_t data) {
	// same as read 
	uint32_t mapped_address = 0;
	if (mapperptr->mapperWrite(address, mapped_address, data)) {
		if (mapped_address != 0b1111111111111111) {
			prgdata[mapped_address] = data;
		}
		return true;
	}
	else return false;
}
bool cartridge::ppuread(uint16_t address, uint8_t& data) {
	// cartridge gets first priority on ppu reads so it can handle character rom if the mapper being used needs to carry out a read 
	uint32_t mapped_address = 0;
	if (mapperptr->ppuMapperRead(address, mapped_address)) {
		data = chrdata[mapped_address];
		return true;
	}
	else return false;
}
bool cartridge::ppuwrite(uint16_t address, uint8_t data) {
	// same as write
	uint32_t mapped_address = 0;
	if (mapperptr->ppuMapperWrite(address, mapped_address)) {
		chrdata[mapped_address] = data;
		return true;
	}
	else return false;
}
std::string cartridge::getMirror() {
	std::string mode = mapperptr->mirror();
	// some mappers will not set the mirror mode, if this is the case then it is defined by the hardware (cartridge) so just return the stored variable 
	if (mode == "hardware") {
		return mirrorMode;
	}
	// otherwise return the mirror mode set by the mapper 
	else {
		return mode;
	}
}
