#pragma once
#include <memory>
#include "cartridge.h"
#include <cstdint>

class bus;

class ppu
{
	
public:
	ppu();
	~ppu();
	uint8_t read(uint16_t address, bool readOnly);
	void write(uint16_t address, uint8_t data);

	uint8_t ppuread(uint16_t address, bool readOnly);
	void ppuwrite(uint16_t address, uint8_t data);

	void connectCartridge(const std::shared_ptr<cartridge>& cartridge);
	void clock();

	uint8_t nametable[2][1024];
	uint8_t palettes[64];
	uint8_t patterntable[2][256 * 16];
	uint8_t attributetable[64];
	uint8_t oam[64 * 4];
	uint8_t memorymap[16384];

private:
	std::shared_ptr<cartridge> cart;
};

