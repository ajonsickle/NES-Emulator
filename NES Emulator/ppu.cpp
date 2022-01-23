#include "ppu.h"
#include <cstdint>

ppu::ppu() {

}
ppu::~ppu() {

}

uint8_t ppu::read(uint16_t address, bool readOnly) {
	uint8_t data = 0x00;

	switch (address) {
	case 0x0000:

		break;
	case 0x0001:

		break;
	case 0x0002:

		break;
	case 0x0003:

		break;
	case 0x0004:

		break;
	case 0x0005:

		break;
	case 0x0006:

		break;
	case 0x0007:

		break;
	
	}

	return data;
}

void ppu::write(uint16_t address, uint8_t data) {
	switch (address) {
	case 0x0000:

		break;
	case 0x0001:

		break;
	case 0x0002:

		break;
	case 0x0003:

		break;
	case 0x0004:

		break;
	case 0x0005:

		break;
	case 0x0006:

		break;
	case 0x0007:

		break;

	}
}

uint8_t ppu::ppuread(uint16_t address, bool readOnly) {
	uint8_t data = 0x00;
	address = address & 0x3FFF;

	return data;
}

void ppu::ppuwrite(uint16_t address, uint8_t data) {
	address = address & 0x3FFF;
}