#pragma once
class bus;

class cpu
{
public:
	cpu();

	void connectBus(bus *x) {
		bus = x;
	}
private:
	bus *bus = nullptr;
	uint8_t read(uint16_t address);
	void write(uint16_t address, uint8_t data);
};




