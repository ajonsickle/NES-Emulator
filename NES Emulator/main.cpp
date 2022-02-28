
#include <iostream>
#include <sstream>
#include <filesystem>
#include <fstream>
#include <string>

#include "bus.h"
#include "cpu.h"

#define OLC_PGE_APPLICATION
#include "gameEngine.h"

class nesEmulator : public olc::PixelGameEngine
{
	
public:

	bus nes;
	std::shared_ptr<cartridge> cart;
	bool running = false;
	float fResidualTime = 0.0f;
	std::vector<std::filesystem::directory_entry> files;
	int counter = 0;

	void addToFilesVec(std::filesystem::directory_entry x) {
		files.push_back(x);
	}
	// game engine method override
	bool OnUserCreate()
	{
		// add to shared cartridge pointer and insert into bus 
		std::string stringvar = files[counter].path().string();
		cart = std::make_shared<cartridge>(stringvar);
		nes.insertcartridge(cart);
		nes.reset();
		return true;
	}
	void resetButton(uint8_t controller) {
			nes.controller = nes.controller | 0b00000000;
	}
	// game engine method override 
	// from docs: // Called every frame, and provides you with a time per frame value
	bool OnUserUpdate(float fElapsedTime)
	{
		Clear(olc::DARK_BLUE);
		// controller mappings from the game engine 
		// https://wiki.nesdev.org/w/index.php/Standard_controller
		// controller stored 8 bits for 8 keys; 0 - A, 1 - B, 2 - Select, 3 - Start, 4 - Up, 5 - Down, 6 - Left, 7 - Right
		nes.controller = 0;
		if (GetKey(olc::Key::SPACE).bHeld) {
			nes.controller= nes.controller | 0b10000000;
		}
		else {
			resetButton(0);
		}
		if (GetKey(olc::Key::X).bHeld) {
			nes.controller = nes.controller | 0b01000000;
		}
		else {
			resetButton(0);
		}
		if (GetKey(olc::Key::A).bHeld) {
			nes.controller = nes.controller | 0b00100000;
		}
		else {
			resetButton(0);
		}
		if (GetKey(olc::Key::S).bHeld) {
			nes.controller = nes.controller | 0b00010000;
		}
		else {
			resetButton(0);
		}
		if (GetKey(olc::Key::UP).bHeld) {
			nes.controller = nes.controller | 0b00001000;
		}
		else {
			resetButton(0);
		}
		if (GetKey(olc::Key::DOWN).bHeld) {
			nes.controller = nes.controller | 0b00000100;
		}
		else {
			resetButton(0);
		}
		if (GetKey(olc::Key::LEFT).bHeld) {
			nes.controller = nes.controller | 0b00000010;
		}
		else {
			resetButton(0);
		}
		if (GetKey(olc::Key::RIGHT).bHeld) {
			nes.controller = nes.controller | 0b00000001;
		}
		else {
			resetButton(0);
		}

		if (GetKey(olc::Key::P).bPressed) running = !running;

		if (GetKey(olc::Key::R).bPressed) nes.reset();
		// press c to cycle through games in rom folder
		if (GetKey(olc::Key::C).bPressed && files.size() != 1) {
			nes.reset();
			counter++;
			if (counter == files.size()) {
				counter = 0;
			}
			OnUserCreate();
		}
		if (GetKey(olc::Key::B).bPressed && files.size() != 1) {
			nes.reset();
			counter--;
			if (counter == -1) {
				counter = files.size() - 1; 
			}
			OnUserCreate();
		}

		// clock until the frame is complete. 60hz so complete one frame every 1/60th of a second 
		if (running) {
			if (fResidualTime > 0.0f)
				fResidualTime -= fElapsedTime;
			else
			{
				fResidualTime += (1.0f / 60.0f) - fElapsedTime;
				do {
					nes.clock();
				} while (!nes.ppu.frame_complete);
				nes.ppu.frame_complete = false;
			}
		}

		DrawSprite(0, 0, &nes.ppu.getBGLayout(), 2);
		return true;
	}
};
int main()
{
	nesEmulator emu;
	using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;
	// loop through rom folder and add to files vector 
	std::filesystem::path path = std::filesystem::current_path() / "ROMs";
	for (const auto& dirEntry : recursive_directory_iterator("C:\\ROMs")) {
		
		emu.addToFilesVec(dirEntry);
	}
	
	emu.Construct(512, 480, 2, 2, true, true, false);
	emu.Start();
	return 0;
}