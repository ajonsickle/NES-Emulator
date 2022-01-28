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
	// bus constructor/destructor
	bus();
	~bus();
	// cpu, ppu, ram and cartridge are all connected to the bus
	cpu cpu;
	ppu ppu;
	std::array<uint8_t, 2048> ram;
	std::shared_ptr<cartridge> cart;
	// bus must have read and write methods for both the cpu and ppu
	void write(uint16_t address, uint8_t data);
	uint8_t read(uint16_t address, bool readOnly);
	// method to add the bus to ownership of the cartridge object
	void insertCartridge(const std::shared_ptr<cartridge>& cartridge);
	//resets cpu
	void reset();
	// executes a single clock cycle of the ppu/cpu
	void clock();
	// counter for the number of total clock cycles that have been executed
	uint32_t clockCounter = 0;
};

