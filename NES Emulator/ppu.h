#pragma once
#include <cstdint>
#include <memory>

#include "gameEngine.h"

#include "cartridge.h"

class ppu
{
public:
	ppu();

	// https://wiki.nesdev.org/w/index.php?title=PPU_nametables
	// the nes contained 2 physical 1024 byte nametables (areas of memory used for background layout) but 4 logical ones after mirroring. each byte represents a tile, which is a character cell consisting of 8x8 pixels.
	// each nametable had 30 rows of 32 tiles, therefore using 30 x 32 = 960 bytes. The remaining 64 were used by the attribute tables contained at the end of each nametable 
	// which controlled which palette was assigned to each part of the background. 
	uint8_t nametable[2][1024];
	// https://wiki.nesdev.org/w/index.php?title=PPU_pattern_tables
	// contained a pattern table, an area of memory that stored either background tiles or sprite tiles. each tile in the pattern table was 16 bytes (8x8x2), and represented
	// an 8x8 region of pixels on the screen. every 8x1 row in a specific tile was represented by a pair of 2 bytes, one being called the most significant byte, and the other the 
	// least significant byte. this was because the nes only had a colour depth of 2, meaning 2 bits per pixel. these 2 bits were therefore split up so that a 'most significant' 
	// and a 'least significant' tile could be created by combining 8 of these 8x1 pixel rows. this was convenient as it meant complicated bitwise manipulation didnt have to be done
	// the pattern table was divided into two 256-tile sections (16x16), left and right. one half usually handles sprites while the other handles background layout 
	uint8_t patterntable[2][256 * 16];
	// https://wiki.nesdev.org/w/index.php?title=PPU_palettes
	// palette memory map runs from $3F00 to $3F1F, where each colour takes up one byte. therefore 32 bytes are required.
	// the choice of palette for each 16x16 pixel area is controlled by the attribute table at the end of each nametable. 
	// at $3F00, the background colour was held, which was an 8-bit value that indexes one of the colours the nes was able to render. there were 4 background palettes
	// and 4 sprite palettes. for each of these palettes, 3 memory addresses were available; for example, background palette 0 had $3F01 - $3F03, therefore 3 bytes, or 3 colours. 
	// there is therefore an unused memory address between each palette ($3F04/$3F08/$3F0C etc.) which can contain unique data or be represented as a transparent pixel, so the background 
	// is shown instead 
	uint8_t	palettes[32];

	// https://wiki.nesdev.org/w/index.php/PPU_OAM
	// struct to hold a singular sprite. each sprite's information occupies 4 bytes, where byte 0 is the y position of the top of the sprite, byte 1 is the tile index number, byte 2 is the attributes, and byte 3 is the x position of the left side of the sprite
	
	struct sprite {
		uint8_t ypos;
		uint8_t tileID;
		uint8_t attribute;
		uint8_t xpos;
	};
	// the oam memory stores 64 sprites
	sprite oam[64];
	// pointer to oam memory space
	uint8_t* oampointer = (uint8_t*)oam;
	// OAMADDR register
	uint8_t OAMADDR = 0;
	// OAMDATA register
	uint8_t OAMDATA = 0;

	sprite spriteScanline[8];
	uint8_t spriteCount;
	uint8_t sprite_shifter_pattern_lo[8];
	uint8_t sprite_shifter_pattern_hi[8];

	bool spritezerohit = false;
	bool spritezerorender = false;

	// array of every pixel the nes is capable of displaying as shown in https://wiki.nesdev.org/w/index.php?title=PPU_palettes#Palettes
	std::array<olc::Pixel, 64> colours;
	// represents a single sprite of 256x240 pixels, physical render of the background layout 
	olc::Sprite backgroundLayout = olc::Sprite(256, 240);
	// left and right pattern tables are represented as sprites 
	std::array<olc::Sprite, 2> patternTables = { olc::Sprite(128, 128), olc::Sprite(128, 128) };
	// method to return a reference to the background layout sprite 
	olc::Sprite& getBGLayout();
	// method to return a reference to the desired pattern table
	olc::Sprite& getPatternTables(uint8_t i, uint8_t palette);
	// returns reference to a specific colour given the palette and pixel to search for 
	olc::Pixel& GetPixel(uint8_t palette, uint8_t pixel);

	bool frame_complete = false;

	// ppu has 9 registers - https://wiki.nesdev.org/w/index.php?title=PPU_registers#Summary
	// ctrl register has various flags controlling the operation of the ppu
	union PPUCTRL
	{
		struct
		{
			uint8_t nametable_x : 1;
			uint8_t nametable_y : 1;
			uint8_t increment_mode : 1;
			uint8_t pattern_sprite : 1;
			uint8_t pattern_background : 1;
			uint8_t sprite_size : 1;
			uint8_t slave_mode : 1;
			uint8_t enable_nmi : 1;
		};

		uint8_t reg;
	};
	PPUCTRL control;
	// mask register controls the rendering of sprites, backgrounds and colour effects 
	union PPUMASK
	{
		struct
		{
			uint8_t grayscale : 1;
			uint8_t render_background_left : 1;
			uint8_t render_sprites_left : 1;
			uint8_t render_background : 1;
			uint8_t render_sprites : 1;
			uint8_t enhance_red : 1;
			uint8_t enhance_green : 1;
			uint8_t enhance_blue : 1;
		};

		uint8_t reg;
	};
	PPUMASK mask;
	// status register reflects the state of the ppu
	union PPUSTATUS
	{
		struct
		{
			uint8_t unused : 5;
			uint8_t sprite_overflow : 1;
			uint8_t sprite_zero_hit : 1;
			uint8_t vertical_blank : 1;
		};

		uint8_t reg;
	};
	PPUSTATUS status;

	// https://wiki.nesdev.org/w/index.php/PPU_scrolling
	// the nes scrolls, i.e. moves the displayed portion of the map. this is done in many games, most famously in super mario bros, by moving mario right the map also scrolls right.
	// the range $4000 - $FFFF mirrors the lower part of the PPU memory, so memory access only needs 14-bit addresses. These addresses are constructed from several registers, 
	// including the v register, t register, x register and w register. 
	// The v and t registers are both 15 bits, where the v register holds the current PPU address, or whatever the PPU is about to access. The t register holds 
	// a temporary address shared by the PPUSCROLL and the PPUADDR register. It can be thought of as the address of the top left tile on screen; it is essentially a buffer
	// for the v register as changes to this register will be copied to v. The x register is 3 bits and holds the fine x position (the x position within a 8x8 pixel tile). The 
	// w register is a flag which determines the first/second write toggle of the PPUSCROLL and PPUADDR registers. 

	// fine offsets specify what part of an 8x8 tile each pixel falls on, and coarse offsets specify which tile. the fine x value does not change during rendering, and it can only be changed
	// by a $2005 write 
	union bgScrollReg
	{
		struct
		{

			uint16_t coarse_x : 5;
			uint16_t coarse_y : 5;
			uint16_t nametable_x : 1;
			uint16_t nametable_y : 1;
			uint16_t fine_y : 3;
			uint16_t unused : 1;
		};

		uint16_t reg = 0;
	};
	bgScrollReg vreg; 
	bgScrollReg treg;
	uint8_t xreg = 0;
	uint8_t wreg = 0;

	// buffer containing data from the previous read request
	uint8_t ppu_data_buffer = 0;

	// https://wiki.nesdev.org/w/index.php/PPU_rendering
	// scanlines are essentially the rows of rendering. the ppu renders 262 scanlines per frame, as the display is 256x240 however there are 22 invisible scanlines where the program 
	// is allowed to access PPU memory (vertical blank period). From scanlines 0-239, the visible ones, the PPU is busy fetching data so the program should not access PPU memory during this time.
	int16_t scanline = 0;
	// each scanline lasts for 341 PPU clock cycles, with each clock cycle producing one pixel. cycle 0 is an idle cycle; in cycles 1-256, the data for each tile 
	// is fetched. each memory access takes 2 PPU cycles to complete, and 4 must be performed per tile: nametable byte, attribute table byte, pattern table tile low, pattern
	// table tile high. the data from these accesses is placed into internal latches and written to the appropriate registers every 8 cycles. from cycles 
	// 257-230, the tile data for the sprites on the next scanline are fetched. From cycles 321-336, the first two tiles for the next scanline are fetched 
	// and loaded into the shift registers. Finally, from cycles 337-340, two bytes are fetched representing the same nametable byte that will be fetched at the beginning 
	// of the next scanline. 
	int16_t cycle = 0;
	uint8_t bg_next_tile_id = 0;
	uint8_t bg_next_tile_attrib = 0;
	uint8_t bg_next_tile_lsb = 0;
	uint8_t bg_next_tile_msb = 0;
	uint16_t bg_shifter_pattern_lo = 0;
	uint16_t bg_shifter_pattern_hi = 0;
	uint16_t bg_shifter_attrib_lo = 0;
	uint16_t bg_shifter_attrib_hi = 0;

	void ScrollHori();
	void ScrollVert();
	void UpdateHoriRegisters();
	void UpdateVertRegisters();
	void LoadShifters();
	void UpdateShifters();
	// read/write to ppu registers 
	uint8_t read(uint16_t address);
	void write(uint16_t address, uint8_t data);
	// ppu read/write to ppu memory
	uint8_t ppuread(uint16_t address);
	void ppuwrite(uint16_t address, uint8_t data);
	// cartridge pointer, shared with bus 
	std::shared_ptr<cartridge> cart;
	// connect cartridge to ppu
	void connectCartridge(const std::shared_ptr<cartridge>& cartridge);
	// execute one ppu clock cycle
	void clock();
	// reset ppu
	void reset();
	// can hold true during the vertical blanking period 
	bool nmi = false;
};
