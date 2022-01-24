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
	bus();
	~bus();
	cpu cpu;
	ppu ppu;
	std::array<uint8_t, 2048> ram;
	std::shared_ptr<cartridge> cart;
	void write(uint16_t address, uint8_t data);
	uint8_t read(uint16_t address, bool readOnly);
	void insertCartridge(const std::shared_ptr<cartridge>& cartridge);
	void reset();
	void clock();
private:
	uint32_t clockCounter = 0;
};

