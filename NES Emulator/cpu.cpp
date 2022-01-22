#include "cpu.h"
#include <cstdint>
#include "bus.h"
#include <map>
cpu::cpu() {
	using a = cpu;
	lookup =
	{
		{ "BRK", &a::BRK, &a::IMM, 7 },{ "ORA", &a::ORA, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::ZP0, 3 },{ "ASL", &a::ASL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHP", &a::PHP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::IMM, 2 },{ "ASL", &a::ASL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABS, 4 },{ "ASL", &a::ASL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BPL", &a::BPL, &a::REL, 2 },{ "ORA", &a::ORA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ZPX, 4 },{ "ASL", &a::ASL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLC", &a::CLC, &a::IMP, 2 },{ "ORA", &a::ORA, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABX, 4 },{ "ASL", &a::ASL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "JSR", &a::JSR, &a::ABS, 6 },{ "AND", &a::AND, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "BIT", &a::BIT, &a::ZP0, 3 },{ "AND", &a::AND, &a::ZP0, 3 },{ "ROL", &a::ROL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLP", &a::PLP, &a::IMP, 4 },{ "AND", &a::AND, &a::IMM, 2 },{ "ROL", &a::ROL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "BIT", &a::BIT, &a::ABS, 4 },{ "AND", &a::AND, &a::ABS, 4 },{ "ROL", &a::ROL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BMI", &a::BMI, &a::REL, 2 },{ "AND", &a::AND, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ZPX, 4 },{ "ROL", &a::ROL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEC", &a::SEC, &a::IMP, 2 },{ "AND", &a::AND, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ABX, 4 },{ "ROL", &a::ROL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTI", &a::RTI, &a::IMP, 6 },{ "EOR", &a::EOR, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "EOR", &a::EOR, &a::ZP0, 3 },{ "LSR", &a::LSR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHA", &a::PHA, &a::IMP, 3 },{ "EOR", &a::EOR, &a::IMM, 2 },{ "LSR", &a::LSR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::ABS, 3 },{ "EOR", &a::EOR, &a::ABS, 4 },{ "LSR", &a::LSR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVC", &a::BVC, &a::REL, 2 },{ "EOR", &a::EOR, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ZPX, 4 },{ "LSR", &a::LSR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLI", &a::CLI, &a::IMP, 2 },{ "EOR", &a::EOR, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ABX, 4 },{ "LSR", &a::LSR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTS", &a::RTS, &a::IMP, 6 },{ "ADC", &a::ADC, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ADC", &a::ADC, &a::ZP0, 3 },{ "ROR", &a::ROR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLA", &a::PLA, &a::IMP, 4 },{ "ADC", &a::ADC, &a::IMM, 2 },{ "ROR", &a::ROR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::IND, 5 },{ "ADC", &a::ADC, &a::ABS, 4 },{ "ROR", &a::ROR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVS", &a::BVS, &a::REL, 2 },{ "ADC", &a::ADC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ZPX, 4 },{ "ROR", &a::ROR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEI", &a::SEI, &a::IMP, 2 },{ "ADC", &a::ADC, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ABX, 4 },{ "ROR", &a::ROR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "???", &a::NOP, &a::IMP, 2 },{ "STA", &a::STA, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZP0, 3 },{ "STA", &a::STA, &a::ZP0, 3 },{ "STX", &a::STX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "DEY", &a::DEY, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 2 },{ "TXA", &a::TXA, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "STY", &a::STY, &a::ABS, 4 },{ "STA", &a::STA, &a::ABS, 4 },{ "STX", &a::STX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCC", &a::BCC, &a::REL, 2 },{ "STA", &a::STA, &a::IZY, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZPX, 4 },{ "STA", &a::STA, &a::ZPX, 4 },{ "STX", &a::STX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "TYA", &a::TYA, &a::IMP, 2 },{ "STA", &a::STA, &a::ABY, 5 },{ "TXS", &a::TXS, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::NOP, &a::IMP, 5 },{ "STA", &a::STA, &a::ABX, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::XXX, &a::IMP, 5 },
		{ "LDY", &a::LDY, &a::IMM, 2 },{ "LDA", &a::LDA, &a::IZX, 6 },{ "LDX", &a::LDX, &a::IMM, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "LDY", &a::LDY, &a::ZP0, 3 },{ "LDA", &a::LDA, &a::ZP0, 3 },{ "LDX", &a::LDX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "TAY", &a::TAY, &a::IMP, 2 },{ "LDA", &a::LDA, &a::IMM, 2 },{ "TAX", &a::TAX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "LDY", &a::LDY, &a::ABS, 4 },{ "LDA", &a::LDA, &a::ABS, 4 },{ "LDX", &a::LDX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCS", &a::BCS, &a::REL, 2 },{ "LDA", &a::LDA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "LDY", &a::LDY, &a::ZPX, 4 },{ "LDA", &a::LDA, &a::ZPX, 4 },{ "LDX", &a::LDX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "CLV", &a::CLV, &a::IMP, 2 },{ "LDA", &a::LDA, &a::ABY, 4 },{ "TSX", &a::TSX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 4 },{ "LDY", &a::LDY, &a::ABX, 4 },{ "LDA", &a::LDA, &a::ABX, 4 },{ "LDX", &a::LDX, &a::ABY, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "CPY", &a::CPY, &a::IMM, 2 },{ "CMP", &a::CMP, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPY", &a::CPY, &a::ZP0, 3 },{ "CMP", &a::CMP, &a::ZP0, 3 },{ "DEC", &a::DEC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INY", &a::INY, &a::IMP, 2 },{ "CMP", &a::CMP, &a::IMM, 2 },{ "DEX", &a::DEX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "CPY", &a::CPY, &a::ABS, 4 },{ "CMP", &a::CMP, &a::ABS, 4 },{ "DEC", &a::DEC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BNE", &a::BNE, &a::REL, 2 },{ "CMP", &a::CMP, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ZPX, 4 },{ "DEC", &a::DEC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLD", &a::CLD, &a::IMP, 2 },{ "CMP", &a::CMP, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ABX, 4 },{ "DEC", &a::DEC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "CPX", &a::CPX, &a::IMM, 2 },{ "SBC", &a::SBC, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPX", &a::CPX, &a::ZP0, 3 },{ "SBC", &a::SBC, &a::ZP0, 3 },{ "INC", &a::INC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INX", &a::INX, &a::IMP, 2 },{ "SBC", &a::SBC, &a::IMM, 2 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::SBC, &a::IMP, 2 },{ "CPX", &a::CPX, &a::ABS, 4 },{ "SBC", &a::SBC, &a::ABS, 4 },{ "INC", &a::INC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BEQ", &a::BEQ, &a::REL, 2 },{ "SBC", &a::SBC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ZPX, 4 },{ "INC", &a::INC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SED", &a::SED, &a::IMP, 2 },{ "SBC", &a::SBC, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ABX, 4 },{ "INC", &a::INC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
	};
}

void cpu::write(uint16_t address, uint8_t data) {
	bus->write(address, data);
};
uint8_t cpu::read(uint16_t address) {
	return bus->read(address, false);
}
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
uint8_t cpu::GetFlag(statusflagslist f)
{
	return ((sr & f) > 0) ? 1 : 0;
}

void cpu::SetFlag(statusflagslist f, bool v)
{
	if (v)
		sr |= f;
	else
		sr &= ~f;
}

//addressing modes 
uint8_t cpu::IMP() {
	fetched = accumulator;
	return 0;
}

uint8_t cpu::IMM() {
	addr_abs = pc++;
	return 0;
}

uint8_t cpu::ZP0() {
	addr_abs = read(pc);
	pc++;
	addr_abs &= 0x00FF;
	return 0;
}

uint8_t cpu::ZPX() {
	addr_abs = (read(pc) + xindex);
	pc++;
	addr_abs &= 0x00FF;
	return 0;
}

uint8_t cpu::ZPY() {
	addr_abs = (read(pc) + yindex);
	pc++;
	addr_abs &= 0x00FF;
	return 0;
}

uint8_t cpu::ABS() {
	uint16_t lo = read(pc);
	pc++;
	uint16_t hi = read(pc);
	pc++;

	addr_abs = (hi << 8) | lo;
	return 0;
}

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

uint8_t cpu::IND() {
	uint16_t ptr_lo = read(pc);
	pc++;
	uint16_t ptr_hi = read(pc);
	pc++;

	uint16_t ptr = (ptr_hi << 8) | ptr_lo;

	if (ptr_lo == 0x00FF)
	{
		addr_abs = (read(ptr & 0xFF00) << 8) | read(ptr + 0);
	}
	else 
	{
		addr_abs = (read(ptr + 1) << 8) | read(ptr + 0);
	}

	return 0;
}

uint8_t cpu::IZX() {
	uint16_t t = read(pc);
	pc++;

	uint16_t lo = read((uint16_t)(t + (uint16_t)xindex) & 0x00FF);
	uint16_t hi = read((uint16_t)(t + (uint16_t)xindex + 1) & 0x00FF);

	addr_abs = (hi << 8) | lo;

	return 0;
}
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

uint8_t cpu::REL()
{
	addr_rel = read(pc);
	pc++;
	if (addr_rel & 0x80)
		addr_rel |= 0xFF00;
	return 0;
}

// instructions

uint8_t cpu::fetch() {
	if (lookup[opcode].addressingmode != &cpu::IMP) {
		fetched = read(addr_abs);
	}
	return fetched;
}

uint8_t cpu::AND() {
	fetch();
	accumulator = accumulator & fetched;
	SetFlag(Z, accumulator == 0x00);
	SetFlag(N, accumulator & 0x80);
	return 1;
}

uint8_t cpu::CLC() {
	SetFlag(C, false);
	return 0;
}

uint8_t cpu::ADC() {
	fetch();
	uint16_t temp = (uint16_t)accumulator + (uint16_t)fetched + (uint16_t)GetFlag(C);
	SetFlag(C, temp > 255);
	SetFlag(Z, (temp & 0x00FF) == 0);
	SetFlag(N, temp & 0x80);
	SetFlag(O, (~((uint16_t)accumulator ^ (uint16_t)fetched) & ((uint16_t)accumulator ^ (uint16_t)temp)) & 0x0080);
	accumulator = temp & 0x00FF;
	return 1;
}

uint8_t cpu::SBC() {
	fetch();
	uint16_t value = ((uint16_t)fetched ^ 0x00FF);
	uint16_t temp = (uint16_t)accumulator + value + (uint16_t)GetFlag(C);
	SetFlag(C, temp & 0xFF00);
	SetFlag(Z, (temp & 0x00FF) == 0);
	SetFlag(N, temp & 0x0080);
	SetFlag(O, (temp ^ (uint16_t)accumulator) & (temp ^ value) & 0x0080);
	accumulator = temp & 0x00FF;
	return 1;

}

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

void cpu::interrupt() {
	if (GetFlag(ID) == 0) {

		pushtostack((pc >> 8) & 0x00FF);
		pushtostack(pc & 0x00FF);

		SetFlag(B, 0);
		SetFlag(U, 1);
		SetFlag(ID, 1);
		pushtostack(sr);
		addr_abs = 0xFFFE;
		uint16_t lo = read(addr_abs + 0);
		uint16_t hi = read(addr_abs + 1);
		pc = (hi << 8) | lo;

		cycles = 7;

	}
}
void cpu::nminterrupt() {

		pushtostack((pc >> 8) & 0x00FF);
		pushtostack(pc & 0x00FF);

		SetFlag(B, 0);
		SetFlag(U, 1);
		SetFlag(ID, 1);
		pushtostack(sr);
		addr_abs = 0xFFFA;
		uint16_t lo = read(addr_abs + 0);
		uint16_t hi = read(addr_abs + 1);
		pc = (hi << 8) | lo;

		cycles = 8;


}

void cpu::pushtostack(uint8_t data) {
	write(0x0100 + sp, data);
	sp--;
}

uint8_t cpu::popfromstack() {
	sp++;
	auto data = read(0x0100 + sp); 
	return data;
}

uint8_t cpu::RTI() {
	sp++;
	sr = read(0x0100 + sp);
	sr &= ~B;
	sr &= ~U;
	sp++;
	pc = (uint16_t)read(0x0100 + sp);
	sp++;
	pc |= (uint16_t)read(0x0100 + sp) << 8;
	return 0;
}

uint8_t cpu::ASL() {
	fetch();
	auto temp = (uint16_t)fetched << 1;
	SetFlag(C, (temp & 0xFF00) > 0);
	SetFlag(N, (temp & 0x800));
	SetFlag(Z, (temp & 0x00FF) == 0x00);
	if (lookup[opcode].addressingmode == &cpu::IMP) {
		accumulator = temp & 0x00FF;
	}
	else {
		write(addr_abs, temp & 0x00FF);
	}
	return 0;
}

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

uint8_t cpu::BIT() {
	fetch();
	auto temp = accumulator & fetched;
	SetFlag(Z, (temp & 0x00FF) == 0x00);
	SetFlag(N, fetched & (1 << 7));
	SetFlag(O, fetched & (1 << 6));
	return 0;
}

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

uint8_t cpu::CLD() {
	SetFlag(D, false);
	return 0;
}

uint8_t cpu::CLI() {
	SetFlag(ID, false);
	return 0;
}

uint8_t cpu::CLV() {
	SetFlag(O, false);
	return 0;
}

uint8_t cpu::CMP() {
	fetch();
	auto temp = (uint16_t)accumulator - (uint16_t)fetched;
	SetFlag(C, (accumulator >= fetched));
	SetFlag(N, (temp & 0x0080));
	SetFlag(Z, (temp & 0x00FF) == 0x0080);
	return 1;
}

uint8_t cpu::CPX() {
	fetch();
	auto temp = (uint16_t)xindex - (uint16_t)fetched;
	SetFlag(C, (xindex >= fetched));
	SetFlag(N, (temp & 0x0080));
	SetFlag(Z, (temp & 0x00FF) == 0x0080);
	return 1;
}

uint8_t cpu::CPY() {
	fetch();
	auto temp = (uint16_t)yindex - (uint16_t)fetched;
	SetFlag(C, (yindex >= fetched));
	SetFlag(N, (temp & 0x0080));
	SetFlag(Z, (temp & 0x00FF) == 0x0080);
	return 1;
}

uint8_t cpu::DEC() {
	fetch();
	write(addr_abs, fetched - 1);
	SetFlag(N, (fetched - 1) & 0x80);
	SetFlag(Z, (fetched - 1) == 0x00);
	return 0;
}

uint8_t cpu::DEX() {
	xindex -= 1;
	SetFlag(N, xindex & 0x80);
	SetFlag(Z, xindex == 0x00);
	return 0;
}

uint8_t cpu::DEY() {
	yindex -= 1;
	SetFlag(N, yindex & 0x80);
	SetFlag(Z, yindex == 0x00);
	return 0;
}

uint8_t cpu::EOR() {
	fetch();
	auto temp = (fetched | accumulator) & (~(fetched & accumulator));
	SetFlag(N, temp & 0x80);
	SetFlag(Z, temp == 0x00);
	return 1;
}

uint8_t cpu::INC() {
	fetch();
	write(addr_abs, fetched + 1);
	SetFlag(N, (fetched + 1) & 0x80);
	SetFlag(Z, (fetched + 1) == 0x00);
	return 0;
}

uint8_t cpu::INX() {
	xindex++;
	SetFlag(N, xindex & 0x80);
	SetFlag(Z, xindex == 0x00);
	return 0;
}

uint8_t cpu::INY() {
	yindex++;
	SetFlag(N, yindex & 0x80);
	SetFlag(Z, yindex == 0x00);
	return 0;
}

uint8_t cpu::JMP() {
	pc = addr_abs;
	return 0;
}

uint8_t cpu::JSR() {

	pc--;

	pushtostack((pc >> 8) & 0x00FF);
	pushtostack(pc & 0x00FF);

	pc = addr_abs;
	return 0;
}

uint8_t cpu::LDA() {

	fetch();
	accumulator = fetched;
	SetFlag(N, fetched & 0x80);
	SetFlag(Z, fetched == 0x00);
	return 1;

}

uint8_t cpu::LDX() {

	fetch();
	xindex = fetched;
	SetFlag(N, fetched & 0x80);
	SetFlag(Z, fetched == 0x00);
	return 1;

}

uint8_t cpu::LDY() {

	fetch();
	yindex = fetched;
	SetFlag(N, fetched & 0x80);
	SetFlag(Z, fetched == 0x00);
	return 1;

}

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

uint8_t cpu::NOP() {
	return 0;
}

uint8_t cpu::ORA() {
	fetch();
	auto temp = fetched | accumulator;
	SetFlag(N, (temp & 0x80));
	SetFlag(Z, (temp == 0x00));
	accumulator = temp;
	return 1;
}

uint8_t cpu::PHA() {
	pushtostack(accumulator);
	return 0;
}

uint8_t cpu::PHP() {
	SetFlag(B, true);
	SetFlag(U, true);
	pushtostack(sr);
	return 0;
}

uint8_t cpu::PLA() {
	accumulator = popfromstack();
	SetFlag(N, accumulator & 0x80);
	SetFlag(Z, accumulator == 0x00);
	return 0;
}

uint8_t cpu::PLP() {
	sr = popfromstack();
	return 0;
}

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

uint8_t cpu::RTS() {
	auto temp = popfromstack();
	temp++;
	pc = temp;
	return 0;
}

uint8_t cpu::SEC() {
	SetFlag(C, true);
	return 0;
}

uint8_t cpu::SED() {
	SetFlag(D, true);
	return 0;
}

uint8_t cpu::SEI() {
	SetFlag(ID, true);
	return 0;
}

uint8_t cpu::STA() {
	write(addr_abs, accumulator);
	return 0;
}

uint8_t cpu::STX() {
	write(addr_abs, xindex);
	return 0;
}

uint8_t cpu::STY() {
	write(addr_abs, yindex);
	return 0;
}

uint8_t cpu::TAX() {
	xindex = accumulator;
	SetFlag(N, xindex & 0x80);
	SetFlag(Z, xindex == 0x00);
	return 0;
}

uint8_t cpu::TAY() {
	yindex = accumulator;
	SetFlag(N, yindex & 0x80);
	SetFlag(Z, yindex == 0x00);
	return 0;
}

uint8_t cpu::TSX() {
	xindex = sp;
	SetFlag(N, yindex & 0x80);
	SetFlag(Z, yindex == 0x00);
	return 0;
}

uint8_t cpu::TXA() {
	accumulator = xindex;
	SetFlag(N, accumulator & 0x80);
	SetFlag(Z, accumulator == 0x00);
	return 0;
}

uint8_t cpu::TXS() {
	sp = xindex;
	return 0;
}

uint8_t cpu::TYA() {
	accumulator = yindex;
	SetFlag(N, accumulator & 0x80);
	SetFlag(Z, accumulator == 0x00);
	return 0;
}

uint8_t cpu::XXX() {
	return 0;
}

bool cpu::complete()
{
	return cycles == 0;
}

// this function is copy and pasted
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
			sInst += "$" + hex(value, 2) + " [$" + hex(addr + value, 4) + "] {REL}";
		}

		mapLines[line_addr] = sInst;
	}

	return mapLines;
}


