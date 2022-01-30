#include "ppu.h"
#include <cstdint>

ppu::ppu() {
	// array of every colour the nes is capable of displaying
	palettes[0x00] = olc::Pixel(84, 84, 84);
	palettes[0x01] = olc::Pixel(0, 30, 116);
	palettes[0x02] = olc::Pixel(8, 16, 144);
	palettes[0x03] = olc::Pixel(48, 0, 136);
	palettes[0x04] = olc::Pixel(68, 0, 100);
	palettes[0x05] = olc::Pixel(92, 0, 48);
	palettes[0x06] = olc::Pixel(84, 4, 0);
	palettes[0x07] = olc::Pixel(60, 24, 0);
	palettes[0x08] = olc::Pixel(32, 42, 0);
	palettes[0x09] = olc::Pixel(8, 58, 0);
	palettes[0x0A] = olc::Pixel(0, 64, 0);
	palettes[0x0B] = olc::Pixel(0, 60, 0);
	palettes[0x0C] = olc::Pixel(0, 50, 60);
	palettes[0x0D] = olc::Pixel(0, 0, 0);
	palettes[0x0E] = olc::Pixel(0, 0, 0);
	palettes[0x0F] = olc::Pixel(0, 0, 0);

	palettes[0x10] = olc::Pixel(152, 150, 152);
	palettes[0x11] = olc::Pixel(8, 76, 196);
	palettes[0x12] = olc::Pixel(48, 50, 236);
	palettes[0x13] = olc::Pixel(92, 30, 228);
	palettes[0x14] = olc::Pixel(136, 20, 176);
	palettes[0x15] = olc::Pixel(160, 20, 100);
	palettes[0x16] = olc::Pixel(152, 34, 32);
	palettes[0x17] = olc::Pixel(120, 60, 0);
	palettes[0x18] = olc::Pixel(84, 90, 0);
	palettes[0x19] = olc::Pixel(40, 114, 0);
	palettes[0x1A] = olc::Pixel(8, 124, 0);
	palettes[0x1B] = olc::Pixel(0, 118, 40);
	palettes[0x1C] = olc::Pixel(0, 102, 120);
	palettes[0x1D] = olc::Pixel(0, 0, 0);
	palettes[0x1E] = olc::Pixel(0, 0, 0);
	palettes[0x1F] = olc::Pixel(0, 0, 0);

	palettes[0x20] = olc::Pixel(236, 238, 236);
	palettes[0x21] = olc::Pixel(76, 154, 236);
	palettes[0x22] = olc::Pixel(120, 124, 236);
	palettes[0x23] = olc::Pixel(176, 98, 236);
	palettes[0x24] = olc::Pixel(228, 84, 236);
	palettes[0x25] = olc::Pixel(236, 88, 180);
	palettes[0x26] = olc::Pixel(236, 106, 100);
	palettes[0x27] = olc::Pixel(212, 136, 32);
	palettes[0x28] = olc::Pixel(160, 170, 0);
	palettes[0x29] = olc::Pixel(116, 196, 0);
	palettes[0x2A] = olc::Pixel(76, 208, 32);
	palettes[0x2B] = olc::Pixel(56, 204, 108);
	palettes[0x2C] = olc::Pixel(56, 180, 204);
	palettes[0x2D] = olc::Pixel(60, 60, 60);
	palettes[0x2E] = olc::Pixel(0, 0, 0);
	palettes[0x2F] = olc::Pixel(0, 0, 0);

	palettes[0x30] = olc::Pixel(236, 238, 236);
	palettes[0x31] = olc::Pixel(168, 204, 236);
	palettes[0x32] = olc::Pixel(188, 188, 236);
	palettes[0x33] = olc::Pixel(212, 178, 236);
	palettes[0x34] = olc::Pixel(236, 174, 236);
	palettes[0x35] = olc::Pixel(236, 174, 212);
	palettes[0x36] = olc::Pixel(236, 180, 176);
	palettes[0x37] = olc::Pixel(228, 196, 144);
	palettes[0x38] = olc::Pixel(204, 210, 120);
	palettes[0x39] = olc::Pixel(180, 222, 120);
	palettes[0x3A] = olc::Pixel(168, 226, 144);
	palettes[0x3B] = olc::Pixel(152, 226, 180);
	palettes[0x3C] = olc::Pixel(160, 214, 228);
	palettes[0x3D] = olc::Pixel(160, 162, 160);
	palettes[0x3E] = olc::Pixel(0, 0, 0);
	palettes[0x3F] = olc::Pixel(0, 0, 0);
}
ppu::~ppu() {

}
olc::Sprite& ppu::GetScreen()
{
	return sprScreen;
}

olc::Sprite& ppu::GetNameTable(uint8_t i)
{
	return sprNameTable[i];
}

olc::Pixel& ppu::GetColourFromPaletteRam(uint8_t palette, uint8_t pixel) {
	return palettes[ppuread((0x3F00 + (palette * 4) + pixel) & 0x3F, false)];
}

olc::Sprite& ppu::GetPatternTable(uint8_t i, uint8_t palette)
{
	// pattern table is 16x16
	for (uint16_t x = 0; x < 16; x++) {
		for (uint16_t y = 0; y < 16; y++) {
			// a single tile is 16 bytes and there are 16 of them in a single row 
			uint16_t offset = y * 256 + x * 16;
			// single tile is 8x8
			for (uint16_t row = 0; row < 8; row++) {
				// the nes pattern tables used 2 bits per pixel, so 4 colours were available. rather than storing each colour code on 
				// one single tile, they were split up into least significant bit and most significant bit, so they had to be combined to get a whole pixel
				uint8_t tile_lsb = ppuread(i * 0x1000 + offset + row, false); 
				uint8_t tile_msb = ppuread(i * 0x1000 + offset + row + 8, false);
				for (uint16_t column = 0; column < 8; column++) {
					// will only preserve the least significant bit, so a value between 0 and 3 will be returned
					uint8_t pixel = (tile_lsb & 0x01) + (tile_msb & 0x01);
					tile_lsb = tile_lsb >> 1;
					tile_msb = tile_msb >> 1;

					sprPatternTable[i].SetPixel(x * 8 + (7 - column), y * 8 + row, GetColourFromPaletteRam(palette, pixel));
				}
			}
		}
	}
	return sprPatternTable[i];
}

uint8_t ppu::read(uint16_t address, bool readOnly) {
	uint8_t data = 0x00;

	switch (address) {
	case 0x0000:

		break;
	case 0x0001:

		break;
	case 0x0002:
		status.vblank = 1;
		data = (status.reg & 0xE0) | (ppu_data_buffer & 0x1F);
		status.vblank = 0;
		address_latch = 0;
		break;
	case 0x0003:

		break;
	case 0x0004:

		break;
	case 0x0005:

		break;
	case 0x0006:
		if (address_latch == 0) {
			ppu_address = (ppu_address & 0x00FF) | (data << 8);
			address_latch = 1;
		}
		else {
			ppu_address = (ppu_address & 0xFF00) | data;
			address_latch = 0;
		}
		break;
	case 0x0007:
		data = ppu_data_buffer;
		ppu_data_buffer = ppuread(ppu_address, false);

		if (ppu_address >= 0x3f00) data = ppu_data_buffer;
		ppu_address++;
		break;
	
	}

	return data;
}

void ppu::write(uint16_t address, uint8_t data) {
	switch (address) {
	case 0x0000:
		ctrl.reg = data;
		break;
	case 0x0001:
		mask.reg = data;
		break;
	case 0x0002:

		break;
	case 0x0003:

		break;
	case 0x0004:

		break;
	case 0x0005:

		break;
	case 0x0006:
		if (address_latch == 0) {
			ppu_address = (ppu_address & 0xFF00) | data;
			address_latch = 1;
		}
		else {
			ppu_address = (ppu_address & 0xFF00) | data;
			address_latch = 0;
		}
		break;
	case 0x0007:
		ppuwrite(ppu_address, data);
		ppu_address++;
		break;

	}
}

uint8_t ppu::ppuread(uint16_t address, bool readOnly) {
	uint8_t data = 0x00;
	address = address & 0x3FFF;
	// cartridge has priority
	if (cart->ppuRead(address, data)) {

	}
	// pattern memory
	else if (address >= 0x0000 && address <= 0x1FFF) {
		// chooses whether its the left or right hand side of the pattern table. the most significant bit of the 
		// ppu address is taken, and the remaining bits are taken to find the offset into that table 
		data = pattern[(address & 0x1000) >> 12][address & 0x0FFF];
	}
	// nametable memory
	else if (address >= 0x2000 && address <= 0x3EFF) {

	}
	// palette memory
	else if (address >= 0x3F00 && address <= 0x3FFF) {
		// mask first 5 bits 
		address = address & 0x001F;
		// mirroring
		if (address == 0x0010) address = 0x0000;
		if (address == 0x0014) address = 0x0004;
		if (address == 0x0018) address = 0x0008;
		if (address == 0x001C) address = 0x000C;
		data = palette[address];
	}
	return data;
}

void ppu::ppuwrite(uint16_t address, uint8_t data) {
	address = address & 0x3FFF;
	// cartridge has priority
	// cartridge has priority
	if (cart->ppuWrite(address, data)) {

	}
	// pattern memory
	else if (address >= 0x0000 && address <= 0x1FFF) {
		// chooses whether its the left or right hand side of the pattern table. the most significant bit of the 
		// ppu address is taken, and the remaining bits are taken to find the offset into that table 
		pattern[(address & 0x1000) >> 12][address & 0x0FFF] = data;
	}
	// nametable memory
	else if (address >= 0x2000 && address <= 0x3EFF) {

	}
	// palette memory
	else if (address >= 0x3F00 && address <= 0x3FFF) {
		address = address & 0x001F;
		// mirroring
		if (address == 0x0010) address = 0x0000;
		if (address == 0x0014) address = 0x0004;
		if (address == 0x0018) address = 0x0008;
		if (address == 0x001C) address = 0x000C;
		palette[address] = data;
	}
}

void ppu::connectCartridge(const std::shared_ptr<cartridge>& cartridge) {
	this->cart = cartridge;
}

void ppu::clock() {

	sprScreen.SetPixel(cycle - 1, scanline, palettes[(rand() % 2) ? 0x3F : 0x30]);
	cycle++;
	if (cycle >= 341) {
		cycle = 0;
		scanline++;
		if (scanline >= 261) {
			scanline = -1;
			frame_complete = true;
		}
	}
}