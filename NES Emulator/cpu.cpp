#include "cpu.h"
#include <cstdint>
#include "bus.h"
#include "bus.cpp"
cpu::cpu() {

}
void cpu::write(uint16_t address, uint8_t data) {
	bus->write(address, data);
};
uint8_t cpu::read(uint16_t address) {
	return bus->read(address);
}

