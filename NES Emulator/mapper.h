#include <cstdint>	
#include <array>
#include <string>
#pragma once


class mapper
{
	//https://wiki.nesdev.org/w/index.php?title=mapper
	// mappers basically translate physical memory addresses into the CPU and PPU's address spaces 
public:

	// generic constructor
	mapper(uint8_t prgsections, uint8_t chrsections);

	//takes input address from cpu/ppu and tries to map it into an address in the prg/chr rom, returns true if valid
	// pure virtual function (it is designed to be implemented by children classes and therefore just has to be declared)
	// mapped address is passed as a ref so that it can be used by the cartridge. data may also be changed by the mapper when reading so it is passed as a ref 
	virtual bool mapperRead(uint16_t address, uint32_t &mapped_address, uint8_t &data) = 0;
	virtual bool mapperWrite(uint16_t address, uint32_t &mapped_address, uint8_t data = 0) = 0;
	virtual bool ppuMapperRead(uint16_t address, uint32_t &mapped_address) = 0;
	virtual bool ppuMapperWrite(uint16_t address, uint32_t &mapped_address) = 0;
	virtual void reset() = 0;
	// get mirror mode
	virtual std::string mirror();
	// number of prg and chr banks 
	uint8_t prgsections1 = 0;
	uint8_t chrsections1 = 0;
};
