#pragma once
class cartridge
{
	cartridge();
	~cartridge();

public:
	bool read(uint16_t address, uint8_t &data);
	bool write(uint16_t address, uint8_t data);

	bool ppuRead(uint16_t address, uint8_t &data);
	bool ppuWrite(uint16_t address, uint8_t data)
};

