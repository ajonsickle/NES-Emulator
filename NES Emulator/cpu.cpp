#include "cpu.h"
#include <cstdint>
#include "bus.h"
#include <map>
// constructor
cpu::cpu() {
	// instruction set op code matrix. based on http://archive.6502.org/datasheets/rockwell_r65c00_microprocessors.pdf page 9
	// ??? represents any illegal opcodes or NOP operations. these are represented by blank spaces in the pdf
	lookup =
	{
		{ "BRK", &cpu::BRK, &cpu::IMM, 7 },{ "ORA", &cpu::ORA, &cpu::IZX, 6 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 8 },{ "???", &cpu::NOP, &cpu::IMP, 3 },{ "ORA", &cpu::ORA, &cpu::ZP0, 3 },{ "ASL", &cpu::ASL, &cpu::ZP0, 5 },{ "???", &cpu::XXX, &cpu::IMP, 5 },{ "PHP", &cpu::PHP, &cpu::IMP, 3 },{ "ORA", &cpu::ORA, &cpu::IMM, 2 },{ "ASL", &cpu::ASL, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "???", &cpu::NOP, &cpu::IMP, 4 },{ "ORA", &cpu::ORA, &cpu::ABS, 4 },{ "ASL", &cpu::ASL, &cpu::ABS, 6 },{ "???", &cpu::XXX, &cpu::IMP, 6 },
		{ "BPL", &cpu::BPL, &cpu::REL, 2 },{ "ORA", &cpu::ORA, &cpu::IZY, 5 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 8 },{ "???", &cpu::NOP, &cpu::IMP, 4 },{ "ORA", &cpu::ORA, &cpu::ZPX, 4 },{ "ASL", &cpu::ASL, &cpu::ZPX, 6 },{ "???", &cpu::XXX, &cpu::IMP, 6 },{ "CLC", &cpu::CLC, &cpu::IMP, 2 },{ "ORA", &cpu::ORA, &cpu::ABY, 4 },{ "???", &cpu::NOP, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 7 },{ "???", &cpu::NOP, &cpu::IMP, 4 },{ "ORA", &cpu::ORA, &cpu::ABX, 4 },{ "ASL", &cpu::ASL, &cpu::ABX, 7 },{ "???", &cpu::XXX, &cpu::IMP, 7 },
		{ "JSR", &cpu::JSR, &cpu::ABS, 6 },{ "AND", &cpu::AND, &cpu::IZX, 6 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 8 },{ "BIT", &cpu::BIT, &cpu::ZP0, 3 },{ "AND", &cpu::AND, &cpu::ZP0, 3 },{ "ROL", &cpu::ROL, &cpu::ZP0, 5 },{ "???", &cpu::XXX, &cpu::IMP, 5 },{ "PLP", &cpu::PLP, &cpu::IMP, 4 },{ "AND", &cpu::AND, &cpu::IMM, 2 },{ "ROL", &cpu::ROL, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "BIT", &cpu::BIT, &cpu::ABS, 4 },{ "AND", &cpu::AND, &cpu::ABS, 4 },{ "ROL", &cpu::ROL, &cpu::ABS, 6 },{ "???", &cpu::XXX, &cpu::IMP, 6 },
		{ "BMI", &cpu::BMI, &cpu::REL, 2 },{ "AND", &cpu::AND, &cpu::IZY, 5 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 8 },{ "???", &cpu::NOP, &cpu::IMP, 4 },{ "AND", &cpu::AND, &cpu::ZPX, 4 },{ "ROL", &cpu::ROL, &cpu::ZPX, 6 },{ "???", &cpu::XXX, &cpu::IMP, 6 },{ "SEC", &cpu::SEC, &cpu::IMP, 2 },{ "AND", &cpu::AND, &cpu::ABY, 4 },{ "???", &cpu::NOP, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 7 },{ "???", &cpu::NOP, &cpu::IMP, 4 },{ "AND", &cpu::AND, &cpu::ABX, 4 },{ "ROL", &cpu::ROL, &cpu::ABX, 7 },{ "???", &cpu::XXX, &cpu::IMP, 7 },
		{ "RTI", &cpu::RTI, &cpu::IMP, 6 },{ "EOR", &cpu::EOR, &cpu::IZX, 6 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 8 },{ "???", &cpu::NOP, &cpu::IMP, 3 },{ "EOR", &cpu::EOR, &cpu::ZP0, 3 },{ "LSR", &cpu::LSR, &cpu::ZP0, 5 },{ "???", &cpu::XXX, &cpu::IMP, 5 },{ "PHA", &cpu::PHA, &cpu::IMP, 3 },{ "EOR", &cpu::EOR, &cpu::IMM, 2 },{ "LSR", &cpu::LSR, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "JMP", &cpu::JMP, &cpu::ABS, 3 },{ "EOR", &cpu::EOR, &cpu::ABS, 4 },{ "LSR", &cpu::LSR, &cpu::ABS, 6 },{ "???", &cpu::XXX, &cpu::IMP, 6 },
		{ "BVC", &cpu::BVC, &cpu::REL, 2 },{ "EOR", &cpu::EOR, &cpu::IZY, 5 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 8 },{ "???", &cpu::NOP, &cpu::IMP, 4 },{ "EOR", &cpu::EOR, &cpu::ZPX, 4 },{ "LSR", &cpu::LSR, &cpu::ZPX, 6 },{ "???", &cpu::XXX, &cpu::IMP, 6 },{ "CLI", &cpu::CLI, &cpu::IMP, 2 },{ "EOR", &cpu::EOR, &cpu::ABY, 4 },{ "???", &cpu::NOP, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 7 },{ "???", &cpu::NOP, &cpu::IMP, 4 },{ "EOR", &cpu::EOR, &cpu::ABX, 4 },{ "LSR", &cpu::LSR, &cpu::ABX, 7 },{ "???", &cpu::XXX, &cpu::IMP, 7 },
		{ "RTS", &cpu::RTS, &cpu::IMP, 6 },{ "ADC", &cpu::ADC, &cpu::IZX, 6 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 8 },{ "???", &cpu::NOP, &cpu::IMP, 3 },{ "ADC", &cpu::ADC, &cpu::ZP0, 3 },{ "ROR", &cpu::ROR, &cpu::ZP0, 5 },{ "???", &cpu::XXX, &cpu::IMP, 5 },{ "PLA", &cpu::PLA, &cpu::IMP, 4 },{ "ADC", &cpu::ADC, &cpu::IMM, 2 },{ "ROR", &cpu::ROR, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "JMP", &cpu::JMP, &cpu::IND, 5 },{ "ADC", &cpu::ADC, &cpu::ABS, 4 },{ "ROR", &cpu::ROR, &cpu::ABS, 6 },{ "???", &cpu::XXX, &cpu::IMP, 6 },
		{ "BVS", &cpu::BVS, &cpu::REL, 2 },{ "ADC", &cpu::ADC, &cpu::IZY, 5 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 8 },{ "???", &cpu::NOP, &cpu::IMP, 4 },{ "ADC", &cpu::ADC, &cpu::ZPX, 4 },{ "ROR", &cpu::ROR, &cpu::ZPX, 6 },{ "???", &cpu::XXX, &cpu::IMP, 6 },{ "SEI", &cpu::SEI, &cpu::IMP, 2 },{ "ADC", &cpu::ADC, &cpu::ABY, 4 },{ "???", &cpu::NOP, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 7 },{ "???", &cpu::NOP, &cpu::IMP, 4 },{ "ADC", &cpu::ADC, &cpu::ABX, 4 },{ "ROR", &cpu::ROR, &cpu::ABX, 7 },{ "???", &cpu::XXX, &cpu::IMP, 7 },
		{ "???", &cpu::NOP, &cpu::IMP, 2 },{ "STA", &cpu::STA, &cpu::IZX, 6 },{ "???", &cpu::NOP, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 6 },{ "STY", &cpu::STY, &cpu::ZP0, 3 },{ "STA", &cpu::STA, &cpu::ZP0, 3 },{ "STX", &cpu::STX, &cpu::ZP0, 3 },{ "???", &cpu::XXX, &cpu::IMP, 3 },{ "DEY", &cpu::DEY, &cpu::IMP, 2 },{ "???", &cpu::NOP, &cpu::IMP, 2 },{ "TXA", &cpu::TXA, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "STY", &cpu::STY, &cpu::ABS, 4 },{ "STA", &cpu::STA, &cpu::ABS, 4 },{ "STX", &cpu::STX, &cpu::ABS, 4 },{ "???", &cpu::XXX, &cpu::IMP, 4 },
		{ "BCC", &cpu::BCC, &cpu::REL, 2 },{ "STA", &cpu::STA, &cpu::IZY, 6 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 6 },{ "STY", &cpu::STY, &cpu::ZPX, 4 },{ "STA", &cpu::STA, &cpu::ZPX, 4 },{ "STX", &cpu::STX, &cpu::ZPY, 4 },{ "???", &cpu::XXX, &cpu::IMP, 4 },{ "TYA", &cpu::TYA, &cpu::IMP, 2 },{ "STA", &cpu::STA, &cpu::ABY, 5 },{ "TXS", &cpu::TXS, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 5 },{ "???", &cpu::NOP, &cpu::IMP, 5 },{ "STA", &cpu::STA, &cpu::ABX, 5 },{ "???", &cpu::XXX, &cpu::IMP, 5 },{ "???", &cpu::XXX, &cpu::IMP, 5 },
		{ "LDY", &cpu::LDY, &cpu::IMM, 2 },{ "LDA", &cpu::LDA, &cpu::IZX, 6 },{ "LDX", &cpu::LDX, &cpu::IMM, 2 },{ "???", &cpu::XXX, &cpu::IMP, 6 },{ "LDY", &cpu::LDY, &cpu::ZP0, 3 },{ "LDA", &cpu::LDA, &cpu::ZP0, 3 },{ "LDX", &cpu::LDX, &cpu::ZP0, 3 },{ "???", &cpu::XXX, &cpu::IMP, 3 },{ "TAY", &cpu::TAY, &cpu::IMP, 2 },{ "LDA", &cpu::LDA, &cpu::IMM, 2 },{ "TAX", &cpu::TAX, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "LDY", &cpu::LDY, &cpu::ABS, 4 },{ "LDA", &cpu::LDA, &cpu::ABS, 4 },{ "LDX", &cpu::LDX, &cpu::ABS, 4 },{ "???", &cpu::XXX, &cpu::IMP, 4 },
		{ "BCS", &cpu::BCS, &cpu::REL, 2 },{ "LDA", &cpu::LDA, &cpu::IZY, 5 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 5 },{ "LDY", &cpu::LDY, &cpu::ZPX, 4 },{ "LDA", &cpu::LDA, &cpu::ZPX, 4 },{ "LDX", &cpu::LDX, &cpu::ZPY, 4 },{ "???", &cpu::XXX, &cpu::IMP, 4 },{ "CLV", &cpu::CLV, &cpu::IMP, 2 },{ "LDA", &cpu::LDA, &cpu::ABY, 4 },{ "TSX", &cpu::TSX, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 4 },{ "LDY", &cpu::LDY, &cpu::ABX, 4 },{ "LDA", &cpu::LDA, &cpu::ABX, 4 },{ "LDX", &cpu::LDX, &cpu::ABY, 4 },{ "???", &cpu::XXX, &cpu::IMP, 4 },
		{ "CPY", &cpu::CPY, &cpu::IMM, 2 },{ "CMP", &cpu::CMP, &cpu::IZX, 6 },{ "???", &cpu::NOP, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 8 },{ "CPY", &cpu::CPY, &cpu::ZP0, 3 },{ "CMP", &cpu::CMP, &cpu::ZP0, 3 },{ "DEC", &cpu::DEC, &cpu::ZP0, 5 },{ "???", &cpu::XXX, &cpu::IMP, 5 },{ "INY", &cpu::INY, &cpu::IMP, 2 },{ "CMP", &cpu::CMP, &cpu::IMM, 2 },{ "DEX", &cpu::DEX, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "CPY", &cpu::CPY, &cpu::ABS, 4 },{ "CMP", &cpu::CMP, &cpu::ABS, 4 },{ "DEC", &cpu::DEC, &cpu::ABS, 6 },{ "???", &cpu::XXX, &cpu::IMP, 6 },
		{ "BNE", &cpu::BNE, &cpu::REL, 2 },{ "CMP", &cpu::CMP, &cpu::IZY, 5 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 8 },{ "???", &cpu::NOP, &cpu::IMP, 4 },{ "CMP", &cpu::CMP, &cpu::ZPX, 4 },{ "DEC", &cpu::DEC, &cpu::ZPX, 6 },{ "???", &cpu::XXX, &cpu::IMP, 6 },{ "CLD", &cpu::CLD, &cpu::IMP, 2 },{ "CMP", &cpu::CMP, &cpu::ABY, 4 },{ "NOP", &cpu::NOP, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 7 },{ "???", &cpu::NOP, &cpu::IMP, 4 },{ "CMP", &cpu::CMP, &cpu::ABX, 4 },{ "DEC", &cpu::DEC, &cpu::ABX, 7 },{ "???", &cpu::XXX, &cpu::IMP, 7 },
		{ "CPX", &cpu::CPX, &cpu::IMM, 2 },{ "SBC", &cpu::SBC, &cpu::IZX, 6 },{ "???", &cpu::NOP, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 8 },{ "CPX", &cpu::CPX, &cpu::ZP0, 3 },{ "SBC", &cpu::SBC, &cpu::ZP0, 3 },{ "INC", &cpu::INC, &cpu::ZP0, 5 },{ "???", &cpu::XXX, &cpu::IMP, 5 },{ "INX", &cpu::INX, &cpu::IMP, 2 },{ "SBC", &cpu::SBC, &cpu::IMM, 2 },{ "NOP", &cpu::NOP, &cpu::IMP, 2 },{ "???", &cpu::SBC, &cpu::IMP, 2 },{ "CPX", &cpu::CPX, &cpu::ABS, 4 },{ "SBC", &cpu::SBC, &cpu::ABS, 4 },{ "INC", &cpu::INC, &cpu::ABS, 6 },{ "???", &cpu::XXX, &cpu::IMP, 6 },
		{ "BEQ", &cpu::BEQ, &cpu::REL, 2 },{ "SBC", &cpu::SBC, &cpu::IZY, 5 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 8 },{ "???", &cpu::NOP, &cpu::IMP, 4 },{ "SBC", &cpu::SBC, &cpu::ZPX, 4 },{ "INC", &cpu::INC, &cpu::ZPX, 6 },{ "???", &cpu::XXX, &cpu::IMP, 6 },{ "SED", &cpu::SED, &cpu::IMP, 2 },{ "SBC", &cpu::SBC, &cpu::ABY, 4 },{ "NOP", &cpu::NOP, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 7 },{ "???", &cpu::NOP, &cpu::IMP, 4 },{ "SBC", &cpu::SBC, &cpu::ABX, 4 },{ "INC", &cpu::INC, &cpu::ABX, 7 },{ "???", &cpu::XXX, &cpu::IMP, 7 },
	};
}
// destructor 
cpu::~cpu() {

}
// calls the bus write function, writes a byte of data to the specified address in ram 
void cpu::write(uint16_t address, uint8_t data) {
	bus->write(address, data);
};
// calls bus read function, reads a byte of data from the address specified in ram
uint8_t cpu::read(uint16_t address) {
	return bus->read(address, false);
}
// if there are 0 cycles left to perform, then the next instruction is ready to be fetched. the opcode is read from the program counter and the program counter is incremented as in the fetch execute cycle,
// then the lookup table is referenced to find the number of cycles that are needed. there are also 2 variables for additional cycles, as sometimes an addressing mode/instruction will require 
// extra clock cycles to finish processing, so every addressing mode and opcode function return a value to represent this. these values are added on to the cycles counter. if cycles is not 0, cycles is decremented
void cpu::clock() {
	if (cycles == 0) {
		opcode = read(pc);
		pc++;
		cycles = lookup[opcode].cycles;
		uint8_t additional_cycle1 = (this->*lookup[opcode].addressingmode)();
		uint8_t additional_cycle2 = (this->*lookup[opcode].operate)();
		cycles += (additional_cycle1 & additional_cycle2);

	}
	cycles--;
}
// 
uint8_t cpu::GetFlag(statusflagslist f)
{
	/*
	gets specific value of a bit in the status register, f being the flag that is to be checked.performing an AND operation on the status register and the flag will produce a byte where everything is 0
	except the position of the flag if it is set. for example if you want to check if the zero flag is set, as it is the 2nd bit if it is set the status register will show 00000010, while f will be 
	00000010 as well. performing an AND operation between these two will produce 00000010 which is 2 in decimal and therefore greater than 0, so it will return 1, indicating that the zero flag is set.
	on the other hand, if the carry flag is set while the zero flag is not, the status register will be 00000001 while f will be 00000010. an and operation of these will produce 00000000, which is not greater than 
	0, returning 0 accordingly to indicate that the zero flag is not set.
	*/ 
	if (sr & f > 0) {
		return 1;
	}
	else {
		return 0;
	}
}

/*
sets a specific bit of the status register. v represents the value you want to set it to, true for 1, false for 0. therefore if the status register contained 
00000010 and you wanted to clear the zero flag, f would be 00000010 and v would be false. so the status register and NOT f would be ANDed together, giving 
00000010 & 11111101 which is 00000000. The new status register would then be 00000000 with the zero flag cleared.  
*/ 
void cpu::SetFlag(statusflagslist f, bool v)
{
	if (v)
		sr = sr | f;
	else
		sr = sr & ~f;
}

//addressing modes 
// implied addressing means the data is mandatory for the instruction. the operand is therefore implicitly defined by the instruction because there is no address operand. fetched data is therefore taken straight from the accumulator
uint8_t cpu::IMP() {
	fetched = accumulator;
	return 0;
}
// in immediate addressing the operand itself is used as the value for the operation rather than fetching a value from a memory address. this operand is held in the program counter so the absolute address is set to the next increment of the pc
uint8_t cpu::IMM() {
	addr_abs = pc++;
	return 0;
}
// ram was set up in 256-byte pages, and as the NES hardware had a 2048 byte RAM there were eight pages. zero page addressing fetches the value from an 8-bit address on the zero page (the first page). the 
// zero page is from address range $0000 to $00FF, so the absolute address is read from the program counter and ANDed with the upper bound of the address range to only preserve the last 8 bits
uint8_t cpu::ZP0() {
	addr_abs = read(pc);
	pc++;
	addr_abs &= 0x00FF;
	return 0;
}
// zero page addressing with the absolute address being offset by the x register
uint8_t cpu::ZPX() {
	addr_abs = (read(pc) + xindex);
	pc++;
	addr_abs &= 0x00FF;
	return 0;
}
// zero page addressing with the absolute address being offset by the y register
uint8_t cpu::ZPY() {
	addr_abs = (read(pc) + yindex);
	pc++;
	addr_abs &= 0x00FF;
	return 0;
}
// the operand is the 2 byte effective address. the first (lo) byte is the least significant byte while the second (hi) byte is the most significant byte. 
// therefore if the operand is $3491, the lo byte will be 11010000 while the hi byte will be 10010001. shifting the hi byte left by 8 bits gives 1001000100000000. ORing this with the lo byte gives 
// 0000000011010000 | 1001000100000000 = 1001000111010000 which is a combination of both the lo byte and hi byte, producing a full 16-bit address $3491. 
uint8_t cpu::ABS() {
	uint16_t lo = read(pc);
	pc++;
	uint16_t hi = read(pc);
	pc++;

	addr_abs = (hi << 8) | lo;
	return 0;
}
// absolute addressing with the absolute address being offset by the x register. ANDing the absolute address with $FF00 preserves the first 8 bits, so if this does not match the hi byte, it means 
// a page boundary has been crossed, and therefore an extra cycle is required. 
uint8_t cpu::ABX() {
	uint16_t lo = read(pc);
	pc++;
	uint16_t hi = read(pc);
	pc++;

	addr_abs = (hi << 8) | lo;
	addr_abs += xindex;

	if ((addr_abs & 0xFF00) != (hi << 8)) {
		return 1;
	}
	else {
		return 0;
	}
}
// absolute addressing with the absolute address being offset by the y register.
uint8_t cpu::ABY() {
	uint16_t lo = read(pc);
	pc++;
	uint16_t hi = read(pc);
	pc++;

	addr_abs = (hi << 8) | lo;
	addr_abs += yindex;

	if ((addr_abs & 0xFF00) != (hi << 8)) {
		return 1;
	}
	else {
		return 0;
	}
}
// indirect addressing, data is accessed using a pointer. the 16-bit address of the pointer is given in the two bytes following the opcode. only used by the jump instruction
uint8_t cpu::IND() {
	uint16_t ptr_lo = read(pc);
	pc++;
	uint16_t ptr_hi = read(pc);
	pc++;

	uint16_t ptr = (ptr_hi << 8) | ptr_lo;
	// if the lo byte is 0x00FF then a page boundary must be crossed to read the hi byte. on the 6502 this didn't work and instead wrapped around in the same page (https://www.nesdev.com/6502bugs.txt)
	if (ptr_lo == 0x00FF)
	{
		addr_abs = (read(ptr & 0xFF00) << 8) | read(ptr);
	}
	else 
	{
		addr_abs = (read(ptr + 1) << 8) | read(ptr);
	}

	return 0;
}
// indirect X addressing. the address read from the program counter is offset by the x register to index an address which contains the actual required data
uint8_t cpu::IZX() {
	uint16_t t = read(pc);
	pc++;

	uint16_t lo = read((uint16_t)(t + (uint16_t)xindex) & 0x00FF);
	uint16_t hi = read((uint16_t)(t + (uint16_t)xindex + 1) & 0x00FF);

	addr_abs = (hi << 8) | lo;

	return 0;
}
// // indirect Y addressing. the address read from the program counter is offset by the y register to index an address which contains the actual required data
uint8_t cpu::IZY()
{
	uint16_t t = read(pc);
	pc++;

	uint16_t lo = read(t & 0x00FF);
	uint16_t hi = read((t + 1) & 0x00FF);

	addr_abs = (hi << 8) | lo;
	addr_abs += yindex;

	if ((addr_abs & 0xFF00) != (hi << 8))
		return 1;
	else
		return 0;
}

// relative addressing, only used in branch instructions. the program counter contains an 8-bit offset which is ORed with 0xFF00 (as the target must be in the range of -128 to 127 bytes of the current value)
uint8_t cpu::REL()
{
	addr_rel = read(pc);
	pc++;
	if (addr_rel & 0x80)
		addr_rel = addr_rel | 0xFF00;
	return 0;
}


// resets the cpu, self-explanatory. the absolute address is set to 0xFFFC as it contains a secondary address for the program counter to jump to. the programmer would usually set this address themselves
void cpu::reset() {
	accumulator = 0;
	xindex = 0;
	yindex = 0;
	sp = 0;
	sr = 0x00 | U;

	addr_abs = 0xFFFC;
	uint16_t lo = read(addr_abs + 0);
	uint16_t hi = read(addr_abs + 1);

	pc = (hi << 8) | lo;
	addr_rel = 0x0000;
	addr_abs = 0x0000;
	fetched = 0x00;

	cycles = 8; 
}
// only happens when the interrupt disable flag is not set. to preserve the current state of the processor, the program counter is pushed to the stack as well as the current status register 
// so that they can be restored by the RTI instruction when the interrupt is complete. the program counter is then set to a new fixed address which contains the typical routine for dealing with interrupts, starting at $FFFE
void cpu::interrupt() {
	if (GetFlag(ID) == 0) {

		pushtostack((pc >> 8) & 0x00FF);
		pushtostack(pc & 0x00FF);

		SetFlag(B, 0);
		SetFlag(U, 1);
		SetFlag(ID, 1);
		pushtostack(sr);
		addr_abs = 0xFFFE;
		uint16_t lo = read(addr_abs);
		uint16_t hi = read(addr_abs + 1);
		pc = (hi << 8) | lo;

		cycles = 7;

	}
}
// functions the same as a normal interrupt but will always occur, even if the interrupt disable flag is set. the routine for dealing with a non maskable interrupt is also held in $FFFA rather than $FFFE and it requires an extra cycle
void cpu::nminterrupt() {

		pushtostack((pc >> 8) & 0x00FF);
		pushtostack(pc & 0x00FF);

		SetFlag(B, 0);
		SetFlag(U, 1);
		SetFlag(ID, 1);
		pushtostack(sr);
		addr_abs = 0xFFFA;
		uint16_t lo = read(addr_abs);
		uint16_t hi = read(addr_abs + 1);
		pc = (hi << 8) | lo;

		cycles = 8;


}
// method to push to stack
void cpu::pushtostack(uint8_t data) {
	write(0x0100 + sp, data);
	sp--;
}
// method to pop from stack
uint8_t cpu::popfromstack() {
	sp++;
	auto data = read(0x0100 + sp); 
	return data;
}


// instructions

// fetches data from the absolute address in ram 
uint8_t cpu::fetch() {
	if (lookup[opcode].addressingmode != &cpu::IMP) {
		fetched = read(addr_abs);
	}
	return fetched;
}
// AND fetched data with accumulator. requires an extra cycle if a page boundary is crossed
uint8_t cpu::AND() {
	fetch();
	accumulator = accumulator & fetched;
	SetFlag(Z, accumulator == 0x00);
	SetFlag(N, accumulator & 0x80);
	return 1;
}
// clears carry flag 
uint8_t cpu::CLC() {
	SetFlag(C, false);
	return 0;
}
// add memory to accumulator with carry. must therefore have a variable which holds the sum of the accumulator, the fetched data and the status register containing the carry flag. 
// if the result is greater than 255 the carry bit is set as the maximum addressable range is 2^n-1 where n = 8 which is 255. however the 6502 has support for signed bits so overflow may occur in 
// certain circumstances. in this case, the overflow flag must be set, where O <- ~(A^M) & A^(A+M+C). This can be proven by a truth table holding the sign bit of each variable. 
// A  M  R | O | A^R | A^M |~(A^M) | 
// 0  0  0 | 0 |  0  |  0  |   1   |
// 0  0  1 | 1 |  1  |  0  |   1   |
// 0  1  0 | 0 |  0  |  1  |   0   |
// 0  1  1 | 0 |  1  |  1  |   0   |  so O = ~(A^M) & (A^R)
// 1  0  0 | 0 |  1  |  1  |   0   |
// 1  0  1 | 0 |  0  |  1  |   0   |
// 1  1  0 | 1 |  1  |  0  |   1   |
// 1  1  1 | 0 |  0  |  0  |   1   |
// Positive Number + Positive Number = Negative Result -> Overflow
// Negative Number + Negative Number = Positive Result -> Overflow
// Positive Number + Negative Number = Either Result -> No Overflow
// Positive Number + Positive Number = Positive Result -> No Overflow
// Negative Number + Negative Number = Negative Result -> No Overflow
// extra clock cycle may be required
uint8_t cpu::ADC() {
	fetch();
	auto temp = (uint16_t)accumulator + (uint16_t)fetched + (uint16_t)GetFlag(C);
	SetFlag(C, temp > 255);
	SetFlag(Z, (temp & 0x00FF) == 0);
	SetFlag(N, temp & 0x80);
	SetFlag(O, (~((uint16_t)accumulator ^ (uint16_t)fetched) & ((uint16_t)accumulator ^ (uint16_t)temp)) & 0x0080);
	accumulator = temp & 0x00FF;
	return 1;
}
// subtraction uses a similar thing to addition, but as the 6502 only contains a half adder there is no way to directly subtract one value from another. the second value must therefore be converted 
// into its negative counterpart and then added to the first. this is done using the XOR operation as it essentially performs twos complement. 
uint8_t cpu::SBC() {
	fetch();
	uint16_t value = ((uint16_t)fetched ^ 0000000011111111);
	auto temp = (uint16_t)accumulator + value + (uint16_t)GetFlag(C);
	SetFlag(C, temp & 0xFF00);
	SetFlag(Z, (temp & 0x00FF) == 0);
	SetFlag(N, temp & 0x0080);
	SetFlag(O, (temp ^ (uint16_t)accumulator) & (temp ^ value) & 0x0080);
	accumulator = temp & 0x00FF;
	return 1;

}
// restores cpu state to how it was before an interrupt occured, taking the status register and program counter from the stack 
uint8_t cpu::RTI() {
	sr = popfromstack();
	// clear B and unused flags
	sr = sr & ~B;
	sr = sr & ~U;

	sp++;
	pc = (uint16_t)read(0x0100 + sp);
	sp++;
	pc = pc | (uint16_t)read(0x0100 + sp) << 8;
	return 0;
}

// left shift operation
uint8_t cpu::ASL() {
	fetch();
	auto temp = (uint16_t)fetched << 1;
	SetFlag(C, (temp & 0xFF00) > 0);
	SetFlag(N, (temp & 0x800));
	SetFlag(Z, (temp & 0x00FF) == 0x00);
	// if implied addressing there is no address to write to, so the accumulator holds the result of the operation
	if (lookup[opcode].addressingmode == &cpu::IMP) {
		accumulator = temp & 0x00FF;
	}
	else {
		write(addr_abs, temp & 0x00FF);
	}
	return 0;
}
// branch if the carry bit is clear so that pc = address. at least 1 additional clock cycle will always be required, 2 if a page boundary is crossed
uint8_t cpu::BCC() {
	if (GetFlag(C) == 0) {
		cycles++;
		addr_abs = pc + addr_rel;
		if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
			cycles++;
		}
		pc = addr_abs;
	}
	return 0;
}
// branch if the carry bit is set
uint8_t cpu::BCS() {
	if (GetFlag(C) == 1) {
		cycles++;
		addr_abs = pc + addr_rel;
		if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
			cycles++;
		}
		pc = addr_abs;
	}
	return 0;
}
// branch if the zero flag is set
uint8_t cpu::BEQ() {
		if (GetFlag(Z) == 1) {
			cycles++;
			addr_abs = pc + addr_rel;
			if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
				cycles++;
			}
			pc = addr_abs;
		}
		return 0;
}
// test bits in memory with accumulator. bits 7 and 6 of operand are transferred to bit 7 and 6 of status register (N,O). the zero flag is set to the result of operand AND accumulator
uint8_t cpu::BIT() {
	fetch();
	auto temp = accumulator & fetched;
	SetFlag(Z, (temp & 0x00FF) == 0x00);
	SetFlag(N, fetched & (1 << 7));
	SetFlag(O, fetched & (1 << 6));
	return 0;
}
// branch if negative flag set
uint8_t cpu::BMI() {
	if (GetFlag(N) == 1) {
		cycles++;
		addr_abs = pc + addr_rel;
		if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
			cycles++;
		}
		pc = addr_abs;
	}
	return 0;
}
// branch if zero flag not set
uint8_t cpu::BNE() {
	if (GetFlag(Z) == 0) {
		cycles++;
		addr_abs = pc + addr_rel;
		if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
			cycles++;
		}
		pc = addr_abs;
	}
	return 0;
}
// branch if negative flag not set
uint8_t cpu::BPL() {
	if (GetFlag(N) == 0) {
		cycles++;
		addr_abs = pc + addr_rel;
		if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
			cycles++;
		}
		pc = addr_abs;
	}
	return 0;
}
// executes a break, similar to an interrupt. the status register is pushed to the stack with the break flag set to 1 and the return address being pushed to the stack is pc+2
uint8_t cpu::BRK() {
	pc++;

	SetFlag(ID, 1);
	pushtostack((pc >> 8) & 0x00FF);
	pushtostack(pc & 0x00FF);

	SetFlag(B, 1);
	pushtostack(sr);
	SetFlag(B, 0);

	pc = (uint16_t)read(0xFFFE) | ((uint16_t)read(0xFFFF) << 8);
	return 0;
}
// branch if overflow flag clear
uint8_t cpu::BVC() {
	if (GetFlag(O) == 0) {
		cycles++;
		addr_abs = pc + addr_rel;
		if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
			cycles++;
		}
		pc = addr_abs;
	}
	return 0;
}
// branch if overflow flag set 
uint8_t cpu::BVS() {
	if (GetFlag(O) == 1) {
		cycles++;
		addr_abs = pc + addr_rel;
		if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
			cycles++;
		}
		pc = addr_abs;
	}
	return 0;
}
// clear decimal flag
uint8_t cpu::CLD() {
	SetFlag(D, false);
	return 0;
}
// clear interrupt disable flag
uint8_t cpu::CLI() {
	SetFlag(ID, false);
	return 0;
}
// clear overflow flag
uint8_t cpu::CLV() {
	SetFlag(O, false);
	return 0;
}
// compare memory with accumulator
uint8_t cpu::CMP() {
	fetch();
	auto temp = (uint16_t)accumulator - (uint16_t)fetched;
	SetFlag(C, (accumulator >= fetched));
	SetFlag(N, (temp & 0x0080));
	SetFlag(Z, (temp & 0x00FF) == 0x0080);
	return 1;
}
// compare memory with x register
uint8_t cpu::CPX() {
	fetch();
	auto temp = (uint16_t)xindex - (uint16_t)fetched;
	SetFlag(C, (xindex >= fetched));
	SetFlag(N, (temp & 0x0080));
	SetFlag(Z, (temp & 0x00FF) == 0x0080);
	return 1;
}
// compare memory with y register
uint8_t cpu::CPY() {
	fetch();
	auto temp = (uint16_t)yindex - (uint16_t)fetched;
	SetFlag(C, (yindex >= fetched));
	SetFlag(N, (temp & 0x0080));
	SetFlag(Z, (temp & 0x00FF) == 0x0080);
	return 1;
}
// decrement memory by one
uint8_t cpu::DEC() {
	fetch();
	write(addr_abs, fetched - 1);
	SetFlag(N, (fetched - 1) & 0x80);
	SetFlag(Z, (fetched - 1) == 0x00);
	return 0;
}
// decrement x reg by 1
uint8_t cpu::DEX() {
	xindex -= 1;
	SetFlag(N, xindex & 0x80);
	SetFlag(Z, xindex == 0x00);
	return 0;
}
// decrement y reg by 1
uint8_t cpu::DEY() {
	yindex -= 1;
	SetFlag(N, yindex & 0x80);
	SetFlag(Z, yindex == 0x00);
	return 0;
}
// XOR memory with accumulator
uint8_t cpu::EOR() {
	fetch();
	auto temp = (fetched | accumulator) & (~(fetched & accumulator));
	SetFlag(N, temp & 0x80);
	SetFlag(Z, temp == 0x00);
	accumulator = temp;
	return 1;
}
// increment memory by one
uint8_t cpu::INC() {
	fetch();
	write(addr_abs, fetched + 1);
	SetFlag(N, (fetched + 1) & 0x80);
	SetFlag(Z, (fetched + 1) == 0x00);
	return 0;
}
// increment x reg by one
uint8_t cpu::INX() {
	xindex++;
	SetFlag(N, xindex & 0x80);
	SetFlag(Z, xindex == 0x00);
	return 0;
}
// increment y reg by one
uint8_t cpu::INY() {
	yindex++;
	SetFlag(N, yindex & 0x80);
	SetFlag(Z, yindex == 0x00);
	return 0;
}
// jump to new address
uint8_t cpu::JMP() {
	pc = addr_abs;
	return 0;
}
// jump to new address pushing the current address to the stack 
uint8_t cpu::JSR() {

	pc--;

	pushtostack((pc >> 8) & 0x00FF);
	pushtostack(pc & 0x00FF);

	pc = addr_abs;
	return 0;
}
// load accumulator with memory
uint8_t cpu::LDA() {

	fetch();
	accumulator = fetched;
	SetFlag(N, fetched & 0x80);
	SetFlag(Z, fetched == 0x00);
	return 1;

}
// load x reg with memory
uint8_t cpu::LDX() {

	fetch();
	xindex = fetched;
	SetFlag(N, fetched & 0x80);
	SetFlag(Z, fetched == 0x00);
	return 1;

}
// load y reg with memory
uint8_t cpu::LDY() {

	fetch();
	yindex = fetched;
	SetFlag(N, fetched & 0x80);
	SetFlag(Z, fetched == 0x00);
	return 1;

}
// shift right 
uint8_t cpu::LSR() {
	fetch();
	SetFlag(C, fetched & 0x0001);
	auto temp = fetched >> 1;
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, false);
	if (lookup[opcode].addressingmode == &cpu::IMP)
		accumulator = temp & 0x00FF;
	else
		write(addr_abs, temp & 0x00FF);
	return 0;
}
// no operation
uint8_t cpu::NOP() {
	return 0;
}
// or memory with accumulator
uint8_t cpu::ORA() {
	fetch();
	auto temp = fetched | accumulator;
	SetFlag(N, (temp & 0x80));
	SetFlag(Z, (temp == 0x00));
	accumulator = temp;
	return 1;
}
// push accumulator on to stack
uint8_t cpu::PHA() {
	pushtostack(accumulator);
	return 0;
}
// push processor status on to stack with break flag and unused flag set to 1 
uint8_t cpu::PHP() {
	SetFlag(B, true);
	SetFlag(U, true);
	pushtostack(sr);
	return 0;
}
// pop accumulator from stack
uint8_t cpu::PLA() {
	accumulator = popfromstack();
	SetFlag(N, accumulator & 0x80);
	SetFlag(Z, accumulator == 0x00);
	return 0;
}
// pop status register from stack 
uint8_t cpu::PLP() {
	sr = popfromstack();
	return 0;
}
// rotate one bit left. this means the left most bit will go to the front of the binary number
uint8_t cpu::ROL() {
	fetch();
	auto temp = (uint16_t)fetched << 1 | GetFlag(C);
	SetFlag(C, temp & 0xFF00);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	if (lookup[opcode].addressingmode == &cpu::IMP) {
		accumulator = temp & 0x00FF;
	}
	else {
		write(addr_abs, temp & 0x00FF);
	}
	return 0;
	
}
// rotate one bit right 
uint8_t cpu::ROR() {
	fetch();
	auto temp = (uint16_t)(GetFlag(C) << 7) | (fetched >> 1);
	SetFlag(C, fetched & 0x01);
	SetFlag(Z, (temp & 0x00FF) == 0x00);
	SetFlag(N, temp & 0x0080);
	if (lookup[opcode].addressingmode == &cpu::IMP)
		accumulator = temp & 0x00FF;
	else
		write(addr_abs, temp & 0x00FF);
	return 0;
}
// return from subroutine
uint8_t cpu::RTS() {
	auto temp = popfromstack();
	temp++;
	pc = temp;
	return 0;
}
// set carry flag 
uint8_t cpu::SEC() {
	SetFlag(C, true);
	return 0;
}
// set decimal flag
uint8_t cpu::SED() {
	SetFlag(D, true);
	return 0;
}
// set interrupt disable flag
uint8_t cpu::SEI() {
	SetFlag(ID, true);
	return 0;
}
// store accumulator in memory
uint8_t cpu::STA() {
	write(addr_abs, accumulator);
	return 0;
}
// store x register in memory
uint8_t cpu::STX() {
	write(addr_abs, xindex);
	return 0;
}
// store y register in memory
uint8_t cpu::STY() {
	write(addr_abs, yindex);
	return 0;
}
// transfer accumulator to x register
uint8_t cpu::TAX() {
	xindex = accumulator;
	SetFlag(N, xindex & 0x80);
	SetFlag(Z, xindex == 0x00);
	return 0;
}
// transfer accumulator to y register
uint8_t cpu::TAY() {
	yindex = accumulator;
	SetFlag(N, yindex & 0x80);
	SetFlag(Z, yindex == 0x00);
	return 0;
}
// transfer stack pointer to x register
uint8_t cpu::TSX() {
	xindex = sp;
	SetFlag(N, yindex & 0x80);
	SetFlag(Z, yindex == 0x00);
	return 0;
}
// transfer accumulator to x register
uint8_t cpu::TXA() {
	accumulator = xindex;
	SetFlag(N, accumulator & 0x80);
	SetFlag(Z, accumulator == 0x00);
	return 0;
}
// transfer x register to stack pointer
uint8_t cpu::TXS() {
	sp = xindex;
	return 0;
}
// transfer y register to accumulator
uint8_t cpu::TYA() {
	accumulator = yindex;
	SetFlag(N, accumulator & 0x80);
	SetFlag(Z, accumulator == 0x00);
	return 0;
}
// catches all illegal opcodes
uint8_t cpu::XXX() {
	return 0;
}

// check if there are no more cycles left 
bool cpu::complete()
{
	if (cycles == 0) return true;
	else return false;
}

// this function is copy and pasted as i have no clue how to perform disassembly 
std::map<uint16_t, std::string> cpu::disassemble(uint16_t nStart, uint16_t nStop)
{
	uint32_t addr = nStart;
	uint8_t value = 0x00, lo = 0x00, hi = 0x00;
	std::map<uint16_t, std::string> mapLines;
	uint16_t line_addr = 0;

	auto hex = [](uint32_t n, uint8_t d)
	{
		std::string s(d, '0');
		for (int i = d - 1; i >= 0; i--, n >>= 4)
			s[i] = "0123456789ABCDEF"[n & 0xF];
		return s;
	};

	while (addr <= (uint32_t)nStop)
	{
		line_addr = addr;

		std::string sInst = "$" + hex(addr, 4) + ": ";

		uint8_t opcode = bus->read(addr, true); addr++;
		sInst += lookup[opcode].name + " ";

		if (lookup[opcode].addressingmode == &cpu::IMP)
		{
			sInst += " {IMP}";
		}
		else if (lookup[opcode].addressingmode == &cpu::IMM)
		{
			value = bus->read(addr, true); addr++;
			sInst += "#$" + hex(value, 2) + " {IMM}";
		}
		else if (lookup[opcode].addressingmode == &cpu::ZP0)
		{
			lo = bus->read(addr, true); addr++;
			hi = 0x00;
			sInst += "$" + hex(lo, 2) + " {ZP0}";
		}
		else if (lookup[opcode].addressingmode == &cpu::ZPX)
		{
			lo = bus->read(addr, true); addr++;
			hi = 0x00;
			sInst += "$" + hex(lo, 2) + ", X {ZPX}";
		}
		else if (lookup[opcode].addressingmode == &cpu::ZPY)
		{
			lo = bus->read(addr, true); addr++;
			hi = 0x00;
			sInst += "$" + hex(lo, 2) + ", Y {ZPY}";
		}
		else if (lookup[opcode].addressingmode == &cpu::IZX)
		{
			lo = bus->read(addr, true); addr++;
			hi = 0x00;
			sInst += "($" + hex(lo, 2) + ", X) {IZX}";
		}
		else if (lookup[opcode].addressingmode == &cpu::IZY)
		{
			lo = bus->read(addr, true); addr++;
			hi = 0x00;
			sInst += "($" + hex(lo, 2) + "), Y {IZY}";
		}
		else if (lookup[opcode].addressingmode == &cpu::ABS)
		{
			lo = bus->read(addr, true); addr++;
			hi = bus->read(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + " {ABS}";
		}
		else if (lookup[opcode].addressingmode == &cpu::ABX)
		{
			lo = bus->read(addr, true); addr++;
			hi = bus->read(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", X {ABX}";
		}
		else if (lookup[opcode].addressingmode == &cpu::ABY)
		{
			lo = bus->read(addr, true); addr++;
			hi = bus->read(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", Y {ABY}";
		}
		else if (lookup[opcode].addressingmode == &cpu::IND)
		{
			lo = bus->read(addr, true); addr++;
			hi = bus->read(addr, true); addr++;
			sInst += "($" + hex((uint16_t)(hi << 8) | lo, 4) + ") {IND}";
		}
		else if (lookup[opcode].addressingmode == &cpu::REL)
		{
			value = bus->read(addr, true); addr++;
			sInst += "$" + hex(value, 2) + " [$" + hex(addr + (int8_t)value, 4) + "] {REL}";
		}

		mapLines[line_addr] = sInst;
	}

	return mapLines;
}


