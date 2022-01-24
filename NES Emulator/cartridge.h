#pragma once
#include <cstdint>
#include <vector>
#include <string>
class cartridge
{
	cartridge(const std::string& romfile);
	~cartridge();

public:
	bool read(uint16_t address, uint8_t &data);
	bool write(uint16_t address, uint8_t data);

	bool ppuRead(uint16_t address, uint8_t &data);
	bool ppuWrite(uint16_t address, uint8_t data);

	//nes rom files contained header (16 bytes), trainer (0 or 512 bytes), prg rom data (16384 * x byes), chr rom data (8192 * y byes), 
	// playchoice inst-rom (0 or 8192 * y bytes), playchoice PROM (16 bytes data, 16 bytes CounterOut)
	std::vector<uint8_t> prgdata;
	std::vector<uint8_t> chrdata;

	uint8_t mapper = 0;
	uint8_t prgsections = 0;
	uint8_t chrsections = 0;
};

