#include "bus.h"
#include "ppu.h"

bus::bus() {

	cpu.connectBus(this);
	for (auto& i : ram) i = 0x0000;

}

bus::~bus()
{
}

void bus::write(uint16_t address, uint8_t data) {
	if (address >= 0x0000 && address <= 0x1FFF) {
		ram[address & 0x07FF] = data;
	}
	else if (address >= 0x2000 && address <= 0x3FFF) {
		ppu.write(address & 0x0007, data);
	}
}
uint8_t bus::read(uint16_t address, bool readOnly) {
	uint8_t data = 0x00;
	if (address >= 0x0000 && address <= 0x1FFF) {
		// mirroring
		data = ram[address & 0x07FF];
	}
	else if (address >= 0x2000 && address <= 0x3FFF) {
		data = ppu.read(address & 0x0007, readOnly);
	}
	return data;
}

void bus::insertCartridge(const std::shared_ptr<cartridge>& cartridge) {
	this->cart = cartridge;
	ppu.connectCartridge(cartridge);
}
void bus::reset() {
	cpu.reset();
	clockCounter = 0;
}
void bus::clock() {

}