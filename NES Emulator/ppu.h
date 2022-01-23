#pragma once

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
};

