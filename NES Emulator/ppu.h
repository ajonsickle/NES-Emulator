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

	uint8_t nametable[2][1024];
	uint8_t palettes[64];
	uint8_t patterntable[2][256 * 16];

	olc::Sprite& GetScreen();
	olc::Sprite& GetNameTable(uint8_t i);
	olc::Sprite& GetPatternTable(uint8_t i);
	bool frame_complete = false;

private:
	std::shared_ptr<cartridge> cart;
	olc::Pixel  palScreen[0x40];
	olc::Sprite sprScreen = olc::Sprite(256, 240);
	olc::Sprite sprNameTable[2] = { olc::Sprite(256, 240), olc::Sprite(256, 240) };
	olc::Sprite sprPatternTable[2] = { olc::Sprite(128, 128), olc::Sprite(128, 128) };
	int16_t scanline = 0;
	int16_t cycle = 0;
};

