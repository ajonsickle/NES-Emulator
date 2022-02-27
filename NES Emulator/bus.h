#pragma once
#include <cstdint>
#include "cpu.h"
#include <array>
#include "ppu.h"
#include "cartridge.h"
#include <memory>

class bus
{

public:
	// bus constructor
	bus();
	// cpu, ppu, ram and cartridge are all connected to the bus
	cpu cpu;
	ppu ppu;
	std::array<uint8_t, 2048> ram;
	std::shared_ptr<cartridge> cart;
	// controller state 
	uint8_t controller = 0;
	// shift register for controller. controller input is read serially by constantly reloading the shift registers based on the current input
	uint8_t controller_shift = 0;
	// https://wiki.nesdev.org/w/index.php/PPU_OAM#DMA
	// programs copy object attribute (sprite) data into the addressable ram of the cpu and then copy it to the oam frame by frame using the OAMDMA register which is held at $4014. any write to this register 
	// will cause the oam to be initialized by writing the OAMDATA 256 times using successive bytes. the cpu is suspended while this happens 
	uint8_t dmapage = 0;
	uint8_t dmaaddress = 0;
	uint8_t dmadata = 0;
	bool dmaActive = false;
	bool dmaSynced = false;
	// bus must have read and write methods for both the cpu and ppu
	void write(uint16_t address, uint8_t data);
	uint8_t read(uint16_t address);
	// method to add the bus to ownership of the cartridge object
	void insertcartridge(const std::shared_ptr<cartridge>& cartridge);
	//resets everything connected to the bus
	void reset();
	// executes a single clock cycle of the ppu/cpu
	void clock();
	// counter for the number of total clock cycles that have been executed
	uint32_t clockCounter = 0;
};