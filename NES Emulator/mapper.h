#include <cstdint>
#pragma once

class mapper
{
	//https://wiki.nesdev.org/w/index.php?title=Mapper
public:
	mapper(uint8_t prgsections, uint8_t chrsections);
	~mapper();

	//takes input address from cpu/ppu and tries to map it into an address in the prg/chr rom, returns true if valid
	// pure virtual function
	virtual bool mapperRead(uint16_t address, uint32_t& mapped_address) = 0;
	virtual bool mapperWrite(uint16_t address, uint32_t& mapped_address) = 0;
	virtual bool ppuMapperRead(uint16_t address, uint32_t& mapped_address) = 0;
	virtual bool ppuMapperWrite(uint16_t address, uint32_t& mapped_address) = 0;

protected:
	uint8_t prgsections1 = 0;
	uint8_t chrsections1 = 0;
};

