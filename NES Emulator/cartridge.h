#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include "mapper000.h"
#include <cstdint>
#include <fstream>
#include <vector>
#include "mapper.h"
#include "mapper001.h"

class cartridge
{
public:
	// constructor
	cartridge(const std::string& romfile);
	void reset();
	// as the cartridge has priority over all the read/write functions of the cpu and ppu there must be a function to let the other components know that it is handling the read/write	
	bool read(uint16_t address, uint8_t& data);
	bool write(uint16_t address, uint8_t data);
	bool ppuread(uint16_t address, uint8_t& data);
	bool ppuwrite(uint16_t address, uint8_t data);

	/* ppu had nametables, which were a 1KB area of memory used for background layout. https://wiki.nesdev.org/w/index.php?title=PPU_nametables
	the NES had four logical nametables arranged in a 2x2 pattern, starting at $2000 in the top left and ending at $2C00 in the bottom right. As the NES only had 2KB of VRAM mirroring
	also has to be used for the nametables. In vertical mirroring $2000 = $2800 while $2400 = $2C00. In horizontal mirroring $2000 = $2400 while $2800 = $2C00. In one-screen mirroring,
	all remaining 3 nametables are a mirror of the first. There were two types of one-screen mirroring, low, whereby all 4 nametables showed the first 1KB bank of CIRAM, and upper,
	where all four nametables showed the second 1KB bank of CIRAM. The mode is set to horizontal by default.
	*/
	std::string mirrorMode = "horizontal";
	// nes rom files contained header (16 bytes), trainer (0 or 512 bytes), prg rom data (16384 * x bytes), chr rom data (8192 * y bytes), 
	// playchoice inst-rom (0 or 8192 * y bytes), playchoice PROM (16 bytes data, 16 bytes CounterOut)
	uint8_t mapperid = 0;
	uint8_t prgsections = 0;
	uint8_t chrsections = 0;
	std::vector<uint8_t> prgdata;
	std::vector<uint8_t> chrdata;
	// pointer to whatever mapper is required for this rom
	std::shared_ptr<mapper> mapperptr;
	// get mirror mode 
	std::string getMirror(); 
};