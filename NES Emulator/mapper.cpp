#include "mapper.h"

// constructor
mapper::mapper(uint8_t prgsections, uint8_t chrsections) {
	prgsections1 = prgsections;
	chrsections1 = chrsections;
	reset();
}

void mapper::reset() {

};

std::string mapper::mirror() {
	return "hardware";
}