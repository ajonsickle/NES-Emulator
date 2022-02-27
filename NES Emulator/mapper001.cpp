#include "mapper.h"
#include "mapper001.h"

// https://wiki.nesdev.org/w/index.php?title=MMC1
mapper001::mapper001(uint8_t prgBanks, uint8_t chrBanks) : mapper(prgBanks, chrBanks)
{
	ram.resize(32 * 1024);
}

// & = ref 
bool mapper001::mapperRead(uint16_t address, uint32_t& mapped_address, uint8_t& data)
{
	// if reading between 0x6000 and 0x7FFF the mapper will directly set the data, so the mapped address is set to a very high number so the cartridge knows this has happened
	if (address >= 0x6000 && address <= 0x7FFF)
	{
		mapped_address = 0b1111111111111111;

		data = ram[address & 0x1FFF];

		return true;
	}

	if (address >= 0x8000)
	{
		// second bit of control register contains prg rom bank mode 
		if (controlRegister & 0x08) {
			// https://wiki.nesdev.org/w/index.php/MMC1#Control_.28internal.2C_.248000-.249FFF.29
			// 16 kilobyte mode so depending on the bank we're in, multiply by 16 kilobytes and add the address
			if (address >= 0x8000 && address <= 0xBFFF) {
				mapped_address = prgBank0 * 16384 + (address & 0x3FFF);
				return true;
			}
			if (address >= 0xC000 && address <= 0xFFFF) {
				mapped_address = prgBank1 * 16384 + (address & 0x3FFF);
				return true;
			}
		}
		else {
			// https://wiki.nesdev.org/w/index.php/MMC1#Control_.28internal.2C_.248000-.249FFF.29
			// 32 kilobyte mode so multiply by 32 kilobytes and add the address
			mapped_address = prgROM * 32768 + (address & 0x7FFF);
			return true;
		}
	}



	return false;
}

bool mapper001::mapperWrite(uint16_t address, uint32_t& mapped_address, uint8_t data)
{
	if (address >= 0x6000 && address <= 0x7FFF) {
		// let the cartridge know that the data has been set by the mapper so nothing more has to happen 
		mapped_address = 0b1111111111111111;

		ram[address & 0x1FFF] = data;
		return true;

	}

	if (address >= 0x8000 && address <= 0xFFFF)
	{
		if (data & 0x80) {
			// writing a value with bit 7 set to any address in $8000-$FFFF clears the shift register to its initial state
			loadRegister = 0;
			loadShiftRegister = 0;
			controlRegister = controlRegister | 0x0C;
		}
		else {
			// data is loaded serially into the load registers; on the first four writes, the MMC1 shifts bit 0 into a shift register
			loadRegister >>= 1;
			loadRegister = loadRegister | ((data & 0x01) << 4);
			loadShiftRegister++;

			if (loadShiftRegister == 5) {
				// on the fifth write, the MMC1 copies bit 0 and the shift register contents into an internal register selected by bits 14 and 13 of the address, and then clears the shift register
				uint8_t addr = (address >> 13) & 0x03;

				if (addr == 0) {

					controlRegister = loadRegister & 0x1F;
					// bit 3 of the control register contains information about mirroring 
					if ((controlRegister & 0x03) == 0) {
						mirrorMode = "onescreen_lo";
					}
					else if ((controlRegister & 0x03) == 1) {
						mirrorMode = "onescreen_hi";
					}
					else if ((controlRegister & 0x03) == 2) {
						mirrorMode = "vertical";
					}
					else if ((controlRegister & 0x03) == 3) {
						mirrorMode = "horizontal";
					}
				}
				else if (addr == 1) {
					// set the low bank of the chr rom or the 8kb chr rom
					if (controlRegister & 0x10) {
						chrBank0 = loadRegister & 0x1F;
					}
					else {
						chrROM = loadRegister & 0x1E;
					}
				}
				else if (addr == 2) {
					// set the high bank of the chr rom
					if (controlRegister & 0x10) {
						chrBank1 = loadRegister & 0x1F;
					}
				}
				else if (addr == 3) {
					// bit 2 and 3 of the control register contain program rom mode
					uint8_t prgmode = (controlRegister >> 2) & 0x03;
					if (prgmode == 0 || prgmode == 1) {
						// set 32kb program rom
						prgROM = (loadRegister & 0x0E) >> 1;
					}
					else if (prgmode == 2) {
						// set high 16kb program bank
						prgBank0 = 0;
						prgBank1 = loadRegister & 0x0F;
					}
					else if (prgmode == 3) {
						// set low 16kb program bank 
						prgBank0 = loadRegister & 0x0F;
						prgBank1 = prgsections1 - 1;
					}
				}
				// reset load register 
				loadRegister = 0;
				loadShiftRegister = 0;
			}
		}
	}

	return false;
}


bool mapper001::ppuMapperRead(uint16_t address, uint32_t& mapped_address) {
	if (address < 0x2000) {
		if (chrsections1 == 0) {
			mapped_address = address;
			return true;
		}
		else {
			// first bit of control register holds chr bank mode 
			if (controlRegister & 0x10) {
				// https://wiki.nesdev.org/w/index.php/MMC1#Control_.28internal.2C_.248000-.249FFF.29
				// 4 kilobyte mode, so multiply by 4 kilobytes and add the address
				if (address >= 0 && address <= 0x0FFF) {
					mapped_address = chrBank0 * 4096 + (address & 0x0FFF);
				}
				else if (address >= 0x1000 && address <= 0x1FFF) {
					mapped_address = chrBank1 * 4096 + (address & 0x0FFF);
				}
				return true;
			}
			else {
				// 8 kilobyte mode so multiply by 8 kilobytes and add address
				mapped_address = chrROM * 8192 + (address & 0x1FFF);
				return true;
			}
		}
	}
	return false;
}

bool mapper001::ppuMapperWrite(uint16_t address, uint32_t& mapped_address) {
	if (address < 0x2000) {
		if (chrsections1 == 0) {
			mapped_address = address;
		}
		return true;
	}
	else {
		return false;
	}
}

void mapper001::reset() {
	loadRegister = 0;
	loadShiftRegister = 0;
	controlRegister = 0x1C;
	chrBank0 = 0;
	chrBank1 = 0;
	chrROM = 0;
	prgBank0 = 0;
	prgBank1 = prgsections1 - 1;
	prgROM = 0;
}

std::string mapper001::mirror() {
	return mirrorMode;
}