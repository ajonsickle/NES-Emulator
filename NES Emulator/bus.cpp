#include "bus.h"
#include "ppu.h"

// bus constructor, calls connectBus method from cpu so an instance of bus is created in cpu class
bus::bus() {

	cpu.connectBus(this);
}

/*
writes to either the cpu or ppu, however the cartridge has priority and can therefore override this method. as the NES' 8KB internal ram was held in memory address $0000 to $1FFF,
if the address parameter is between these values the function is treated as a cpu write and the ram is edited accordingly. The NES actually only had 2KB internal ram from $0000 to $07FF,
but a method called mirroring was used where the same memory could be accessed at multiple addresses so there was in reality 8KB of useable ram. must therefore perform an AND operation
between the address and 0x07FF. the ppu can also be written to, the NES PPU registers were held in memory address $2000 to $2007 with mirroring from $2008 to $3FFF
*/
void bus::write(uint16_t address, uint8_t data) {
	if (!cart->write(address, data)) {

		if (address >= 0 && address <= 0x1FFF) {
			if (address >= 0x07FF) {
				address = address & 0x07FF;
			}
			ram[address] = data;
		}
		else if (address >= 0x2000 && address <= 0x3FFF) {
			ppu.write(address, data);
		}
		else if (address == 0x4014) {
			// https://wiki.nesdev.org/w/index.php/PPU_registers#OAM_DMA_.28.244014.29_.3E_write
			// writes to this register contain the page to be written to where each page is 256 bytes
			dmapage = data;
			// set dmaActive flag to true
			dmaActive = true;
		}
		// the nes controller ports are stored at $4016 and $4017
		else if (address == 0x4016) {
			controller_shift = controller;
		}
		
	}
}
// mostly same as write
uint8_t bus::read(uint16_t address) {
	uint8_t data = 0;
	if (!cart->read(address, data)) {

		if (address >= 0 && address <= 0x1FFF) {
			if (address >= 0x07FF) {
				address = address & 0x07FF;
			}
			data = ram[address];
		}
		else if (address >= 0x2000 && address <= 0x3FFF) {
			data = ppu.read(address);
		}
		else if (address == 0x4016)
		{
			// https://wiki.nesdev.org/w/index.php/Standard_controller#Output_.28.244016.2F.244017_read.29
			// controller reading works by returning the most significant bit, shifting the controller register, returning the most significant bit, and repeating this cycle for each consecutive read
			// mask it with 10000000 and then return whether that is greater than 0 or not, as we want to return 00000001/00000000 not 10000000/00000000
				data = (controller_shift & 0b10000000) > 0;
				controller_shift = controller_shift << 1;
		}
	}
	return data;
}

// adds the bus as an owner of the cartridge object under the shared pointer and connects the cartridge to the ppu so the bus and ppu have shared ownership using the shared_ptr
void bus::insertcartridge(const std::shared_ptr<cartridge>& cartridge) {
	this->cart = cartridge;
	ppu.connectCartridge(cartridge);
}
// reset method
void bus::reset() {
	cart->reset();
	ppu.reset();
	cpu.reset();
	clockCounter = 0;
	dmapage = 0;
	dmaaddress = 0;
	dmadata = 0;
	dmaActive = false;
	dmaSynced = false;
}
// the nes CPU operated at a speed of 1789773Hz while the PPU operated at a speed of 5369318Hz, so the PPU operated at roughly 3 times the speed of the CPU and therefore the cpu clock must be called once for every 3 ppu clock cycles.
void bus::clock() {
	ppu.clock();
	if (clockCounter % 3 == 0) {
		// most roms write a copy of object attribute memory to somewhere in the cpus addressable ram and then copy it to oam each frame using the oamdma register
		// when dma is active, the cpu is suspended while the transfer takes place 
		// https://wiki.nesdev.org/w/index.php/PPU_OAM#DMA
		if (!dmaActive) {
			cpu.clock();
		}
		else {
			// waiting to synchronize clocks
			if (!dmaSynced) {
				// if clock cycle is odd, dma becomes synced so that the process can begin on the next cycle
				if (clockCounter % 2 == 1) {
					dmaSynced = true; 
				}
			}
			else {
				// on even cycles data is read, on odd cycles it is written
				if (clockCounter % 2 == 0) {
					dmadata = read(dmapage << 8 | dmaaddress);
				}
				else {
					// write to oam pointer and then increment dmaaddress as we are writing to oam memory 1 by 1 
					ppu.oampointer[dmaaddress] = dmadata;
					dmaaddress++;
					// dma is finished when the dma address is 0 as it means we have fully wrapped around the 256 values, so reset the flags 
					if (dmaaddress == 0) {
						dmaActive = false;
						dmaSynced = false;
					}
				}
			}
		}
	}

	if (ppu.nmi == true) {
		ppu.nmi = false;
		cpu.nminterrupt();
	}
	clockCounter++;
}