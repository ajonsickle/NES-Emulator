#include "bus.h"
#include "ppu.h"

// bus constructor, calls connectBus method from cpu so an instance of bus is created in cpu class
bus::bus() {

	cpu.connectBus(this);
}

//bus destructor
bus::~bus()
{
}

/*
writes to either the cpu or ppu, however the cartridge has priority and can therefore override this method. as the NES' 8KB internal ram was held in memory address $0000 to $1FFF, 
if the address parameter is between these values the function is treated as a cpu write and the ram is edited accordingly. The NES actually only had 2KB internal ram from $0000 to $07FF, 
but a method called mirroring was used where the same memory could be accessed at multiple addresses so there was in reality 8KB of useable ram. must therefore perform an AND operation
between the address and 0x07FF. the ppu can also be written to, the NES PPU registers were held in memory address $2000 to $2007 with mirroring from $2008 to $3FFF
*/
void bus::write(uint16_t address, uint8_t data) {
	if (cart->write(address, data) == true) {

	}
	else if (address >= 0x0000 && address <= 0x1FFF) {
		ram[address & 0x07FF] = data;
	}
	else if (address >= 0x2000 && address <= 0x3FFF) {
		ppu.write(address & 0x0007, data);
	}
}
// mostly same as write
uint8_t bus::read(uint16_t address, bool readOnly) {
	uint8_t data = 0x00;
	if (cart->read(address, data) == true) {

	}
	else if (address >= 0x0000 && address <= 0x1FFF) {
		data = ram[address & 0x07FF];
	}
	else if (address >= 0x2000 && address <= 0x3FFF) {
		data = ppu.read(address & 0x0007, readOnly);
	}
	return data;
}

// adds the bus as an owner of the cartridge object under the shared pointer and connects the cartridge to the ppu so the bus and ppu have shared ownership using the shared_ptr
void bus::insertCartridge(const std::shared_ptr<cartridge>& cartridge) {
	this->cart = cartridge;
	ppu.connectCartridge(cartridge);
}
// calls the cpu reset method and sets the total clock counter to 0
void bus::reset() {
	cpu.reset();
	clockCounter = 0;
}
// the nes CPU operated at a speed of 1789773Hz while the PPU operated at a speed of 5369318Hz, so the PPU operated at roughly 3 times the speed of the CPU and therefore the cpu clock must be called once for every 3 ppu clock cycles.
void bus::clock() {
	ppu.clock();
	if (clockCounter % 3 == 0) {
		cpu.clock();
	}
	clockCounter++;
}