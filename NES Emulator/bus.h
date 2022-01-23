#pragma once
#include <cstdint>
#include "cpu.h"
#include <array>
#include "ppu.h"
#include "cartridge.h"

class bus
{

public: 
	bus();
	~bus();
public: 
	cpu cpu;
	ppu ppu;
	std::array<uint8_t, 2048> ram;

public:
	void write(uint16_t address, uint8_t data);
	uint8_t read(uint16_t address, bool readOnly);

};

