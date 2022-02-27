#include "mapper000.h"

// https://wiki.nesdev.org/w/index.php?title=NROM
// https://www.copetti.org/images/consoles/nes/diagram.c464dfd123bab5f03a9f07be39056625d5de0a69f01e5f57eefdfd02811cbb33.png
mapper000::mapper000(uint8_t prgsections, uint8_t chrsections) : mapper(prgsections, chrsections) {

}
// & = ref 
bool mapper000::mapperRead(uint16_t address, uint32_t& mapped_address, uint8_t &data) {
	// rom data is held only in $8000-$FFFF
	if (address >= 0x8000 && address <= 0xFFFF) {
		// change mapping based on number of prg banks
		auto temp = 0x3FFF;
		if (prgsections1 > 1) temp = 0x7FFF;
		mapped_address = address & temp;
		return true;
	}
	return false;
}

bool mapper000::mapperWrite(uint16_t address, uint32_t& mapped_address, uint8_t data) {
	// rom data is held only in $8000-$FFFF
	if (address >= 0x8000 && address <= 0xFFFF) {
		// change mapping based on number of prg banks
		auto temp = 0x3FFF;
		if (prgsections1 > 1) temp = 0x7FFF;
		mapped_address = address & temp;
		return true;
	}
	return false;
}

bool mapper000::ppuMapperRead(uint16_t address, uint32_t& mapped_address) {
	// no mapping required for ppu
	if (address >= 0 && address <= 0x1FFF) {
		mapped_address = address;
		return true;
	}
	return false;
}

bool mapper000::ppuMapperWrite(uint16_t address, uint32_t& mapped_address) {
	if (address >= 0 && address <= 0x1FFF) {
		if (chrsections1 == 0) {
			mapped_address = address;
			return true;
		}
	}
	return false;
}

void mapper000::reset() {

}