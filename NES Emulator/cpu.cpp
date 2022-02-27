#include "cpu.h"
#include "bus.h"

// Constructor
cpu::cpu()
{

}

// calls the bus write function, writes a byte of data to the specified address in ram 
void cpu::write(uint16_t address, uint8_t data) {
	bus->write(address, data);
};
// calls bus read function, reads a byte of data from the address specified in ram
uint8_t cpu::read(uint16_t address) {
	return bus->read(address);
}


// method to check whether the current opcode uses implied addressing or not 
bool cpu::implied(uint8_t opcode) {
	if (opcode == 0 || opcode == 2 || opcode == 3 || opcode == 4 || opcode == 7 || opcode == 8 || opcode == 10 || opcode == 11 || opcode == 12 || opcode == 15 || opcode == 18 || opcode == 19 || opcode == 20 || opcode == 23 || opcode == 24 || opcode == 26 || opcode == 27 || opcode == 28
		|| opcode == 31 || opcode == 34 || opcode == 35 || opcode == 39 || opcode == 40 || opcode == 42 || opcode == 43 || opcode == 47 || opcode == 50 || opcode == 51 || opcode == 52 || opcode == 55 || opcode == 56 || opcode == 58 || opcode == 59 || opcode == 60 || opcode == 63
		|| opcode == 64 || opcode == 66 || opcode == 67 || opcode == 68 || opcode == 71 || opcode == 72 || opcode == 74 || opcode == 75 || opcode == 79 || opcode == 82 || opcode == 83 || opcode == 84 || opcode == 87 || opcode == 88 || opcode == 90 || opcode == 91
		|| opcode == 92 || opcode == 95 || opcode == 96 || opcode == 98 || opcode == 99 || opcode == 100 || opcode == 103 || opcode == 104 || opcode == 106 || opcode == 107 || opcode == 111 || opcode == 114 || opcode == 115 || opcode == 116 || opcode == 119
		|| opcode == 120 || opcode == 122 || opcode == 123 || opcode == 124 || opcode == 127 || opcode == 128 || opcode == 130 || opcode == 131 || opcode == 135 || opcode == 136 || opcode == 137 || opcode == 138 || opcode == 139 || opcode == 143 || opcode == 146
		|| opcode == 147 || opcode == 151 || opcode == 152 || opcode == 154 || opcode == 155 || opcode == 156 || opcode == 158 || opcode == 159 || opcode == 163 || opcode == 167 || opcode == 168 || opcode == 170 || opcode == 171 || opcode == 175 || opcode == 178
		|| opcode == 179 || opcode == 183 || opcode == 184 || opcode == 186 || opcode == 187 || opcode == 191 || opcode == 194 || opcode == 195 || opcode == 199 || opcode == 200 || opcode == 202 || opcode == 203 || opcode == 207 || opcode == 210 || opcode == 211 || opcode == 212
		|| opcode == 215 || opcode == 216 || opcode == 218 || opcode == 219 || opcode == 220 || opcode == 223 || opcode == 226 || opcode == 227 || opcode == 231 || opcode == 232 || opcode == 234 || opcode == 235 || opcode == 239 || opcode == 242
		|| opcode == 243 || opcode == 244 || opcode == 247 || opcode == 248 || opcode == 250 || opcode == 251 || opcode == 252 || opcode == 255) return true;
	return false;
}


// if there are 0 cycles left to perform, then the next instruction is ready to be fetched. the opcode is read from the program counter and the program counter is incremented as in the fetch execute cycle,
// then the opcode is looked up to find the addressing mode and the function is called. there are also 2 variables for additional cycles, as sometimes an addressing mode/instruction will require 
// extra clock cycles to finish processing, so every addressing mode and opcode function return a value to represent this. these values are added on to the cycles counter. if cycles is not 0, cycles is decremented
void cpu::clock() {
	if (cycles == 0) {
		opcode = read(pc);
		SetStatusFlag('U', true);
		pc++;
		uint8_t x = 0;
		uint8_t y = 0;
	http://archive.6502.org/datasheets/rockwell_r65c00_microprocessors.pdf
	https://www.masswerk.at/6502/6502_instruction_set.html
		switch (opcode) {
		case 0:
			x = IMP();
			y = BRK();
			cycles = 7;
			break;
		case 1:
			x = IZX();
			y = ORA();
			cycles = 6;
			break;
		case 2:
			x = IMP();
			y = XXX();
			cycles = 2;
			break;
		case 3:
			x = IMP();
			y = XXX();
			cycles = 8;
			break;
		case 4:
			x = IMP();
			y = XXX();
			cycles = 3;
			break;
		case 5:
			x = ZP0();
			y = ORA();
			cycles = 3;
			break;
		case 6:
			x = ZP0();
			y = ASL();
			cycles = 5;
			break;
		case 7:
			x = IMP();
			y = XXX();
			cycles = 5;
			break;
		case 8:
			x = IMP();
			y = PHP();
			cycles = 3;
			break;
		case 9:
			x = IMM();
			y = ORA();
			cycles = 2;
			break;
		case 10:
			x = IMP();
			y = ASL();
			cycles = 2;
			break;
		case 11:
			x = IMP();
			y = XXX();
			cycles = 2;
			break;
		case 12:
			x = IMP();
			y = XXX();
			cycles = 4;
			break;
		case 13:
			x = ABS();
			y = ORA();
			cycles = 4;
			break;
		case 14:
			x = ABS();
			y = ASL();
			cycles = 6;
			break;
		case 15:
			x = IMP();
			y = XXX();
			cycles = 6;
			break;
		case 16:
			x = REL();
			y = BPL();
			cycles = 2;
			break;
		case 17:
			x = IZY();
			y = ORA();
			cycles = 5;
			break;
		case 18:
			x = IMP();
			y = XXX();
			cycles = 2;
			break;
		case 19:
			x = IMP();
			y = XXX();
			cycles = 8;
			break;
		case 20:
			x = IMP();
			y = XXX();
			cycles = 4;
			break;
		case 21:
			x = ZPX();
			y = ORA();
			cycles = 4;
			break;
		case 22:
			x = ZPX();
			y = ASL();
			cycles = 6;
			break;
		case 23:
			x = IMP();
			y = XXX();
			cycles = 6;
			break;
		case 24:
			x = IMP();
			y = CLC();
			cycles = 2;
			break;
		case 25:
			x = ABY();
			y = ORA();
			cycles = 4;
			break;
		case 26:
			x = IMP();
			y = XXX();
			cycles = 2;
			break;
		case 27:
			x = IMP();
			y = XXX();
			cycles = 7;
			break;
		case 28:
			x = IMP();
			y = XXX();
			cycles = 4;
			break;
		case 29:
			x = ABX();
			y = ORA();
			cycles = 4;
			break;
		case 30:
			x = ABX();
			y = ASL();
			cycles = 7;
			break;
		case 31:
			x = IMP();
			y = XXX();
			cycles = 7;
			break;
		case 32:
			x = ABS();
			y = JSR();
			cycles = 6;
			break;
		case 33:
			x = IZX();
			y = AND();
			cycles = 6;
			break;
		case 34:
			x = IMP();
			y = XXX();
			cycles = 2;
			break;
		case 35:
			x = IMP();
			y = XXX();
			cycles = 8;
			break;
		case 36:
			x = ZP0();
			y = BIT();
			cycles = 3;
			break;
		case 37:
			x = ZP0();
			y = AND();
			cycles = 3;
			break;
		case 38:
			x = ZP0();
			y = ROL();
			cycles = 5;
			break;
		case 39:
			x = IMP();
			y = XXX();
			cycles = 5;
			break;
		case 40:
			x = IMP();
			y = PLP();
			cycles = 4;
			break;
		case 41:
			x = IMM();
			y = AND();
			cycles = 2;
			break;
		case 42:
			x = IMP();
			y = ROL();
			cycles = 2;
			break;
		case 43:
			x = IMP();
			y = XXX();
			cycles = 2;
			break;
		case 44:
			x = ABS();
			y = BIT();
			cycles = 4;
			break;
		case 45:
			x = ABS();
			y = AND();
			cycles = 4;
			break;
		case 46:
			x = ABS();
			y = ROL();
			cycles = 6;
			break;
		case 47:
			x = IMP();
			y = XXX();
			cycles = 6;
			break;
		case 48:
			x = REL();
			y = BMI();
			cycles = 2;
			break;
		case 49:
			x = IZY();
			y = AND();
			cycles = 5;
			break;
		case 50:
			x = IMP();
			y = XXX();
			cycles = 2;
			break;
		case 51:
			x = IMP();
			y = XXX();
			cycles = 8;
			break;
		case 52:
			x = IMP();
			y = XXX();
			cycles = 4;
			break;
		case 53:
			x = ZPX();
			y = AND();
			cycles = 4;
			break;
		case 54:
			x = ZPX();
			y = ROL();
			cycles = 6;
			break;
		case 55:
			x = IMP();
			y = XXX();
			cycles = 6;
			break;
		case 56:
			x = IMP();
			y = SEC();
			cycles = 2;
			break;
		case 57:
			x = ABY();
			y = AND();
			cycles = 4;
			break;
		case 58:
			x = IMP();
			y = XXX();
			cycles = 2;
			break;
		case 59:
			x = IMP();
			y = XXX();
			cycles = 7;
			break;
		case 60:
			x = IMP();
			y = XXX();
			cycles = 4;
			break;
		case 61:
			x = ABX();
			y = AND();
			cycles = 4;
			break;
		case 62:
			x = ABX();
			y = ROL();
			cycles = 7;
			break;
		case 63:
			x = IMP();
			y = XXX();
			cycles = 7;
			break;
		case 64:
			x = IMP();
			y = RTI();
			cycles = 6;
			break;
		case 65:
			x = IZX();
			y = EOR();
			cycles = 6;
			break;
		case 66:
			x = IMP();
			y = XXX();
			cycles = 2;
			break;
		case 67:
			x = IMP();
			y = XXX();
			cycles = 8;
			break;
		case 68:
			x = IMP();
			y = XXX();
			cycles = 3;
			break;
		case 69:
			x = ZP0();
			y = EOR();
			cycles = 3;
			break;
		case 70:
			x = ZP0();
			y = LSR();
			cycles = 5;
			break;
		case 71:
			x = IMP();
			y = XXX();
			cycles = 5;
			break;
		case 72:
			x = IMP();
			y = PHA();
			cycles = 3;
			break;
		case 73:
			x = IMM();
			y = EOR();
			cycles = 2;
			break;
		case 74:
			x = IMP();
			y = LSR();
			cycles = 2;
			break;
		case 75:
			x = IMP();
			y = XXX();
			cycles = 2;
			break;
		case 76:
			x = ABS();
			y = JMP();
			cycles = 3;
			break;
		case 77:
			x = ABS();
			y = EOR();
			cycles = 4;
			break;
		case 78:
			x = ABS();
			y = LSR();
			cycles = 6;
			break;
		case 79:
			x = IMP();
			y = XXX();
			cycles = 6;
			break;
		case 80:
			x = REL();
			y = BVC();
			cycles = 2;
			break;
		case 81:
			x = IZY();
			y = EOR();
			cycles = 5;
			break;
		case 82:
			x = IMP();
			y = XXX();
			cycles = 2;
			break;
		case 83:
			x = IMP();
			y = XXX();
			cycles = 8;
			break;
		case 84:
			x = IMP();
			y = XXX();
			cycles = 4;
			break;
		case 85:
			x = ZPX();
			y = EOR();
			cycles = 4;
			break;
		case 86:
			x = ZPX();
			y = LSR();
			cycles = 6;
			break;
		case 87:
			x = IMP();
			y = XXX();
			cycles = 6;
			break;
		case 88:
			x = IMP();
			y = CLI();
			cycles = 2;
			break;
		case 89:
			x = ABY();
			y = EOR();
			cycles = 4;
			break;
		case 90:
			x = IMP();
			y = XXX();
			cycles = 2;
			break;
		case 91:
			x = IMP();
			y = XXX();
			cycles = 7;
			break;
		case 92:
			x = IMP();
			y = XXX();
			cycles = 4;
			break;
		case 93:
			x = ABX();
			y = EOR();
			cycles = 4;
			break;
		case 94:
			x = ABX();
			y = LSR();
			cycles = 7;
			break;
		case 95:
			x = IMP();
			y = XXX();
			cycles = 7;
			break;
		case 96:
			x = IMP();
			y = RTS();
			cycles = 6;
			break;
		case 97:
			x = IZX();
			y = ADC();
			cycles = 6;
			break;
		case 98:
			x = IMP();
			y = XXX();
			cycles = 2;
			break;
		case 99:
			x = IMP();
			y = XXX();
			cycles = 8;
			break;
		case 100:
			x = IMP();
			y = XXX();
			cycles = 3;
			break;
		case 101:
			x = ZP0();
			y = ADC();
			cycles = 3;
			break;
		case 102:
			x = ZP0();
			y = ROR();
			cycles = 5;
			break;
		case 103:
			x = IMP();
			y = XXX();
			cycles = 5;
			break;
		case 104:
			x = IMP();
			y = PLA();
			cycles = 4;
			break;
		case 105:
			x = IMM();
			y = ADC();
			cycles = 2;
			break;
		case 106:
			x = IMP();
			y = ROR();
			cycles = 2;
			break;
		case 107:
			x = IMP();
			y = XXX();
			cycles = 2;
			break;
		case 108:
			x = IND();
			y = JMP();
			cycles = 5;
			break;
		case 109:
			x = ABS();
			y = ADC();
			cycles = 4;
			break;
		case 110:
			x = ABS();
			y = ROR();
			cycles = 6;
			break;
		case 111:
			x = IMP();
			y = XXX();
			cycles = 6;
			break;
		case 112:
			x = REL();
			y = BVS();
			cycles = 2;
			break;
		case 113:
			x = IZY();
			y = ADC();
			cycles = 5;
			break;
		case 114:
			x = IMP();
			y = XXX();
			cycles = 2;
			break;
		case 115:
			x = IMP();
			y = XXX();
			cycles = 8;
			break;
		case 116:
			x = IMP();
			y = XXX();
			cycles = 4;
			break;
		case 117:
			x = ZPX();
			y = ADC();
			cycles = 4;
			break;
		case 118:
			x = ZPX();
			y = ROR();
			cycles = 6;
			break;
		case 119:
			x = IMP();
			y = XXX();
			cycles = 6;
			break;
		case 120:
			x = IMP();
			y = SEI();
			cycles = 2;
			break;
		case 121:
			x = ABY();
			y = ADC();
			cycles = 4;
			break;
		case 122:
			x = IMP();
			y = XXX();
			cycles = 2;
			break;
		case 123:
			x = IMP();
			y = XXX();
			cycles = 7;
			break;
		case 124:
			x = IMP();
			y = XXX();
			cycles = 4;
			break;
		case 125:
			x = ABX();
			y = ADC();
			cycles = 4;
			break;
		case 126:
			x = ABX();
			y = ROR();
			cycles = 7;
			break;
		case 127:
			x = IMP();
			y = XXX();
			cycles = 7;
			break;
		case 128:
			x = IMP();
			y = XXX();
			cycles = 2;
			break;
		case 129:
			x = IZX();
			y = STA();
			cycles = 6;
			break;
		case 130:
			x = IMP();
			y = XXX();
			cycles = 2;
			break;
		case 131:
			x = IMP();
			y = XXX();
			cycles = 6;
			break;
		case 132:
			x = ZP0();
			y = STY();
			cycles = 3;
			break;
		case 133:
			x = ZP0();
			y = STA();
			cycles = 3;
			break;
		case 134:
			x = ZP0();
			y = STX();
			cycles = 3;
			break;
		case 135:
			x = IMP();
			y = XXX();
			cycles = 3;
			break;
		case 136:
			x = IMP();
			y = DEY();
			cycles = 2;
			break;
		case 137:
			x = IMP();
			y = XXX();
			cycles = 2;
			break;
		case 138:
			x = IMP();
			y = TXA();
			cycles = 2;
			break;
		case 139:
			x = IMP();
			y = XXX();
			cycles = 2;
			break;
		case 140:
			x = ABS();
			y = STY();
			cycles = 4;
			break;
		case 141:
			x = ABS();
			y = STA();
			cycles = 4;
			break;
		case 142:
			x = ABS();
			y = STX();
			cycles = 4;
			break;
		case 143:
			x = IMP();
			y = XXX();
			cycles = 4;
			break;
		case 144:
			x = REL();
			y = BCC();
			cycles = 2;
			break;
		case 145:
			x = IZY();
			y = STA();
			cycles = 6;
			break;
		case 146:
			x = IMP();
			y = XXX();
			cycles = 2;
			break;
		case 147:
			x = IMP();
			y = XXX();
			cycles = 6;
			break;
		case 148:
			x = ZPX();
			y = STY();
			cycles = 4;
			break;
		case 149:
			x = ZPX();
			y = STA();
			cycles = 4;
			break;
		case 150:
			x = ZPY();
			y = STX();
			cycles = 4;
			break;
		case 151:
			x = IMP();
			y = XXX();
			cycles = 4;
			break;
		case 152:
			x = IMP();
			y = TYA();
			cycles = 2;
			break;
		case 153:
			x = ABY();
			y = STA();
			cycles = 5;
			break;
		case 154:
			x = IMP();
			y = TXS();
			cycles = 2;
			break;
		case 155:
			x = IMP();
			y = XXX();
			cycles = 5;
			break;
		case 156:
			x = IMP();
			y = XXX();
			cycles = 5;
			break;
		case 157:
			x = ABX();
			y = STA();
			cycles = 5;
			break;
		case 158:
			x = IMP();
			y = XXX();
			cycles = 5;
			break;
		case 159:
			x = IMP();
			y = XXX();
			cycles = 5;
			break;
		case 160:
			x = IMM();
			y = LDY();
			cycles = 2;
			break;
		case 161:
			x = IZX();
			y = LDA();
			cycles = 6;
			break;
		case 162:
			x = IMM();
			y = LDX();
			cycles = 2;
			break;
		case 163:
			x = IMP();
			y = XXX();
			cycles = 6;
			break;
		case 164:
			x = ZP0();
			y = LDY();
			cycles = 3;
			break;
		case 165:
			x = ZP0();
			y = LDA();
			cycles = 3;
			break;
		case 166:
			x = ZP0();
			y = LDX();
			cycles = 3;
			break;
		case 167:
			x = IMP();
			y = XXX();
			cycles = 3;
			break;
		case 168:
			x = IMP();
			y = TAY();
			cycles = 2;
			break;
		case 169:
			x = IMM();
			y = LDA();
			cycles = 2;
			break;
		case 170:
			x = IMP();
			y = TAX();
			cycles = 2;
			break;
		case 171:
			x = IMP();
			y = XXX();
			cycles = 2;
			break;
		case 172:
			x = ABS();
			y = LDY();
			cycles = 4;
			break;
		case 173:
			x = ABS();
			y = LDA();
			cycles = 4;
			break;
		case 174:
			x = ABS();
			y = LDX();
			cycles = 4;
			break;
		case 175:
			x = IMP();
			y = XXX();
			cycles = 4;
			break;
		case 176:
			x = REL();
			y = BCS();
			cycles = 2;
			break;
		case 177:
			x = IZY();
			y = LDA();
			cycles = 5;
			break;
		case 178:
			x = IMP();
			y = XXX();
			cycles = 2;
			break;
		case 179:
			x = IMP();
			y = XXX();
			cycles = 5;
			break;
		case 180:
			x = ZPX();
			y = LDY();
			cycles = 4;
			break;
		case 181:
			x = ZPX();
			y = LDA();
			cycles = 4;
			break;
		case 182:
			x = ZPY();
			y = LDX();
			cycles = 4;
			break;
		case 183:
			x = IMP();
			y = XXX();
			cycles = 4;
			break;
		case 184:
			x = IMP();
			y = CLV();
			cycles = 2;
			break;
		case 185:
			x = ABY();
			y = LDA();
			cycles = 4;
			break;
		case 186:
			x = IMP();
			y = TSX();
			cycles = 2;
			break;
		case 187:
			x = IMP();
			y = XXX();
			cycles = 4;
			break;
		case 188:
			x = ABX();
			y = LDY();
			cycles = 4;
			break;
		case 189:
			x = ABX();
			y = LDA();
			cycles = 4;
			break;
		case 190:
			x = ABY();
			y = LDX();
			cycles = 4;
			break;
		case 191:
			x = IMP();
			y = XXX();
			cycles = 4;
			break;
		case 192:
			x = IMM();
			y = CPY();
			cycles = 2;
			break;
		case 193:
			x = IZX();
			y = CMP();
			cycles = 6;
			break;
		case 194:
			x = IMP();
			y = XXX();
			cycles = 2;
			break;
		case 195:
			x = IMP();
			y = XXX();
			cycles = 8;
			break;
		case 196:
			x = ZP0();
			y = CPY();
			cycles = 3;
			break;
		case 197:
			x = ZP0();
			y = CMP();
			cycles = 3;
			break;
		case 198:
			x = ZP0();
			y = DEC();
			cycles = 5;
			break;
		case 199:
			x = IMP();
			y = XXX();
			cycles = 5;
			break;
		case 200:
			x = IMP();
			y = INY();
			cycles = 2;
			break;
		case 201:
			x = IMM();
			y = CMP();
			cycles = 2;
			break;
		case 202:
			x = IMP();
			y = DEX();
			cycles = 2;
			break;
		case 203:
			x = IMP();
			y = XXX();
			cycles = 2;
			break;
		case 204:
			x = ABS();
			y = CPY();
			cycles = 4;
			break;
		case 205:
			x = ABS();
			y = CMP();
			cycles = 4;
			break;
		case 206:
			x = ABS();
			y = DEC();
			cycles = 6;
			break;
		case 207:
			x = IMP();
			y = XXX();
			cycles = 6;
			break;

		case 208:
			x = REL();
			y = BNE();
			cycles = 2;
			break;

		case 209:
			x = IZY();
			y = CMP();
			cycles = 5;
			break;
		case 210:
			x = IMP();
			y = XXX();
			cycles = 2;
			break;
		case 211:
			x = IMP();
			y = XXX();
			cycles = 8;
			break;
		case 212:
			x = IMP();
			y = XXX();
			cycles = 4;
			break;
		case 213:
			x = ZPX();
			y = CMP();
			cycles = 4;
			break;
		case 214:
			x = ZPX();
			y = DEC();
			cycles = 6;
			break;
		case 215:
			x = IMP();
			y = XXX();
			cycles = 6;
			break;
		case 216:
			x = IMP();
			y = CLD();
			cycles = 2;
			break;
		case 217:
			x = ABY();
			y = CMP();
			cycles = 4;
			break;
		case 218:
			x = IMP();
			y = XXX();
			cycles = 2;
			break;
		case 219:
			x = IMP();
			y = XXX();
			cycles = 7;
			break;
		case 220:
			x = IMP();
			y = XXX();
			cycles = 4;
			break;
		case 221:
			x = ABX();
			y = CMP();
			cycles = 4;
			break;
		case 222:
			x = ABX();
			y = DEC();
			cycles = 7;
			break;
		case 223:
			x = IMP();
			y = XXX();
			cycles = 7;
			break;

		case 224:
			x = IMM();
			y = CPX();
			cycles = 2;
			break;
		case 225:
			x = IZX();
			y = SBC();
			cycles = 6;
			break;
		case 226:
			x = IMP();
			y = XXX();
			cycles = 2;
			break;
		case 227:
			x = IMP();
			y = XXX();
			cycles = 8;
			break;
		case 228:
			x = ZP0();
			y = CPX();
			cycles = 3;
			break;
		case 229:
			x = ZP0();
			y = SBC();
			cycles = 3;
			break;
		case 230:
			x = ZP0();
			y = INC();
			cycles = 5;
			break;
		case 231:
			x = IMP();
			y = XXX();
			cycles = 5;
			break;
		case 232:
			x = IMP();
			y = INX();
			cycles = 2;
			break;
		case 233:
			x = IMM();
			y = SBC();
			cycles = 2;
			break;
		case 234:
			x = IMP();
			y = XXX();
			cycles = 2;
			break;

		case 235:
			x = IMP();
			y = XXX();
			cycles = 2;
			break;
		case 236:
			x = ABS();
			y = CPX();
			cycles = 4;
			break;

		case 237:
			x = ABS();
			y = SBC();
			cycles = 4;
			break;
		case 238:
			x = ABS();
			y = INC();
			cycles = 6;
			break;
		case 239:
			x = IMP();
			y = XXX();
			cycles = 2;
			break;
		case 240:
			x = REL();
			y = BEQ();
			cycles = 2;
			break;
		case 241:
			x = IZY();
			y = SBC();
			cycles = 5;
			break;
		case 242:
			x = IMP();
			y = XXX();
			cycles = 2;
			break;
		case 243:
			x = IMP();
			y = XXX();
			cycles = 8;
			break;
		case 244:
			x = IMP();
			y = XXX();
			cycles = 4;
			break;
		case 245:
			x = ZPX();
			y = SBC();
			cycles = 4;
			break;
		case 246:
			x = ZPX();
			y = INC();
			cycles = 6;
			break;
		case 247:
			x = IMP();
			y = XXX();
			cycles = 6;
			break;
		case 248:
			x = IMP();
			y = SED();
			cycles = 2;
			break;
		case 249:
			x = ABY();
			y = SBC();
			cycles = 4;
			break;
		case 250:
			x = IMP();
			y = XXX();
			cycles = 2;
			break;
		case 251:
			x = IMP();
			y = XXX();
			cycles = 7;
			break;
		case 252:
			x = IMP();
			y = XXX();
			cycles = 4;
			break;
		case 253:
			x = ABX();
			y = SBC();
			cycles = 4;
			break;
		case 254:
			x = ABX();
			y = INC();
			cycles = 7;
			break;
		case 255:
			x = IMP();
			y = XXX();
			cycles = 7;
			break;
		}
		cycles += (x & y);

	}
	cycles--;
}


// resets the cpu
// the program counter is read in two parts from $FFFC and $FFFD as they contain a secondary address for the program counter to jump to
void cpu::reset() {
	accumulator = 0;
	xindex = 0;
	yindex = 0;
	sp = 0;
	sr = 0 | srflagslist[5];

	uint16_t lo = read(0xFFFC);
	uint16_t hi = read(0xFFFD);

	pc = (hi << 8) | lo;
	addr_rel = 0;
	addr_abs = 0;
	fetched = 0;

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

// only happens when the interrupt disable flag is not set. to preserve the current state of the processor, the program counter is pushed to the stack as well as the current status register 
// so that they can be restored by the RTI instruction when the interrupt is complete. the program counter is then set to a new fixed address which contains the typical routine for dealing with interrupts, starting at $FFFE
void cpu::interrupt() {
	if (GetStatusFlag('ID') == 0) {

		pushtostack((pc >> 8) & 0x00FF);
		pushtostack(pc & 0x00FF);

		SetStatusFlag('B', 0);
		SetStatusFlag('U', 1);
		SetStatusFlag('ID', 1);
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

	SetStatusFlag('B', 0);
	SetStatusFlag('U', 1);
	SetStatusFlag('ID', 1);
	pushtostack(sr);
	addr_abs = 0xFFFA;
	uint16_t lo = read(addr_abs);
	uint16_t hi = read(addr_abs + 1);
	pc = (hi << 8) | lo;

	cycles = 8;


}

uint8_t cpu::GetStatusFlag(char f)
{
	/*
	gets specific value of a bit in the status register, f being the flag that is to be checked.performing an AND operation on the status register and the flag will produce a byte where everything is 0
	except the position of the flag if it is set. for example if you want to check if the zero flag is set, as it is the 2nd bit if it is set the status register will show 00000010, while f will be
	00000010 as well. performing an AND operation between these two will produce 00000010 which is 2 in decimal and therefore greater than 0, so it will return 1, indicating that the zero flag is set.
	on the other hand, if the carry flag is set while the zero flag is not, the status register will be 00000001 while f will be 00000010. an and operation of these will produce 00000000, which is not greater than
	0, returning 0 accordingly to indicate that the zero flag is not set.
	*/

	auto func = [&](uint8_t flags) {
		if ((sr & flags) > 0) return 1;
		else return 0;
	};

	switch (f) {
	case 'C':
		return func(srflagslist[0]);
		break;
	case 'Z':
		return func(srflagslist[1]);
		break;
	case 'ID':
		return func(srflagslist[2]);
		break;
	case 'D':
		return func(srflagslist[3]);
		break;
	case 'B':
		return func(srflagslist[4]);
		break;
	case 'U':
		return func(srflagslist[5]);
		break;
	case 'O':
		return func(srflagslist[6]);
		break;
	case 'N':
		return func(srflagslist[7]);
		break;
	}
}

/*
sets a specific bit of the status register. v represents the value you want to set it to, true for 1, false for 0. therefore if the status register contained
00000010 and you wanted to clear the zero flag, f would be 00000010 and v would be false. so the status register and NOT f would be ANDed together, giving
00000010 & 11111101 which is 00000000. The new status register would then be 00000000 with the zero flag cleared.
*/
void cpu::SetStatusFlag(char f, bool v)
{
	auto func = [&](uint8_t flags) {
		if (v) {
			sr = sr | flags;
		}
		else sr = sr & ~flags;
	};

	switch (f) {
	case 'C':
		func(srflagslist[0]);
		break;
	case 'Z':
		func(srflagslist[1]);
		break;
	case 'ID':
		func(srflagslist[2]);
		break;
	case 'D':
		func(srflagslist[3]);
		break;
	case 'B':
		func(srflagslist[4]);
		break;
	case 'U':
		func(srflagslist[5]);
		break;
	case 'O':
		func(srflagslist[6]);
		break;
	case 'N':
		func(srflagslist[7]);
		break;
	}
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

	uint16_t lo = read((t + xindex) & 0x00FF);
	uint16_t hi = read((t + xindex + 1) & 0x00FF);

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

// instructions

// fetches data from the absolute address in ram 
uint8_t cpu::fetch() {
	if (!implied(opcode)) {
		fetched = read(addr_abs);
	}
	return fetched;
}

// add memory to accumulator with carry
// overflow can occur when two positive numbers are added and the result is a negative number, or when two negative numbers are added and the result is a positive number. aka overflow occurs when the sign of the parameters and result is different
// XOR'ing the accumulator with the fetched data returns 0x80 if their signs are different because the first bit is the sign bit. NOT this operation so that you get 0x80 if they have the same sign. 
// Do the same with the accumulator and the sum of the three variables, then AND these two results together. This will have the effect of returning 1 if the accumulator and the fetched data have the same sign, and the 
// accumulator and summation of the accumulator, fetched data and the carry flag have a different sign. 
// https://stackoverflow.com/questions/29193303/6502-emulation-proper-way-to-implement-adc-and-sbc
uint8_t cpu::ADC() {
	fetch();
	auto temp = accumulator + fetched + GetStatusFlag('C');
	SetStatusFlag('C', temp > 255);
	SetStatusFlag('Z', (temp & 0x00FF) == 0);
	SetStatusFlag('N', temp & 0x80);
	SetStatusFlag('O', (~(accumulator ^ fetched) & (accumulator ^ temp)) & 0x80);
	accumulator = temp & 0x00FF;
	return 1;
}

// AND fetched data with accumulator. requires an extra cycle if a page boundary is crossed
uint8_t cpu::AND() {
	fetch();
	accumulator = accumulator & fetched;
	SetStatusFlag('Z', accumulator == 0);
	SetStatusFlag('N', accumulator & 0x80);
	return 1;
}

// left shift operation
uint8_t cpu::ASL() {
	fetch();
	auto temp = fetched << 1;
	SetStatusFlag('C', (temp & 0xFF00) > 0);
	SetStatusFlag('N', (temp & 0x80));
	SetStatusFlag('Z', (temp & 0x00FF) == 0);
	// if implied addressing there is no address to write to, so the accumulator holds the result of the operation
	if (implied(opcode)) {
		accumulator = temp & 0x00FF;
	}
	else {
		write(addr_abs, temp & 0x00FF);
	}
	return 0;
}


// branch if the carry bit is clear so that pc = address. at least 1 additional clock cycle will always be required, 2 if a page boundary is crossed
uint8_t cpu::BCC() {
	if (GetStatusFlag('C') == 0) {
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
	if (GetStatusFlag('C') == 1) {
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
	if (GetStatusFlag('Z') == 1) {
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
	SetStatusFlag('Z', (temp & 0x00FF) == 0);
	SetStatusFlag('N', fetched & (1 << 7));
	SetStatusFlag('O', fetched & (1 << 6));
	return 0;
}
// branch if negative flag set
uint8_t cpu::BMI() {
	if (GetStatusFlag('N') == 1) {
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
	if (GetStatusFlag('Z') == 0) {
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
	if (GetStatusFlag('N') == 0) {
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

	SetStatusFlag('ID', 1);
	pushtostack((pc >> 8) & 0x00FF);
	pushtostack(pc & 0x00FF);

	SetStatusFlag('B', 1);
	pushtostack(sr);
	SetStatusFlag('B', 0);

	pc = read(0xFFFE) | (read(0xFFFF) << 8);
	return 0;
}
// branch if overflow flag clear
uint8_t cpu::BVC() {
	if (GetStatusFlag('O') == 0) {
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
	if (GetStatusFlag('O') == 1) {
		cycles++;
		addr_abs = pc + addr_rel;
		if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
			cycles++;
		}
		pc = addr_abs;
	}
	return 0;
}

// clears carry flag 
uint8_t cpu::CLC() {
	SetStatusFlag('C', false);
	return 0;
}

// clear decimal flag
uint8_t cpu::CLD() {
	SetStatusFlag('D', false);
	return 0;
}
// clear interrupt disable flag
uint8_t cpu::CLI() {
	SetStatusFlag('ID', false);
	return 0;
}

// clear overflow flag
uint8_t cpu::CLV() {
	SetStatusFlag('O', false);
	return 0;
}

// compare memory with accumulator
uint8_t cpu::CMP()
{
	fetch();
	auto temp = accumulator - fetched;
	SetStatusFlag('C', accumulator >= fetched);
	SetStatusFlag('Z', (temp & 0x00FF) == 0);
	SetStatusFlag('N', temp & 0x0080);
	return 1;
}


// compare memory with x register
uint8_t cpu::CPX()
{
	fetch();
	auto temp = xindex - fetched;
	SetStatusFlag('C', xindex >= fetched);
	SetStatusFlag('Z', (temp & 0x00FF) == 0);
	SetStatusFlag('N', temp & 0x0080);
	return 0;
}


// compare memory with y register
uint8_t cpu::CPY()
{
	fetch();
	auto temp = yindex - fetched;
	SetStatusFlag('C', yindex >= fetched);
	SetStatusFlag('Z', (temp & 0x00FF) == 0);
	SetStatusFlag('N', temp & 0x0080);
	return 0;
}


// decrement memory by one
uint8_t cpu::DEC() {
	fetch();
	auto temp = fetched - 1;
	write(addr_abs, temp & 0x00FF);
	SetStatusFlag('N', temp & 0x80);
	SetStatusFlag('Z', (temp & 0x00FF) == 0);
	return 0;
}
// decrement x reg by 1
uint8_t cpu::DEX() {
	xindex -= 1;
	SetStatusFlag('N', xindex & 0x80);
	SetStatusFlag('Z', xindex == 0);
	return 0;
}
// decrement y reg by 1
uint8_t cpu::DEY() {
	yindex--;
	SetStatusFlag('N', yindex & 0x80);
	SetStatusFlag('Z', yindex == 0);
	return 0;
}
// XOR memory with accumulator
uint8_t cpu::EOR() {
	fetch();
	auto temp = (fetched | accumulator) & (~(fetched & accumulator));
	accumulator = temp;
	SetStatusFlag('N', accumulator & 0x80);
	SetStatusFlag('Z', accumulator == 0);
	return 1;
}
// increment memory by one
uint8_t cpu::INC() {
	fetch();
	auto temp = fetched + 1;
	write(addr_abs, temp & 0x00FF);
	SetStatusFlag('Z', (temp & 0x00FF) == 0);
	SetStatusFlag('N', temp & 0x0080);
	return 0;
}
// increment x reg by one
uint8_t cpu::INX() {
	xindex++;
	SetStatusFlag('N', xindex & 0x80);
	SetStatusFlag('Z', xindex == 0);
	return 0;
}
// increment y reg by one
uint8_t cpu::INY() {
	yindex++;
	SetStatusFlag('N', yindex & 0x80);
	SetStatusFlag('Z', yindex == 0);
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
	SetStatusFlag('N', accumulator & 0x80);
	SetStatusFlag('Z', accumulator == 0);
	return 1;

}
// load x reg with memory
uint8_t cpu::LDX() {

	fetch();
	xindex = fetched;
	SetStatusFlag('N', xindex & 0x80);
	SetStatusFlag('Z', xindex == 0);
	return 1;

}
// load y reg with memory
uint8_t cpu::LDY() {

	fetch();
	yindex = fetched;
	SetStatusFlag('N', fetched & 0x80);
	SetStatusFlag('Z', fetched == 0);
	return 1;

}
// shift right 
uint8_t cpu::LSR() {
	fetch();
	SetStatusFlag('C', fetched & 0x0001);
	auto temp = fetched >> 1;
	SetStatusFlag('Z', (temp & 0x00FF) == 0);
	SetStatusFlag('N', false);
	if (implied(opcode))
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
	SetStatusFlag('N', (temp & 0x80));
	SetStatusFlag('Z', (temp == 0));
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
	SetStatusFlag('B', true);
	SetStatusFlag('U', true);
	pushtostack(sr);
	return 0;
}


// pop accumulator from stack
uint8_t cpu::PLA() {
	accumulator = popfromstack();
	SetStatusFlag('N', accumulator & 0x80);
	SetStatusFlag('Z', accumulator == 0);
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
	auto temp = fetched << 1 | GetStatusFlag('C');
	SetStatusFlag('C', temp & 0xFF00);
	SetStatusFlag('Z', (temp & 0x00FF) == 0);
	SetStatusFlag('N', temp & 0x0080);
	if (implied(opcode)) {
		accumulator = temp & 0x00FF;
	}
	else {
		write(addr_abs, temp & 0x00FF);
	}
	return 0;

}

// restores cpu state to how it was before an interrupt occured, taking the status register and program counter from the stack 
uint8_t cpu::RTI() {
	sr = popfromstack();
	// clear B and unused flags
	sr = sr & ~srflagslist[4];
	sr = sr & ~srflagslist[5];

	sp++;
	pc = read(0x0100 + sp);
	sp++;
	pc = pc | read(0x0100 + sp) << 8;
	return 0;
}


// rotate one bit right 
uint8_t cpu::ROR() {
	fetch();
	auto temp = (GetStatusFlag('C') << 7) | (fetched >> 1);
	SetStatusFlag('C', fetched & 0x01);
	SetStatusFlag('Z', (temp & 0x00FF) == 0);
	SetStatusFlag('N', temp & 0x0080);
	if (implied(opcode))
		accumulator = temp & 0x00FF;
	else
		write(addr_abs, temp & 0x00FF);
	return 0;
}
// return from subroutine
uint8_t cpu::RTS()
{
	auto temp = popfromstack();
	auto temp2 = (popfromstack()) << 8;
	pc = temp | temp2;
	pc++;
	return 0;
}

// subtraction with carry, basically the inverse of the ADC instruction
uint8_t cpu::SBC() {
	fetch();
	auto value = (fetched ^ 0x00FF);
	auto temp = accumulator + value + GetStatusFlag('C');
	SetStatusFlag('C', temp & 0xFF00);
	SetStatusFlag('Z', (temp & 0x00FF) == 0);
	SetStatusFlag('N', temp & 0x0080);
	SetStatusFlag('O', (temp ^ accumulator) & (temp ^ value) & 0x80);
	accumulator = temp & 0x00FF;
	return 1;

}

// set carry flag 
uint8_t cpu::SEC() {
	SetStatusFlag('C', true);
	return 0;
}
// set decimal flag
uint8_t cpu::SED() {
	SetStatusFlag('D', true);
	return 0;
}
// set interrupt disable flag
uint8_t cpu::SEI() {
	SetStatusFlag('ID', true);
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
	SetStatusFlag('N', xindex & 0x80);
	SetStatusFlag('Z', xindex == 0);
	return 0;
}
// transfer accumulator to y register
uint8_t cpu::TAY() {
	yindex = accumulator;
	SetStatusFlag('N', yindex & 0x80);
	SetStatusFlag('Z', yindex == 0);
	return 0;
}
// transfer stack pointer to x register
uint8_t cpu::TSX() {
	xindex = sp;
	SetStatusFlag('N', yindex & 0x80);
	SetStatusFlag('Z', yindex == 0);
	return 0;
}
// transfer accumulator to x register
uint8_t cpu::TXA() {
	accumulator = xindex;
	SetStatusFlag('N', accumulator & 0x80);
	SetStatusFlag('Z', accumulator == 0);
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
	SetStatusFlag('N', accumulator & 0x80);
	SetStatusFlag('Z', accumulator == 0);
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

