#pragma once
#include <memory>
#include "cartridge.h"
#include <cstdint>

#include "olcPixelGameEngine.h"

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

	// ppu contains 8 memory-mapped registers which sit at $2000 to $2007 in the CPU's address space, 2 256-tile pattern tables (which define the shape of the tiles that make up backgrounds and sprites), 
	// 2 physical 1024 byte nametables (4 logical ones after mirroring), a 64-byte attribute table, OAM memory (contains a display list of up to 64 sprites where each sprite occupies 4 bytes), 64 palette outputs
	// and a memory map, which addresses a 16kB space from $0000 to $3FFF. 
	uint8_t nametable[2][1024];
	uint8_t palettes[64];
	uint8_t patterntable[2][256 * 16];
	// shares ownership of the cartridge object with bus 
	std::shared_ptr<cartridge> cart;

	// for the game engine
	olc::Sprite& GetScreen();
	olc::Sprite& GetNameTable(uint8_t i);
	olc::Sprite& GetPatternTable(uint8_t i);
	bool frame_complete = false;
	olc::Pixel  palScreen[0x40];
	olc::Sprite sprScreen = olc::Sprite(256, 240);
	olc::Sprite sprNameTable[2] = { olc::Sprite(256, 240), olc::Sprite(256, 240) };
	olc::Sprite sprPatternTable[2] = { olc::Sprite(128, 128), olc::Sprite(128, 128) };
	int16_t scanline = 0;
	int16_t cycle = 0;
};

