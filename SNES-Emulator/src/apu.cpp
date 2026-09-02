#include "../include/apu.h"

Spc_700::Spc_700()
{
	using ref = Spc_700;

	// 256-entry SPC700 opcode lookup.

	//will take me longer than i think

	lookup =
	{
		// 0                                // 1                                // 2                                // 3                                // 4                                // 5                                // 6                                // 7                                // 8                                // 9                                // A                                // B                                // C                                // D                                // E                                // F
		  {"NOP", &ref::NOP, &ref::IMP, 2}, {"JSTO", &ref::TCALL, &ref::IMP, 8}, {"SET1", &ref::SET1, &ref::DR, 4}, {"BBS", &ref::BBS, &ref::DR, 5}, {"ORA", &ref::OR, &ref::DR, 3}, {"ORA", &ref::OR, &ref::AB, 4}, {"ORA", &ref::OR, &ref::DRI, 3}, {"ORA", &ref::OR, &ref::DRID, 6}, {"ORA", &ref::OR, &ref::IMM, 2}, {"OR", &ref::OR, &ref::DR, 6}, {"ORC", &ref::OR1, &ref::DR, 5}, {"ASL", &ref::ASL, &ref::DR, 4}, {"ASL", &ref::ASL, &ref::AB, 5}, {"PHP", &ref::PUSH, &ref::IMP, 4}, {"SET1", &ref::SET1, &ref::AB, 6}, {"BRK", &ref::BRK, &ref::IMP, 8},    // 0                        
		  {"BPL", &ref::BPL, &ref::RL, 2}, {"JST1", &ref::TCALL, &ref::IMP, 8}, {"CLR1", &ref::CLR1, &ref::DR, 4}, {"BBC", &ref::BBC, &ref::DR, 5}, {"ORA", &ref::OR, &ref::DRX, 4}, {"ORA", &ref::OR, &ref::ABX, 5}, {"ORA", &ref::OR, &ref::ABY, 5}, {"ORA", &ref::OR, &ref::DRII, 6}, {"OR", &ref::OR, &ref::DR, 5}, {"OR", &ref::OR, &ref::DR, 5}, {"DEW", &ref::DECW, &ref::DR, 6}, {"ASL", &ref::ASL, &ref::DRX, 5}, {"ASL", &ref::ASL, &ref::AC, 2}, {"DEX", &ref::DEC, &ref::IMP, 2}, {"CPX", &ref::CMP, &ref::AB, 4}, {"JMP", &ref::JMP, &ref::ABDI, 6},     // 1
		  {"CPL", &ref::CLRP, &ref::IMP, 2}, {"JST2", &ref::TCALL, &ref::IMP, 8}, {"SET1", &ref::SET1, &ref::DR, 4}, {"BBS", &ref::BBS, &ref::DR, 5}, {"AND", &ref::AND, &ref::DR, 3}, {"AND", &ref::AND, &ref::AB, 4}, {"AND", &ref::AND, &ref::DRI, 3}, {"AND", &ref::AND, &ref::DRID, 6}, {"AND", &ref::AND, &ref::IMM, 2}, {"AND", &ref::AND, &ref::DR, 6}, {"ORC", &ref::OR1, &ref::DR, 5}, {"ROL", &ref::ROL, &ref::DR, 4}, {"ROL", &ref::ROL, &ref::AB, 5}, {"PHA", &ref::PUSH, &ref::IMP, 4}, {"CBNE", &ref::CBNE, &ref::DR, 5}, {"BRA", &ref::BRA, &ref::RL, 4},  // 2
		  {"BMI", &ref::BMI, &ref::RL, 2}, {"JST3", &ref::TCALL, &ref::IMP, 8}, {"CLR1", &ref::CLR1, &ref::DR, 4}, {"BBC", &ref::BBC, &ref::DR, 5}, {"AND", &ref::AND, &ref::DRX, 4}, {"AND", &ref::AND, &ref::ABX, 5}, {"AND", &ref::AND, &ref::ABY, 5}, {"AND", &ref::AND, &ref::DRII, 6}, {"AND", &ref::AND, &ref::DR, 5}, {"AND", &ref::AND, &ref::DR, 5}, {"INW", &ref::INCW, &ref::DR, 6}, {"ROL", &ref::ROL, &ref::DRX, 5}, {"ROL", &ref::ROL, &ref::AC, 2}, {"INX", &ref::INC, &ref::IMP, 2}, {"CPX", &ref::CMP, &ref::DR, 3}, {"JSR", &ref::CALL, &ref::AB, 8},   // 3
		  {"SEP", &ref::SETP, &ref::IMP, 2}, {"JST4", &ref::TCALL, &ref::IMP, 8}, {"SET1", &ref::SET1, &ref::DR, 4}, {"BBS", &ref::BBS, &ref::DR, 5}, {"EORA", &ref::EOR, &ref::DR, 3}, {"EORA", &ref::EOR, &ref::AB, 4}, {"EORA", &ref::EOR, &ref::DRI, 3}, {"EORA", &ref::EOR, &ref::DRID, 6}, {"EORA", &ref::EOR, &ref::IMM, 2}, {"EOR", &ref::EOR, &ref::DR, 6}, {"ANDC", &ref::AND1, &ref::DR, 5}, {"LSR", &ref::LSR, &ref::DR, 4}, {"LSR", &ref::LSR, &ref::AB, 5}, {"PHX", &ref::PUSH, &ref::IMP, 4}, {"CLR1", &ref::CLR1, &ref::AB, 6}, {"JSP", &ref::PCALL, &ref::DR, 6}, // 4
		  {"BVC", &ref::BVC, &ref::RL, 2}, {"JST5", &ref::TCALL, &ref::IMP, 8}, {"CLR1", &ref::CLR1, &ref::DR, 4}, {"BBC", &ref::BBC, &ref::DR, 5}, {"EORA", &ref::EOR, &ref::DRX, 4}, {"EORA", &ref::EOR, &ref::ABX, 5}, {"EORA", &ref::EOR, &ref::ABY, 5}, {"EORA", &ref::EOR, &ref::DRII, 6}, {"EOR", &ref::EOR, &ref::DR, 5}, {"EOR", &ref::EOR, &ref::DR, 5}, {"CPW", &ref::CMPW, &ref::DR, 4}, {"LSR", &ref::LSR, &ref::DRX, 5}, {"LSR", &ref::LSR, &ref::AC, 2}, {"TAX", &ref::MOV, &ref::IMP, 2}, {"CPY", &ref::CMP, &ref::AB, 4}, {"JMP", &ref::JMP, &ref::AB, 3},   // 5
		  {"CLC", &ref::CLRC, &ref::IMP, 2}, {"JST6", &ref::TCALL, &ref::IMP, 8}, {"SET1", &ref::SET1, &ref::DR, 4}, {"BBS", &ref::BBS, &ref::DR, 5}, {"CMP", &ref::CMP, &ref::DR, 3}, {"CMP", &ref::CMP, &ref::AB, 4}, {"CMP", &ref::CMP, &ref::DRI, 3}, {"CMP", &ref::CMP, &ref::DRID, 6}, {"CMP", &ref::CMP, &ref::IMM, 2}, {"CMP", &ref::CMP, &ref::DR, 6}, {"ANDC", &ref::AND1, &ref::DR, 5}, {"ROR", &ref::ROR, &ref::DR, 4}, {"ROR", &ref::ROR, &ref::AB, 5}, {"PHY", &ref::PUSH, &ref::IMP, 4}, {"DBNZ", &ref::DBNZ, &ref::DR, 5}, {"RTS", &ref::RET, &ref::IMP, 5},   // 6
		  {"BVS", &ref::BVS, &ref::RL, 2}, {"JST7", &ref::TCALL, &ref::IMP, 8}, {"CLR1", &ref::CLR1, &ref::DR, 4}, {"BBC", &ref::BBC, &ref::DR, 5}, {"CMP", &ref::CMP, &ref::DRX, 4}, {"CMP", &ref::CMP, &ref::ABX, 5}, {"CMP", &ref::CMP, &ref::ABY, 5}, {"CMP", &ref::CMP, &ref::DRII, 6}, {"CMP", &ref::CMP, &ref::DR, 5}, {"CMP", &ref::CMP, &ref::DR, 5}, {"ADW", &ref::ADDW, &ref::DR, 5}, {"ROR", &ref::ROR, &ref::DRX, 5}, {"ROR", &ref::ROR, &ref::AC, 2}, {"TXA", &ref::MOV, &ref::IMP, 2}, {"CPY", &ref::CMP, &ref::DR, 3}, {"RTI", &ref::RETI, &ref::IMP, 6},    // 7
		  {"SEC", &ref::SETC, &ref::IMP, 2}, {"JST8", &ref::TCALL, &ref::IMP, 8}, {"SET1", &ref::SET1, &ref::DR, 4}, {"BBS", &ref::BBS, &ref::DR, 5}, {"ADC", &ref::ADC, &ref::DR, 3}, {"ADC", &ref::ADC, &ref::AB, 4}, {"ADC", &ref::ADC, &ref::DRI, 3}, {"ADC", &ref::ADC, &ref::DRID, 6}, {"ADC", &ref::ADC, &ref::IMM, 2}, {"ADC", &ref::ADC, &ref::DR, 6}, {"EORC", &ref::EOR1, &ref::DR, 5}, {"DEC", &ref::DEC, &ref::DR, 4}, {"DEC", &ref::DEC, &ref::AB, 5}, {"LDY", &ref::MOV, &ref::IMM, 2}, {"PLP", &ref::POP, &ref::IMP, 4}, {"MOV", &ref::MOV, &ref::DR, 5},    // 8
		  {"BCC", &ref::BCC, &ref::RL, 2}, {"JST9", &ref::TCALL, &ref::IMP, 8}, {"CLR1", &ref::CLR1, &ref::DR, 4}, {"BBC", &ref::BBC, &ref::DR, 5}, {"ADC", &ref::ADC, &ref::DRX, 4}, {"ADC", &ref::ADC, &ref::ABX, 5}, {"ADC", &ref::ADC, &ref::ABY, 5}, {"ADC", &ref::ADC, &ref::DRII, 6}, {"ADC", &ref::ADC, &ref::DR, 5}, {"ADC", &ref::ADC, &ref::DR, 5}, {"SBW", &ref::SUBW, &ref::DR, 5}, {"DEC", &ref::DEC, &ref::DRX, 5}, {"DEC", &ref::DEC, &ref::AC, 2}, {"TSX", &ref::MOV, &ref::IMP, 2}, {"DIV", &ref::DIV, &ref::IMP, 12}, {"XCN", &ref::XCN, &ref::AC, 15},    // 9
		  {"CLI", &ref::EI, &ref::IMP, 3}, {"JSTA", &ref::TCALL, &ref::IMP, 8}, {"SET1", &ref::SET1, &ref::DR, 4}, {"BBS", &ref::BBS, &ref::DR, 5}, {"SBC", &ref::SBC, &ref::DR, 3}, {"SBC", &ref::SBC, &ref::AB, 4}, {"SBC", &ref::SBC, &ref::DRI, 3}, {"SBC", &ref::SBC, &ref::DRID, 6}, {"SBC", &ref::SBC, &ref::IMM, 2}, {"SBC", &ref::SBC, &ref::DR, 6}, {"LDC", &ref::MOV1, &ref::DR, 5}, {"INC", &ref::INC, &ref::DR, 4}, {"INC", &ref::INC, &ref::AB, 5}, {"CPY", &ref::CMP, &ref::IMM, 2}, {"PLA", &ref::POP, &ref::IMP, 4}, {"STA", &ref::MOV, &ref::DRI, 4},  // A
		  {"BCS", &ref::BCS, &ref::RL, 2}, {"JSTB", &ref::TCALL, &ref::IMP, 8}, {"CLR1", &ref::CLR1, &ref::DR, 4}, {"BBC", &ref::BBC, &ref::DR, 5}, {"SBC", &ref::SBC, &ref::DRX, 4}, {"SBC", &ref::SBC, &ref::ABX, 5}, {"SBC", &ref::SBC, &ref::ABY, 5}, {"SBC", &ref::SBC, &ref::DRII, 6}, {"SBC", &ref::SBC, &ref::DR, 5}, {"SBC", &ref::SBC, &ref::DR, 5}, {"LDW", &ref::MOVW, &ref::DR, 5}, {"INC", &ref::INC, &ref::DRX, 5}, {"INC", &ref::INC, &ref::AC, 2}, {"TXS", &ref::MOV, &ref::IMP, 2}, {"DAS", &ref::DAS, &ref::AC, 3}, {"LDA", &ref::MOV, &ref::DRI, 4}, // B
		  {"SEI", &ref::DI, &ref::IMP, 3}, {"JSTC", &ref::TCALL, &ref::IMP, 8}, {"SET1", &ref::SET1, &ref::DR, 4}, {"BBS", &ref::BBS, &ref::DR, 5}, {"STA", &ref::MOV, &ref::DR, 4}, {"STA", &ref::MOV, &ref::AB, 5}, {"STA", &ref::MOV, &ref::DRI, 4}, {"STA", &ref::MOV, &ref::DRID, 7}, {"CPX", &ref::CMP, &ref::IMM, 2}, {"STX", &ref::MOV, &ref::AB, 5}, {"STC", &ref::MOV1, &ref::DR, 6}, {"STY", &ref::MOV, &ref::DR, 4}, {"STY", &ref::MOV, &ref::AB, 5}, {"LDX", &ref::MOV, &ref::IMM, 2}, {"PLX", &ref::POP, &ref::IMP, 4}, {"MUL", &ref::MUL, &ref::IMP, 9}, // C
		  {"BNE", &ref::BNE, &ref::RL, 2}, {"JSTD", &ref::TCALL, &ref::IMP, 8}, {"CLR1", &ref::CLR1, &ref::DR, 4}, {"BBC", &ref::BBC, &ref::DR, 5}, {"STA", &ref::MOV, &ref::DRX, 5}, {"STA", &ref::MOV, &ref::ABX, 6}, {"STA", &ref::MOV, &ref::ABY, 6}, {"STA", &ref::MOV, &ref::DRII, 7}, {"STX", &ref::MOV, &ref::DR, 4}, {"STX", &ref::MOV, &ref::DRY, 5}, {"STW", &ref::MOVW, &ref::DR, 5}, {"STY", &ref::MOV, &ref::DRX, 5}, {"DEY", &ref::DEC, &ref::IMP, 2}, {"TYA", &ref::MOV, &ref::IMP, 2}, {"CBNE", &ref::CBNE, &ref::DR, 6}, {"DAA", &ref::DAA, &ref::IMP, 3}, // D
		  {"CLV", &ref::CLRV, &ref::IMP, 2}, {"JSTE", &ref::TCALL, &ref::IMP, 8}, {"SET1", &ref::SET1, &ref::DR, 4}, {"BBS", &ref::BBS, &ref::DR, 5}, {"LDA", &ref::MOV, &ref::DR, 3}, {"LDA", &ref::MOV, &ref::AB, 4}, {"LDA", &ref::MOV, &ref::DRI, 3}, {"LDA", &ref::MOV, &ref::DRID, 6}, {"LDX", &ref::MOV, &ref::IMM, 2}, {"LDX", &ref::MOV, &ref::DR, 3}, {"NOT", &ref::NOT1, &ref::DR, 5}, {"LDY", &ref::MOV, &ref::DR, 3}, {"LDY", &ref::MOV, &ref::AB, 4}, {"NOTC", &ref::NOTC, &ref::IMP, 3}, {"PLY", &ref::POP, &ref::IMP, 4}, {"WAI", &ref::SLEEP, &ref::IMP, 3}, // E
		  {"BEQ", &ref::BEQ, &ref::RL, 2}, {"JSTF", &ref::TCALL, &ref::IMP, 8}, {"CLR1", &ref::CLR1, &ref::DR, 4}, {"BBC", &ref::BBC, &ref::DR, 5}, {"LDA", &ref::MOV, &ref::DRX, 4}, {"LDA", &ref::MOV, &ref::ABX, 5}, {"LDA", &ref::MOV, &ref::ABY, 5}, {"LDA", &ref::MOV, &ref::DRII, 6}, {"LDX", &ref::MOV, &ref::DR, 4}, {"LDX", &ref::MOV, &ref::DRY, 5}, {"MOV", &ref::MOV, &ref::DR, 5}, {"LDY", &ref::MOV, &ref::DRX, 4}, {"INY", &ref::INC, &ref::IMP, 2}, {"TAY", &ref::MOV, &ref::IMP, 2}, {"DBNZ", &ref::DBNZ, &ref::DR, 5}, {"HLT", &ref::STOP, &ref::IMP, 3}, // F


	};
}

uint8_t Spc_700::IMP() { 
	return 0; 
}
uint8_t Spc_700::IMPMF() { 
	return 0; 
}
uint8_t Spc_700::IMPIF() { 
	return 0; 
}
uint8_t Spc_700::IMP8B() { 
	return 0; 
}
uint8_t Spc_700::IMM() { 
	return 0; 
}

uint8_t Spc_700::RL() {
	return 0;
}

uint8_t Spc_700::RLL() {
	return 0;
}

uint8_t Spc_700::DR() {
	return 0;
}

uint8_t Spc_700::DRX() {
	return 0;
}

uint8_t Spc_700::DRY() {
	return 0;
}

uint8_t Spc_700::DRI() {
	return 0;
}

uint8_t Spc_700::DRID() {
	return 0;
}

uint8_t Spc_700::DRII() {
	return 0;
}

uint8_t Spc_700::DRIL() {
	return 0;
}

uint8_t Spc_700::DRIIL() {
	return 0;
}

uint8_t Spc_700::AB() {
	return 0;
}

uint8_t Spc_700::ABX() {
	return 0;
}

uint8_t Spc_700::ABY() {
	return 0;
}

uint8_t Spc_700::ABL() {
	return 0;
}

uint8_t Spc_700::ABIL() {
	return 0;
}

uint8_t Spc_700::SR() {
	return 0;
}

uint8_t Spc_700::SRII() {
	return 0;
}

uint8_t Spc_700::ST() {
	return 0;
}

uint8_t Spc_700::ABD() {
	return 0;
}

uint8_t Spc_700::ABDI() {
	return 0;
}

uint8_t Spc_700::ABDIL() {
	return 0;
}

uint8_t Spc_700::ABDII() {
	return 0;
}

uint8_t Spc_700::AC() {
	return 0;
}

uint8_t Spc_700::IMA() {
	return 0;
}

uint8_t Spc_700::BM() {
	return 0;
}

uint8_t Spc_700::ADC() {
	return 0;
}

uint8_t Spc_700::AND() {
	return 0;
}

uint8_t Spc_700::ADDW() {
	return 0;
}

uint8_t Spc_700::AND1() {
	return 0;
}

uint8_t Spc_700::ASL() {
	return 0;
}

uint8_t Spc_700::BBC() {
	return 0;
}

uint8_t Spc_700::BBS() {
	return 0;
}

uint8_t Spc_700::BSS() {
	return 0;
}

uint8_t Spc_700::BCC() {
	return 0;
}

uint8_t Spc_700::BCS() {
	return 0;
}

uint8_t Spc_700::BEQ() {
	return 0;
}

uint8_t Spc_700::BMI() {
	return 0;
}

uint8_t Spc_700::BNE() {
	return 0;
}

uint8_t Spc_700::BPL() {
	return 0;
}

uint8_t Spc_700::BVC() {
	return 0;
}

uint8_t Spc_700::BVS() {
	return 0;
}

uint8_t Spc_700::BRA() {
	return 0;
}

uint8_t Spc_700::BRK() {
	return 0;
}

uint8_t Spc_700::CALL() {
	return 0;
}

uint8_t Spc_700::CBNE() {
	return 0;
}

uint8_t Spc_700::CLR1() {
	return 0;
}

uint8_t Spc_700::CLRC() {
	return 0;
}

uint8_t Spc_700::CLRP() {
	return 0;
}

uint8_t Spc_700::CLRV() {
	return 0;
}

uint8_t Spc_700::CMP() {
	return 0;
}

uint8_t Spc_700::CMPW() {
	return 0;
}

uint8_t Spc_700::DAA() {
	return 0;
}

uint8_t Spc_700::DAS() {
	return 0;
}

uint8_t Spc_700::DBNZ() {
	return 0;
}

uint8_t Spc_700::DEC() {
	return 0;
}

uint8_t Spc_700::DECW() {
	return 0;
}

uint8_t Spc_700::DI() {
	return 0;
}

uint8_t Spc_700::DIV() {
	return 0;
}

uint8_t Spc_700::EI() {
	return 0;
}

uint8_t Spc_700::EOR() {
	return 0;
}

uint8_t Spc_700::EOR1() {
	return 0;
}

uint8_t Spc_700::INC() {
	return 0;
}

uint8_t Spc_700::INCW() {
	return 0;
}

uint8_t Spc_700::JMP() {
	return 0;
}

uint8_t Spc_700::LSR() {
	return 0;
}

uint8_t Spc_700::MOV() {
	return 0;
}

uint8_t Spc_700::MOV1() {
	return 0;
}

uint8_t Spc_700::MOVW() {
	return 0;
}

uint8_t Spc_700::MUL() {
	return 0;
}

uint8_t Spc_700::NOP() {
	return 0;
}

uint8_t Spc_700::NOT1() {
	return 0;
}

uint8_t Spc_700::NOTC() {
	return 0;
}

uint8_t Spc_700::OR() {
	return 0;
}

uint8_t Spc_700::OR1() {
	return 0;
}

uint8_t Spc_700::PCALL() {
	return 0;
}

uint8_t Spc_700::POP() {
	return 0;
}

uint8_t Spc_700::PUSH() {
	return 0;
}

uint8_t Spc_700::RET() {
	return 0;
}

uint8_t Spc_700::RETI() {
	return 0;
}

uint8_t Spc_700::ROL() {
	return 0;
}

uint8_t Spc_700::ROR() {
	return 0;
}

uint8_t Spc_700::SBC() {
	return 0;
}

uint8_t Spc_700::SET1() {
	return 0;
}

uint8_t Spc_700::SETC() {
	return 0;
}

uint8_t Spc_700::SETP() {
	return 0;
}

uint8_t Spc_700::SLEEP() {
	return 0;
}

uint8_t Spc_700::STOP() {
	return 0;
}

uint8_t Spc_700::SUBW() {
	return 0;
}

uint8_t Spc_700::TCALL() {
	return 0;
}

uint8_t Spc_700::TCLR1() {
	return 0;
}

uint8_t Spc_700::TSET() {
	return 0;
}

uint8_t Spc_700::XCN() {
	return 0;
}

uint8_t Spc_700::XXX() {
	return 0;
}

void Spc_700::clock() {
}

void Spc_700::reset() {
}

uint8_t Spc_700::fetch() {
	return 0;
}

uint8_t Spc_700::read(uint16_t) {
	return 0;
}

void Spc_700::write(uint16_t, uint8_t) {
}

uint8_t Spc_700::getflag(Flag) const {
	return 0;
}

void Spc_700::setflag(Flag, bool) {
}

uint8_t Spc_700::readaccumulator() const {
	return 0;
}

void Spc_700::writeaccumulator(uint8_t) {
}

uint8_t Spc_700::readX() const {
	return 0;
}

uint8_t Spc_700::readY() const {
	return 0;
}

void Spc_700::writeX(uint8_t) {
}

void Spc_700::writeY(uint8_t) {
}

uint8_t Spc_700::fetch8() {
	return 0;
}

uint8_t Spc_700::fetch16() {
	return 0;
}

void Spc_700::push8(uint8_t) {
}

void Spc_700::push16(uint16_t) {
}

uint8_t Spc_700::pull8() {
	return 0;
}

uint16_t Spc_700::pull16() {
	return 0;
}

