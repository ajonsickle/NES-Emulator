#pragma once
#include "mapper.h"
//https://wiki.nesdev.org/w/index.php?title=NROM
class mapper000 : public mapper
{
public:
	mapper000(uint8_t prgsections, uint8_t chrsections);
	~mapper000();

	bool mapperRead(uint16_t address, uint32_t& mapped_address) override;
	bool mapperWrite(uint16_t address, uint32_t& mapped_address) override;
	bool ppuMapperRead(uint16_t address, uint32_t& mapped_address) override;
	bool ppuMapperWrite(uint16_t address, uint32_t& mapped_address) override;
};

