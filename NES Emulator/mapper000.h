#pragma once
#include "mapper.h"
//https://wiki.nesdev.org/w/index.php?title=NROM
// the NES had 255 mappers, this is the 1st 
class mapper000 : public mapper
{
public:
	mapper000(uint8_t prgsections, uint8_t chrsections);
	// implementations of abstract parent functions
	bool mapperRead(uint16_t address, uint32_t& mapped_address, uint8_t &data) override;
	bool mapperWrite(uint16_t address, uint32_t& mapped_address, uint8_t data = 0) override;
	bool ppuMapperRead(uint16_t address, uint32_t& mapped_address) override;
	bool ppuMapperWrite(uint16_t address, uint32_t& mapped_address) override;
	void reset() override;
};