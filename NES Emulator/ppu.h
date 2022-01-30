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

	// nametable, pattern and palette memory
	uint8_t nametable[2][1024];
	uint8_t pattern[2][256 * 16];
	uint8_t palette[32];
	// shares ownership of the cartridge object with bus 
	std::shared_ptr<cartridge> cart;
	// ppu contains 8 memory-mapped registers which sit at $2000 to $2007 in the CPU's address space, 2 256-tile pattern tables (which define the shape of the tiles that make up backgrounds and sprites), 
	// 2 physical 1024 byte nametables (4 logical ones after mirroring), a 64-byte attribute table, OAM memory (contains a display list of up to 64 sprites where each sprite occupies 4 bytes), 64 palette outputs
	// and a memory map, which addresses a 16kB space from $0000 to $3FFF. 
	// chr/pattern rom is for sprites, nametable memory is for layout and palette memory is for colours
	olc::Pixel  palettes[0x40];
	olc::Sprite sprScreen = olc::Sprite(256, 240);
	olc::Sprite sprNameTable[2] = { olc::Sprite(256, 240), olc::Sprite(256, 240) };
	olc::Sprite sprPatternTable[2] = { olc::Sprite(128, 128), olc::Sprite(128, 128) };
	olc::Sprite& GetScreen();
	olc::Sprite& GetNameTable(uint8_t i);
	olc::Sprite& GetPatternTable(uint8_t i, uint8_t palette); // there are two 256-tile sections
	olc::Pixel& GetColourFromPaletteRam(uint8_t palette, uint8_t pixel);
	bool frame_complete = false;
	int16_t scanline = 0;
	int16_t cycle = 0;

	// ppu control register
	union PPUCTRL {
		struct {
			uint8_t nmiEnable : 1;
			uint8_t ppuSlave : 1;
			uint8_t spriteHeight : 1;
			uint8_t backgroundTileSelect : 1;
			uint8_t spriteTileSelect : 1;
			uint8_t incrementMode : 1;
			uint8_t nametableSelect : 2;
		};
		uint8_t reg;
	} ctrl;
	// ppu mask register
	union PPUMASK {
		struct {
			uint8_t blueEmphasis : 1;
			uint8_t greenEmphasis : 1;
			uint8_t redEmphasis : 1;
			uint8_t spriteEnable : 1;
			uint8_t backgroundEnable : 1;
			uint8_t spriteLeftColumnEnable : 1;
			uint8_t backgroundLeftColumnEnable : 1;
			uint8_t greyscale : 1;
		};
		uint8_t reg;
	} mask;
	// ppu status register
	union PPUSTATUS {
		struct {
			uint8_t vblank : 1;
			uint8_t sprite0 : 1;
			uint8_t spriteoverflow : 1;
			uint8_t unused : 5;
		};

		uint8_t reg;
	} status;

	uint8_t address_latch = 0x00;
	uint8_t ppu_data_buffer = 0x00;
	uint16_t ppu_address = 0x0000;
};

