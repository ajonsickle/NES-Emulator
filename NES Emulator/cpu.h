#pragma once
#include <string>
#include <vector>
class bus;

class cpu
{
public:
	cpu();

	void connectBus(bus *x) {
		bus = x;
	}
	void clock();
	void reset();
	void interrupt();
	void nminterrupt();
	void pushtostack(uint8_t data);
	uint8_t popfromstack();

	enum statusflagslist {
		C = (1 << 0),
		Z = (1 << 1),
		ID = (1 << 2),
		D = (1 << 3),
		B = (1 << 4),
		U = (1 << 5),
		O = (1 << 6),
		N = (1 << 7)
	};

	uint8_t accumulator = 0x00;
	uint8_t xindex = 0x00;
	uint8_t yindex = 0x00;
	uint16_t pc = 0x0000;
	uint8_t sp = 0x00;
	uint8_t sr = 0x00;
	
private:
	//opcodes


	uint8_t BRK();
	uint8_t DEX();
	uint8_t CLC();
	uint8_t ORA();
	uint8_t STP();
	uint8_t SLO();
	uint8_t NOP();
	uint8_t ASL();
	uint8_t PHP();
	uint8_t ANC();
	uint8_t BPL();
	uint8_t JSR();
	uint8_t AND();
	uint8_t RLA();
	uint8_t BIT();
	uint8_t ROL();
	uint8_t PLP();
	uint8_t BMI();
	uint8_t SEC();
	uint8_t RTI();
	uint8_t EOR();
	uint8_t SRE();
	uint8_t LSR();
	uint8_t PHA();
	uint8_t ALR();
	uint8_t JMP();
	uint8_t BVC();
	uint8_t CLI();
	uint8_t TSX();
	uint8_t RTS();
	uint8_t ADC();
	uint8_t RRA();
	uint8_t ROR();
	uint8_t ARR();
	uint8_t BVS();
	uint8_t SEI();
	uint8_t STA();
	uint8_t SAX();
	uint8_t STY();
	uint8_t STX();
	uint8_t DEY();
	uint8_t TXS();
	uint8_t TXA();
	uint8_t XAA();
	uint8_t BCC();
	uint8_t AHX();
	uint8_t TYA();
	uint8_t TAS();
	uint8_t SHY();
	uint8_t LDY();
	uint8_t LDA();
	uint8_t LDX();
	uint8_t LAX();
	uint8_t TAY();
	uint8_t TAX();
	uint8_t BCS();
	uint8_t CLV();
	uint8_t LAS();
	uint8_t CPY();
	uint8_t CMP();
	uint8_t DCP();
	uint8_t DEC();
	uint8_t INY();
	uint8_t AXS();
	uint8_t BNE();
	uint8_t CLD();
	uint8_t CPX();
	uint8_t ISC();
	uint8_t INC();
	uint8_t INX();
	uint8_t BEQ();
	uint8_t SBC();
	uint8_t SED();
	uint8_t PLA();

	uint8_t XXX();
	
private:
// addressing modes
	uint8_t IMP();	
	uint8_t IMM();
	uint8_t ZP0();	
	uint8_t ZPX();
	uint8_t ZPY();	
	uint8_t REL();
	uint8_t ABS();	
	uint8_t ABX();
	uint8_t ABY();	
	uint8_t IND();
	uint8_t IZX();	
	uint8_t IZY();


private:
	bus* bus = nullptr;
	uint8_t read(uint16_t address);
	void write(uint16_t address, uint8_t data);
	uint8_t GetFlag(statusflagslist f);
	void SetFlag(statusflagslist f, bool v);

	uint8_t fetch();
	uint8_t fetched = 0x00;
	uint8_t opcode = 0x00;
	uint8_t cycles = 0x00;
	uint16_t addr_abs = 0x0000;
	uint16_t addr_rel = 0x0000;

	struct INSTRUCTION {
		std::string name;
		uint8_t(cpu::* operate)(void) = nullptr;
		uint8_t(cpu::* addressingmode)(void) = nullptr;
		uint8_t cycles = 0;
	};

	std::vector<INSTRUCTION> lookup;
};




