#pragma once
#include "mapper.h"
#include <vector>
//https://wiki.nesdev.org/w/index.php/MMC1#Registers
// mapper 001
class mapper001 : public mapper
{
public:
	mapper001(uint8_t prgsections, uint8_t chrsections);
	// implementations of abstract parent functions
	bool mapperRead(uint16_t address, uint32_t &mapped_address, uint8_t &data) override;
	bool mapperWrite(uint16_t address, uint32_t &mapped_address, uint8_t data = 0) override;
	bool ppuMapperRead(uint16_t address, uint32_t &mapped_address) override;
	bool ppuMapperWrite(uint16_t address, uint32_t &mapped_address) override;
	void reset() override;
	std::string mirror();

	// mapper 001 contained 4 registers: the load register, from $8000-$FFFF, the control register from $8000-$9FFF, the CHR bank 0, from $A000-$BFFF, the CHR bank 1, from $C000-$DFFF, 
	// and the PRG bank from $E000-$FFFF

	uint8_t loadRegister = 0;
	uint8_t loadShiftRegister = 0;
	uint8_t controlRegister = 0;
	uint8_t chrBank0 = 0;
	uint8_t chrBank1 = 0;
	uint8_t chrROM = 0; 
	uint8_t prgBank0 = 0;
	uint8_t prgBank1 = 0; 
	uint8_t prgROM = 0;
	std::string mirrorMode = "horizontal";
	std::vector<uint8_t> ram; 

};