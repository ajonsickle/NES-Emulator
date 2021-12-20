#pragma once
#include <cstdint>
#include "cpu.h"
#include <array>

class bus
{

public: 
	bus();
public: 
	cpu cpu;
	std::array<uint8_t, 65535> ram;

public:
	void write(uint16_t address, uint8_t data);
	uint8_t read(uint16_t address);

};

