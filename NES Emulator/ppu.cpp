

#include "ppu.h"

ppu::ppu()
{
	// array of every pixel colour the NES is capable of displaying - https://wiki.nesdev.org/w/index.php?title=PPU_palettes#Palettes
	colours[0] = olc::Pixel(84, 84, 84);
	colours[0x01] = olc::Pixel(0, 30, 116);
	colours[0x02] = olc::Pixel(8, 16, 144);
	colours[0x03] = olc::Pixel(48, 0, 136);
	colours[0x04] = olc::Pixel(68, 0, 100);
	colours[0x05] = olc::Pixel(92, 0, 48);
	colours[0x06] = olc::Pixel(84, 4, 0);
	colours[0x07] = olc::Pixel(60, 24, 0);
	colours[0x08] = olc::Pixel(32, 42, 0);
	colours[0x09] = olc::Pixel(8, 58, 0);
	colours[0x0A] = olc::Pixel(0, 64, 0);
	colours[0x0B] = olc::Pixel(0, 60, 0);
	colours[0x0C] = olc::Pixel(0, 50, 60);
	colours[0x0D] = olc::Pixel(0, 0, 0);
	colours[0x0E] = olc::Pixel(0, 0, 0);
	colours[0x0F] = olc::Pixel(0, 0, 0);
	colours[0x10] = olc::Pixel(152, 150, 152);
	colours[0x11] = olc::Pixel(8, 76, 196);
	colours[0x12] = olc::Pixel(48, 50, 236);
	colours[0x13] = olc::Pixel(92, 30, 228);
	colours[0x14] = olc::Pixel(136, 20, 176);
	colours[0x15] = olc::Pixel(160, 20, 100);
	colours[0x16] = olc::Pixel(152, 34, 32);
	colours[0x17] = olc::Pixel(120, 60, 0);
	colours[0x18] = olc::Pixel(84, 90, 0);
	colours[0x19] = olc::Pixel(40, 114, 0);
	colours[0x1A] = olc::Pixel(8, 124, 0);
	colours[0x1B] = olc::Pixel(0, 118, 40);
	colours[0x1C] = olc::Pixel(0, 102, 120);
	colours[0x1D] = olc::Pixel(0, 0, 0);
	colours[0x1E] = olc::Pixel(0, 0, 0);
	colours[0x1F] = olc::Pixel(0, 0, 0);
	colours[0x20] = olc::Pixel(236, 238, 236);
	colours[0x21] = olc::Pixel(76, 154, 236);
	colours[0x22] = olc::Pixel(120, 124, 236);
	colours[0x23] = olc::Pixel(176, 98, 236);
	colours[0x24] = olc::Pixel(228, 84, 236);
	colours[0x25] = olc::Pixel(236, 88, 180);
	colours[0x26] = olc::Pixel(236, 106, 100);
	colours[0x27] = olc::Pixel(212, 136, 32);
	colours[0x28] = olc::Pixel(160, 170, 0);
	colours[0x29] = olc::Pixel(116, 196, 0);
	colours[0x2A] = olc::Pixel(76, 208, 32);
	colours[0x2B] = olc::Pixel(56, 204, 108);
	colours[0x2C] = olc::Pixel(56, 180, 204);
	colours[0x2D] = olc::Pixel(60, 60, 60);
	colours[0x2E] = olc::Pixel(0, 0, 0);
	colours[0x2F] = olc::Pixel(0, 0, 0);
	colours[0x30] = olc::Pixel(236, 238, 236);
	colours[0x31] = olc::Pixel(168, 204, 236);
	colours[0x32] = olc::Pixel(188, 188, 236);
	colours[0x33] = olc::Pixel(212, 178, 236);
	colours[0x34] = olc::Pixel(236, 174, 236);
	colours[0x35] = olc::Pixel(236, 174, 212);
	colours[0x36] = olc::Pixel(236, 180, 176);
	colours[0x37] = olc::Pixel(228, 196, 144);
	colours[0x38] = olc::Pixel(204, 210, 120);
	colours[0x39] = olc::Pixel(180, 222, 120);
	colours[0x3A] = olc::Pixel(168, 226, 144);
	colours[0x3B] = olc::Pixel(152, 226, 180);
	colours[0x3C] = olc::Pixel(160, 214, 228);
	colours[0x3D] = olc::Pixel(160, 162, 160);
	colours[0x3E] = olc::Pixel(0, 0, 0);
	colours[0x3F] = olc::Pixel(0, 0, 0);

}

// returns reference to the sprite holding the background layout (physical representation of the nametable)
olc::Sprite& ppu::getBGLayout()
{
	return backgroundLayout;
}



olc::Sprite& ppu::getPatternTables(uint8_t i, uint8_t palette)
{

	// one pattern table was 16x16 tiles 
	for (uint16_t y = 0; y < 16; y++)
	{
		for (uint16_t x = 0; x < 16; x++)
		{
			// as the pattern table is modelled as a 1D array the current coordinates must be converted into an offset. each tile is 16 bytes, and because each row has 
			// 16 tiles, the y coordinate is multiplied by 256 (16x16) and the x coordinate is multiplied by 16.  
			uint16_t nOffset = y * 256 + x * 16;

			// each tile has 8x8 pixels, loop through these
			for (uint16_t row = 0; row < 8; row++)
			{
				// each pixel is stored in a least significant bit plane and a most significant bit plane, so they can be combined together to form a 2 bit pixel value 
				// multiply the pattern table number (0 or 1) by 4096, then index into the offset calculated earlier, add the current row, and 8 if it is the most significant
				// bit, as for any given pixel, the planes are always 8 bytes apart. we therefore get one 8 bit row of pixels from each plane
				uint8_t tile_lsb = ppuread(i * 256 * 16 + nOffset + row);
				uint8_t tile_msb = ppuread(i * 256 * 16 + nOffset + row + 8);

				// now iterate through columns
				for (uint16_t column = 0; column < 8; column++)
				{
					// for each pixel in the given row, we will mask the least significant and most significant rows with 0x01 to only return the least significant bit, 
					// or together the lsb and msb to get a pixel value, then shift both rows right so that on the next column iteration the least significant bit 
					// will be the next bit to check.

					uint8_t pixel = ((tile_lsb & 0x01) << 1) | (tile_msb & 0x01);
					tile_lsb = tile_lsb >> 1;
					tile_msb = tile_msb >> 1;

					// update the physical representation of the pattern table by setting each individual pixel. 
					// as the least significant bit is being used first we are essentially reading from right to left, so it must be read backwards 
					patternTables[i].SetPixel(x * 8 + (7 - column), y * 8 + row, GetPixel(palette, pixel));
				}
			}
		}
	}

	// return the updated pattern table sprite 
	return patternTables[i];
}


olc::Pixel& ppu::GetPixel(uint8_t palette, uint8_t pixel)
{
	// palette addresses start at $3F00. palettes start at $3F01, $3F05, $3F09, $3F0D, $3F11, $3F15, $3F19, $3F1D, so multiply the palette number by 4 and add the 
	// desired pixel number. e.g. for pixel 2 on palette 2, 2 * 4 = 8, 8 + 2 = 10, therefore read into memory address $3F0A. 
	try {
		auto address = 0x3F00 + (palette * 4) + pixel;
		return colours[ppuread(address)];
	}
	catch (const std::exception& e) {
		return colours[ppuread(0x3F00)];
	}

}

// need a method to read to the PPU registers as these are contained in the CPU's ram
uint8_t ppu::read(uint16_t address)
{
	uint8_t data = 0;

		// only status reg, oam data reg and ppu data reg are readable under normal conditions
		switch (address)
		{
		case 0x2002:
			// https://wiki.nesdev.org/w/index.php/PPU_registers#Status_.28.242002.29_.3C_read
			// reading the status register clears bit 7 (vertical blank period) as well as the W register. 
			// the actual status register contains 5 unused bits so we need to mask it with 11100000 to only get the 3 useful bits. 
			data = (status.reg & 0xE0) | (ppu_data_buffer & 0x1F);
			status.vertical_blank = 0;
			wreg = 0;
			break;

		case 0x2004:
			// https://wiki.nesdev.org/w/index.php/PPU_registers#OAM_data_.28.242004.29_.3C.3E_read.2Fwrite
			data = oampointer[OAMADDR];
			break;
		case 0x2007:
			// https://wiki.nesdev.org/w/index.php/PPU_registers#Data_.28.242007.29_.3C.3E_read.2Fwrite
			// reading ppudata register 
			// if the address is between $0 and $3EFF (before the palettes), we return the contents of the data buffer which holds the result of the last read
			// however if we are at address $3F00 to $3FFF, we are reading from the palettes, in which case the data is immediately returned 
			data = ppu_data_buffer;
			// update the buffer
			ppu_data_buffer = ppuread(vreg.reg);
			if (vreg.reg >= 0x3F00) data = ppu_data_buffer;
			// after reading or writing to the ppudata register the v register is incremented by an amount determined by bit 2 of $2000, meaning the increment_mode flag on
			// the ppuctrl register. if it is set to 1, meaning vertical, then the v register is incremented by 32, or an entire row. otherwise it is just incremented by 1 
			// as it is in horizontal mode 
			if (control.increment_mode) {
				vreg.reg += 32;
			}
			else {
				vreg.reg++;
			}
			break;
		}

	return data;
}
// write to the ppu registers
void ppu::write(uint16_t address, uint8_t data)
{
	switch (address)
	{
	case 0x2000:
		// https://wiki.nesdev.org/w/index.php/PPU_registers#Controller_.28.242000.29_.3E_write
		// set T reg accordingly
		control.reg = data;
		treg.nametable_x = control.nametable_x;
		treg.nametable_y = control.nametable_y;
		break;
	case 0x2001:
		// https://wiki.nesdev.org/w/index.php/PPU_registers#Mask_.28.242001.29_.3E_write
		mask.reg = data;
		break;
	case 0x2003:
		OAMADDR = data;
		break;
	case 0x2004:
		oampointer[OAMADDR] = data;
		break;
	case 0x2005:
		// https://wiki.nesdev.org/w/index.php/PPU_registers#Scroll_.28.242005.29_.3E.3E_write_x2
		if (wreg == 0)
		{
			// write to scroll register, split into fine x and coarse x
			xreg = data & 0x07;
			treg.coarse_x = data >> 3;
			wreg = 1;
		}
		else
		{
			// split into fine y and coarse y 
			treg.fine_y = data & 0x07;
			treg.coarse_y = data >> 3;
			wreg = 0;
		}
		break;
	case 0x2006:
		// https://wiki.nesdev.org/w/index.php/PPU_registers#PPUADDR
		// ppuaddr writes happen in two steps, the first one taking the high byte and wreg 0 and the second taking the low byte when wreg is 1. 
		if (wreg == 0)
		{

			treg.reg = (uint16_t)((data & 0x3F) << 8) | (treg.reg & 0x00FF);
			wreg = 1;
		}
		else
		{
			// the vreg is updated after an entire write has been executed
			treg.reg = (treg.reg & 0xFF00) | data;
			vreg = treg;
			wreg = 0;
		}
		break;
	case 0x2007:
		ppuwrite(vreg.reg, data);
		// has the same behaviour as the cpuread function
		if (control.increment_mode) {
			vreg.reg += 32;
		}
		else {
			vreg.reg++;
		}
		break;
	}
}

// read from the PPU's VRAM
uint8_t ppu::ppuread(uint16_t address)
{
	// not sure why you have to do this but a test rom said i should 
	address &= 0x3FFF;
	uint8_t data = 0;

	if (!cart->ppuread(address, data))
	{

		if (address >= 0 && address <= 0x1FFF)
		{
			// mask address with 0x1000 and shift right by 12 to either get 0 or 1 (first or second pattern table)
			// pattern table 1 is from $0000 to $0FFF, so mask the address with 0x0FFF no matter what because both pattern tables have a fixed size
			data = patterntable[(address & 0x1000) >> 12][address & 0x0FFF];
		}
		else if (address >= 0x2000 && address <= 0x3EFF)
		{
			// mask with 0x0FFF so it can be used as an index into the nametable
			address = address & 0x0FFF;
			// https://wiki.nesdev.org/w/index.php?title=Mirroring#Nametable_Mirroring
			if (cart->getMirror() == "vertical")
			{
				// vertical mirroring - $2000 = $2800 and $2400 = $2C00
				if (address >= 0 && address <= 0x03FF)
					data = nametable[0][address & 0x03FF];
				if (address >= 0x0400 && address <= 0x07FF)
					data = nametable[1][address & 0x03FF];
				if (address >= 0x0800 && address <= 0x0BFF)
					data = nametable[0][address & 0x03FF];
				if (address >= 0x0C00 && address <= 0x0FFF)
					data = nametable[1][address & 0x03FF];
			}
			else if (cart->getMirror() == "horizontal")
			{
				// horizontal mirroring - $2000 = $2400 and $2800 = $2C00
				if (address >= 0 && address <= 0x03FF)
					data = nametable[0][address & 0x03FF];
				if (address >= 0x0400 && address <= 0x07FF)
					data = nametable[0][address & 0x03FF];
				if (address >= 0x0800 && address <= 0x0BFF)
					data = nametable[1][address & 0x03FF];
				if (address >= 0x0C00 && address <= 0x0FFF)
					data = nametable[1][address & 0x03FF];
			}
		}
		else if (address >= 0x3F00 && address <= 0x3FFF)
		{
			// palette range is 0x001F, so mask address with that
			address = address & 0x001F;
			// https://wiki.nesdev.org/w/index.php?title=PPU_palettes
			// addresses $3F10, $3F14, $3F18 and $3F1C are mirrors of $3F00, $3F04, $3F08 and $3F0C
			if (address == 0x0010) address = 0;
			if (address == 0x0014) address = 0x04;
			if (address == 0x0018) address = 0x08;
			if (address == 0x001C) address = 0x0C;
			data = palettes[address];


		}
	}


	return data;
}

void ppu::ppuwrite(uint16_t address, uint8_t data)
{
	address &= 0x3FFF;
	if (!cart->ppuwrite(address, data))
	{
		if (address >= 0 && address <= 0x1FFF)
		{
			patterntable[(address & 0x1000) >> 12][address & 0x0FFF] = data;
		}
		else if (address >= 0x2000 && address <= 0x3EFF)
		{
			address &= 0x0FFF;
			if (cart->getMirror() == "vertical")
			{
				if (address >= 0 && address <= 0x03FF)
					nametable[0][address & 0x03FF] = data;
				if (address >= 0x0400 && address <= 0x07FF)
					nametable[1][address & 0x03FF] = data;
				if (address >= 0x0800 && address <= 0x0BFF)
					nametable[0][address & 0x03FF] = data;
				if (address >= 0x0C00 && address <= 0x0FFF)
					nametable[1][address & 0x03FF] = data;
			}
			else if (cart->getMirror() == "horizontal")
			{
				if (address >= 0 && address <= 0x03FF)
					nametable[0][address & 0x03FF] = data;
				if (address >= 0x0400 && address <= 0x07FF)
					nametable[0][address & 0x03FF] = data;
				if (address >= 0x0800 && address <= 0x0BFF)
					nametable[1][address & 0x03FF] = data;
				if (address >= 0x0C00 && address <= 0x0FFF)
					nametable[1][address & 0x03FF] = data;
			}
		}
		else if (address >= 0x3F00 && address <= 0x3FFF)
		{
			address &= 0x001F;
			if (address == 0x0010) address = 0;
			if (address == 0x0014) address = 0x04;
			if (address == 0x0018) address = 0x08;
			if (address == 0x001C) address = 0x0C;
			palettes[address] = data;
		}
	}
}

void ppu::connectCartridge(const std::shared_ptr<cartridge>& cartridge)
{
	// connect ppu to shared cartridge pointer
	this->cart = cartridge;
}

void ppu::reset()
{

	// reset everything
	xreg = 0;
	wreg = 0;
	ppu_data_buffer = 0;
	scanline = 0;
	cycle = 0;
	bg_next_tile_id = 0;
	bg_next_tile_attrib = 0;
	bg_next_tile_lsb = 0;
	bg_next_tile_msb = 0;
	bg_shifter_pattern_lo = 0;
	bg_shifter_pattern_hi = 0;
	bg_shifter_attrib_lo = 0;
	bg_shifter_attrib_hi = 0;
	status.reg = 0;
	mask.reg = 0;
	control.reg = 0;
	vreg.reg = 0;
	treg.reg = 0;
}

void ppu::ScrollHori()
{
	// https://wiki.nesdev.org/w/index.php/PPU_scrolling#Coarse_X_increment
	// scrolling can only happen if rendering is enabled
	if (mask.render_background || mask.render_sprites)
	{
		// nametables are 32x30; if you keep incrementing the coarse x value you will eventually reach another nametable, at which point coarse x is reset and the nametable is changed 
		if (vreg.coarse_x == 31)
		{
			vreg.coarse_x = 0;
			vreg.nametable_x = ~vreg.nametable_x;
		}
		else
		{
			vreg.coarse_x++;
		}
	}
};

void ppu::ScrollVert()
{

	if (mask.render_background || mask.render_sprites)
	{
		// https://wiki.nesdev.org/w/index.php/PPU_scrolling#Y_increment
		if (vreg.fine_y < 7)
		{
			vreg.fine_y++;
		}
		else
		{
			vreg.fine_y = 0;
			auto y = vreg.coarse_y;
			if (y == 29)
			{
				vreg.coarse_y = 0;
				vreg.nametable_y = ~vreg.nametable_y;
			}
			else if (y == 31)
			{
				// the last 2 rows are used for attribute memory so the nametable doesnt need to be flipped but coarse y needs to be reset
				vreg.coarse_y = 0;
			}
			else
			{
				vreg.coarse_y++;
			}
		}
	}
};

void ppu::UpdateHoriRegisters()
{
	if (mask.render_background || mask.render_sprites)
	{
		vreg.nametable_x = treg.nametable_x;
		vreg.coarse_x = treg.coarse_x;
	}
};

void ppu::UpdateVertRegisters()
{
	if (mask.render_background || mask.render_sprites)
	{
		vreg.fine_y = treg.fine_y;
		vreg.nametable_y = treg.nametable_y;
		vreg.coarse_y = treg.coarse_y;
	}
};

void ppu::LoadShifters()
{
	bg_shifter_pattern_lo = (bg_shifter_pattern_lo & 0xFF00) | bg_next_tile_lsb;
	bg_shifter_pattern_hi = (bg_shifter_pattern_hi & 0xFF00) | bg_next_tile_msb;

	if (bg_next_tile_attrib & 0x01) {
		bg_shifter_attrib_lo = (bg_shifter_attrib_lo & 0xFF00) | 0xFF;
	}
	else {
		bg_shifter_attrib_lo = (bg_shifter_attrib_lo & 0xFF00) | 0;
	}
	if (bg_next_tile_attrib & 0x02) {
		bg_shifter_attrib_hi = (bg_shifter_attrib_hi & 0xFF00) | 0xFF;
	}
	else {
		bg_shifter_attrib_hi = (bg_shifter_attrib_hi & 0xFF00) | 0;
	}
};

void ppu::UpdateShifters()
{
	if (mask.render_background)
	{
		bg_shifter_pattern_lo <<= 1;
		bg_shifter_pattern_hi <<= 1;
		bg_shifter_attrib_lo <<= 1;
		bg_shifter_attrib_hi <<= 1;
	}

	if (mask.render_sprites && cycle >= 1 && cycle < 258) {
		for (int i = 0; i < spriteCount; i++) {
			if (spriteScanline[i].xpos > 0) {
				spriteScanline[i].xpos--;
			}
			else {
				sprite_shifter_pattern_lo[i] <<= 1;
				sprite_shifter_pattern_hi[i] <<= 1;
			}
		}
	}
};

void ppu::clock()
{
	// https://wiki.nesdev.org/w/index.php/PPU_rendering#Visible_scanlines_.280-239.29
	// if not in vblank period
	if (scanline >= -1 && scanline < 240)
	{
		if (scanline == 0 && cycle == 0 && (mask.render_background || mask.render_sprites))
		{
			// https://wiki.nesdev.com/w/images/4/4f/Ppu.svg
			// scanline 0 and cycle 0 is skipped
			cycle++;
		}

		if (scanline == -1 && cycle == 1)
		{
			// scanline -1 is not actually real, it is just used to tell the ppu that a new frame has started, in which case the vblank flag should be reset 
			status.vertical_blank = 0;
			status.sprite_zero_hit = 0;
			// clear sprite overflow flag and shifters
			status.sprite_overflow = 0;
			for (int i = 0; i < 8; i++) {
				sprite_shifter_pattern_lo[i] = 0;
				sprite_shifter_pattern_hi[i] = 0;
			}
		}


		if ((cycle >= 2 && cycle < 258) || (cycle >= 321 && cycle < 338))
		{
			UpdateShifters();

			// https://wiki.nesdev.com/w/images/4/4f/Ppu.svg
			// repeated cycle of:
			// two clock cycles used to read a nametable byte, two clock cycles used to read an attribute table byte, two clock cycles used to read low bg tile byte, two clock cycles used to read
			// high tile bg byte, one of those also being used to increment the scroll horizontally
			switch (cycle % 8)
			{
			case 1:
				// fetch nametable byte
				// the shift registers are always pre-prepared to contain information about the next tile, so load them now as we're in that next tile
				LoadShifters();

				// the first 12 bits of the v register contain nametable y, nametable x, coarse y and coarse x info, so this can be used to index into a nametable (where $2000 is the start of 
				// the nametable address space)
				bg_next_tile_id = ppuread(0x2000 | (vreg.reg & 0x0FFF));
				break;
			case 3:
				// fetch attribute table byte
				bg_next_tile_attrib = ppuread(0x23C0 | (vreg.nametable_y << 11) | (vreg.nametable_x << 10) | ((vreg.coarse_y >> 2) << 3) | (vreg.coarse_x >> 2));
				if (vreg.coarse_y & 0x02) bg_next_tile_attrib >>= 4;
				if (vreg.coarse_x & 0x02) bg_next_tile_attrib >>= 2;
				bg_next_tile_attrib &= 0x03;
				break;

			case 5:
				// fetch low bg tile byte
				bg_next_tile_lsb = ppuread((control.pattern_background << 12) + ((uint16_t)bg_next_tile_id << 4) + (vreg.fine_y));

				break;
			case 7:
				// fetch high tile bg byte
				bg_next_tile_msb = ppuread((control.pattern_background << 12) + ((uint16_t)bg_next_tile_id << 4) + (vreg.fine_y) + 8);
				break;
			case 0:
				// increment horizontal
				ScrollHori();
				break;
			}
		}

		// https://wiki.nesdev.com/w/images/4/4f/Ppu.svg
		// cycle 256 increments the Y scroll as it is the end of visible scanlines
		if (cycle == 256)
		{
			ScrollVert();
		}

		// hori(v) = hori(t), aka update shift registers using 'next' values
		if (cycle == 257)
		{
			LoadShifters();
			UpdateHoriRegisters();
		}

		// read next tile when we reach the end of the scanline
		if (cycle == 338 || cycle == 340)
		{
			bg_next_tile_id = ppuread(0x2000 | (vreg.reg & 0x0FFF));
		}

		// when scanline = -1 the vblank period is over, meaning there will be a new frame and therefore new Y values; y must be updated for this 
		if (scanline == -1 && cycle >= 280 && cycle < 305)
		{
			UpdateVertRegisters();
		}

		if (cycle == 257 && scanline >= 0) {
			// end of visible scanline, so clear sprite scanline array by setting everything to 0xFF, then clear shifters
			std::memset(spriteScanline, 0xFF, 8 * sizeof(sprite));
			spriteCount = 0;

			for (uint8_t i = 0; i < 8; i++) {
				sprite_shifter_pattern_lo[i] = 0;
				sprite_shifter_pattern_hi[i] = 0;
			}

			// search for the first 8 visible sprites 
			uint8_t oamvalues = 0;
			spritezerohit = false;
			while (oamvalues < 64 && spriteCount < 9) {
				int16_t difference = ((int16_t)scanline - (int16_t)oam[oamvalues].ypos);
				if (control.sprite_size) {
					if (difference >= 0 && difference < 16) {
						if (spriteCount < 8) {
							// if it is sprite zero then it may trigger a sprite zero hit
							if (oamvalues == 0) {
								spritezerohit = true;
							}
							// copy oam data into sprite scanline array if there is a sprite, then increment sprite count as we have found one
							std::memcpy(&spriteScanline[spriteCount], &oam[oamvalues], sizeof(sprite));

						}
						spriteCount++;
					}
				}
				else {
					if (difference >= 0 && difference < 8) {
						if (spriteCount < 8) {
							// if it is sprite zero then it may trigger a sprite zero hit
							if (oamvalues == 0) {
								spritezerohit = true;
							}
							// copy oam data into sprite scanline array if there is a sprite, then increment sprite count as we have found one
							std::memcpy(&spriteScanline[spriteCount], &oam[oamvalues], sizeof(sprite));

						}
						spriteCount++;
					}
				}
				oamvalues++;
			}
			// sprite overflow flag is set when more than 8 sprites exist on one scanline 
			status.sprite_overflow = (spriteCount >= 8);
		}
	}

	if (cycle == 340) {

		for (uint8_t i = 0; i < spriteCount; i++) {
			uint8_t sprite_pattern_bits_lo;
			uint8_t sprite_pattern_bits_hi;
			uint16_t sprite_pattern_address_lo;
			uint16_t sprite_pattern_address_hi;

			// if 8x16
			if (control.sprite_size) {
				// https://wiki.nesdev.org/w/index.php/PPU_OAM#Byte_2
				// bit 1 of the attribute byte of each sprite holds whether the sprite is flipped vertically or not, so mask with 10000000
				if ((spriteScanline[i].attribute & 0x80)) {
					// if its 8x16 it may take up two tiles, so if the difference between the scanline and the y position of the current sprite is less than 8 we're in the top half, otherwise we're in the bottom
					if (scanline - spriteScanline[i].ypos < 8) {
						sprite_pattern_address_lo = ((spriteScanline[i].tileID & 0x01) << 12) | (((spriteScanline[i].tileID & 0xFE) + 1) << 4) | (7 - (scanline - spriteScanline[i].ypos) & 0x07);
					}
					else {
						// add 1 to the row because we're reading the bottom half 
						sprite_pattern_address_lo = ((spriteScanline[i].tileID & 0x01) << 12) | (((spriteScanline[i].tileID & 0xFE) << 4) | (7 - (scanline - spriteScanline[i].ypos) & 0x07));
					}
				}
				else {

					if (scanline - spriteScanline[i].ypos < 8) {
						sprite_pattern_address_lo = ((spriteScanline[i].tileID & 0x01) << 12) | ((spriteScanline[i].tileID & 0xFE) << 4) | ((scanline - spriteScanline[i].ypos) & 0x07);
					}
					else {

						sprite_pattern_address_lo = ((spriteScanline[i].tileID & 0x01) << 12) | (((spriteScanline[i].tileID & 0xFE) + 1) << 4) | ((scanline - spriteScanline[i].ypos) & 0x07);
					}
				}
			}
			// if 8x8
			else {
				if ((spriteScanline[i].attribute & 0x80)) {
					// select the pattern table, then OR with the tile id shifted left by 4 as each tile is 16 bytes in size, then OR with 7 - scanline - the current sprites y coordinate, which gives the row offset (7 - because it is vertically flipped)
					sprite_pattern_address_lo = (control.pattern_sprite << 12) | (spriteScanline[i].tileID << 4) | (7 - (scanline - spriteScanline[i].ypos));
				}
				else {

					sprite_pattern_address_lo = (control.pattern_sprite << 12) | (spriteScanline[i].tileID << 4) | (scanline - spriteScanline[i].ypos);
				}

			}
			// hi byte plane is 8 bytes from the lo bit plane 
			sprite_pattern_address_hi = sprite_pattern_address_lo + 8;
			sprite_pattern_bits_lo = ppuread(sprite_pattern_address_lo);
			sprite_pattern_bits_hi = ppuread(sprite_pattern_address_hi);

			// flip horizontally
			if (spriteScanline[i].attribute & 0x40) {
				sprite_pattern_bits_lo = (sprite_pattern_bits_lo & 0xF0) >> 4 | (sprite_pattern_bits_lo & 0x0F) << 4;
				sprite_pattern_bits_lo = (sprite_pattern_bits_lo & 0xCC) >> 2 | (sprite_pattern_bits_lo & 0x33) << 2;
				sprite_pattern_bits_lo = (sprite_pattern_bits_lo & 0xAA) >> 1 | (sprite_pattern_bits_lo & 0x55) << 1;
				sprite_pattern_bits_hi = (sprite_pattern_bits_hi & 0xF0) >> 4 | (sprite_pattern_bits_hi & 0x0F) << 4;
				sprite_pattern_bits_hi = (sprite_pattern_bits_hi & 0xCC) >> 2 | (sprite_pattern_bits_hi & 0x33) << 2;
				sprite_pattern_bits_hi = (sprite_pattern_bits_hi & 0xAA) >> 1 | (sprite_pattern_bits_hi & 0x55) << 1;
			}
			// load into shift registers 
			sprite_shifter_pattern_lo[i] = sprite_pattern_bits_lo;
			sprite_shifter_pattern_hi[i] = sprite_pattern_bits_hi;
		}


	}

	if (scanline >= 241 && scanline < 261)
	{
		// https://wiki.nesdev.com/w/images/4/4f/Ppu.svg
		if (scanline == 241 && cycle == 1)
		{
			// the control register may have the enable nmi flag enabled, in which case the cpu can perform operations on the ppu's address space without affecting anything as it is in the vblank period
			status.vertical_blank = 1;
			if (control.enable_nmi) nmi = true;
		}
	}



	uint8_t bg_pixel = 0;
	uint8_t bg_palette = 0;
	// select background pixel and palette to be rendered  
	if (mask.render_background)
	{
		if (mask.render_background_left || (cycle >= 9)) {
			// get the desired location using the fine x register
			uint16_t selector = 0x8000 >> xreg;
			// extract from the shifters 
			uint8_t lopixel = (bg_shifter_pattern_lo & selector) > 0;
			uint8_t hipixel = (bg_shifter_pattern_hi & selector) > 0;
			// combine these pixels together to form the full pixel
			bg_pixel = (lopixel << 1) | hipixel;
			// 
			uint8_t lopalette = (bg_shifter_attrib_lo & selector) > 0;
			uint8_t hipalette = (bg_shifter_attrib_hi & selector) > 0;
			bg_palette = (hipalette << 1) | lopalette;
		}
	}
	// 2 bit pixel to be rendered
	uint8_t foregroundPixel = 0;
	// 3 bit index of the palette the pixel uses
	uint8_t foregroundPalette = 0;
	// indicates if it has higher priority over the background
	uint8_t foregroundPriority = 0;

	// only if we are allowed to render sprites 
	if (mask.render_sprites) {

		if (mask.render_sprites_left || (cycle >= 9)) {
			spritezerorender = false;
			// iterate through every sprite on the current scanline
			for (uint8_t i = 0; i < spriteCount; i++) {
				// if the current sprites x coordinate is 0 we have to use the shifters because a collision has occured between the scanline and the sprite 
				if (spriteScanline[i].xpos == 0) {
					uint8_t foregroundPixelLo = (sprite_shifter_pattern_lo[i] & 0x80) > 0;
					uint8_t foregroundPixelHi = (sprite_shifter_pattern_hi[i] & 0x80) > 0;
					foregroundPixel = (foregroundPixelHi << 1) | foregroundPixelLo;

					foregroundPalette = (spriteScanline[i].attribute & 0x03) + 0x04;
					foregroundPriority = (spriteScanline[i].attribute & 0x20) == 0;

					// if it is visible
					if (foregroundPixel != 0) {
						// if it is sprite zero, state that we are rendering it 
						if (i == 0) {
							spritezerorender = true;
						}
						break;
					}
				}
			}
		}
	}

	// what will actually be drawn 
	uint8_t pixelFinal = 0;
	uint8_t paletteFinal = 0;

	if (bg_pixel == 0 && foregroundPixel == 0) {
		// if both the background and foreground pixels are transparent, just draw the background colour
		pixelFinal = 0;
		paletteFinal = 0;
	}
	else if (bg_pixel > 0 && foregroundPixel == 0) {
		// if the foreground pixel is transparent and the background pixel is visible just draw the background
		pixelFinal = bg_pixel;
		paletteFinal = bg_palette;
	}
	else if (bg_pixel == 0 && foregroundPixel > 0) {
		// if the background pixel is transparent and the foreground pixel is visible just draw the foreground
		pixelFinal = foregroundPixel;
		paletteFinal = foregroundPalette;
	}
	else if (bg_pixel > 0 && foregroundPixel > 0) {
		// if neither are transparent then use the foreground priority flag
		if (foregroundPriority) {
			pixelFinal = foregroundPixel;
			paletteFinal = foregroundPalette;
		}
		else {
			pixelFinal = bg_pixel;
			paletteFinal = bg_palette;
		}

		if (spritezerohit && spritezerorender) {
			if (mask.render_background & mask.render_sprites) {
				if (!(mask.render_background_left | mask.render_sprites_left)) {
					if (cycle >= 9 && cycle < 258) {
						status.sprite_zero_hit = 1;
					}
				}
				else {
					if (cycle >= 1 && cycle < 258) {
						status.sprite_zero_hit = 1;
					}
				}
			}
		}
	}

	backgroundLayout.SetPixel(cycle - 1, scanline, GetPixel(paletteFinal, pixelFinal));

	// increment cycle every time a clock cycle is executed 
	cycle++;
	if (cycle >= 341)
	{
		// 341 cycles in one frame, so reset the cycles whenever we reach the end
		cycle = 0;
		scanline++;
		if (scanline >= 261)
		{
			// also reset the scanline
			scanline = -1;
			frame_complete = true;
		}
	}
}