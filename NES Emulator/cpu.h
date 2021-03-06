#pragma once

#include <vector>

#include <string>
#include <map>

// forward declaration of bus 
class bus;

class cpu
{
public:
	// cpu constructor
	cpu();

	// creates instance of bus class
	void connectBus(bus* n) {
		bus = n;
	}
	// performs one cpu clock cycle
	void clock();
	// resets the cpu
	void reset();
	// executes an interrupt
	void interrupt();
	// executes a non-maskable interrupt (one which cannot be disabled or ignored)
	void nminterrupt();
	// methods to push and pop from stack
	void pushtostack(uint8_t data);
	uint8_t popfromstack();
	// check if all cpu cycles have been carried out
	bool complete();

	// list of every sr flag in the CPU (https://wiki.nesdev.org/w/index.php/sr_flags)
	/*
	Carry Bit (C): Set if there was a carry bit from an arithmetic instruction
	Zero Bit (Z): Set if the result of an instruction is zero
	Interrupt Disable (ID): Set if interrupts besides non-maskable ones are disabled
	Decimal (D): Set if an instruction returns a decimal value
	B: No effect
	Unused (U): No effect (added so the flag list is 8bit)
	Overflow (O): Set if an integer overflow occurs from an instruction
	Negative (N): Set if an instruction returns a negative value
	*/

	uint8_t srflagslist[8] = {0b00000001, 0b00000010, 0b00000100, 0b00001000, 0b00010000, 0b00100000, 0b01000000, 0b10000000};

	// create variables for the accumulator, x register, y register, program counter, stack pointer and sr register
	uint8_t accumulator = 0;
	uint8_t xindex = 0;
	uint8_t yindex = 0;
	uint16_t pc = 0;
	uint8_t sp = 0;
	uint8_t sr = 0;

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
	uint8_t LSR();
	uint8_t PHA();
	uint8_t JMP();
	uint8_t BVC();
	uint8_t CLI();
	uint8_t TSX();
	uint8_t RTS();
	uint8_t ADC();
	uint8_t ROR();
	uint8_t BVS();
	uint8_t SEI();
	uint8_t STA();
	uint8_t STY();
	uint8_t STX();
	uint8_t DEY();
	uint8_t TXS();
	uint8_t TXA();
	uint8_t BCC();
	uint8_t TYA();
	uint8_t LDY();
	uint8_t LDA();
	uint8_t LDX();
	uint8_t TAY();
	uint8_t TAX();
	uint8_t BCS();
	uint8_t CLV();
	uint8_t CPY();
	uint8_t CMP();
	uint8_t DEC();
	uint8_t INY();
	uint8_t BNE();
	uint8_t CLD();
	uint8_t CPX();
	uint8_t INC();
	uint8_t INX();
	uint8_t BEQ();
	uint8_t SBC();
	uint8_t SED();
	uint8_t PLA();

	uint8_t XXX();

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

	// instance of bus 
	bus* bus = nullptr;
	// cpu read and write methods
	uint8_t read(uint16_t address);
	void write(uint16_t address, uint8_t data);
	// get and set methods for flag list
	uint8_t GetStatusFlag(char f);
	void SetStatusFlag(char f, bool v);

	// fetch data from memory when it is needed for an instruction
	uint8_t fetch();
	// result of fetch() method
	uint8_t fetched = 0;
	// holder for opcode of the next instruction to be carried out
	uint8_t opcode = 0;
	// shows how many clock cycles are left for the cpu to execute
	uint8_t cycles = 0;
	// addr_abs holds the address that will need to be accessed to get data for an instruction. addr_rel is only used for relative addressing
	uint16_t addr_abs = 0;
	uint16_t addr_rel = 0;

	bool implied(uint8_t opcode);

};
