#include "bus.h"

bus::bus() {

	for (auto& i : ram) i = 0x0000;

}
void bus::write(uint16_t address, uint8_t data) {
	if (address >= 0x0000 && address <= 0xFFFF) {
		ram[address] = data;
	}
}
uint8_t bus::read(uint16_t address) {
	if (address >= 0x0000 && address <= 0xFFFF) {
		return ram[address];
	}
	return 0x0000;
}