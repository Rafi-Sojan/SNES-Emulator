#include "../include/cpu.h"
#include "../include/bus.h"
#include "../include/memory.h"

CPU_65816::CPU_65816() {
	using ref = CPU_65816; //referential access

	//opcode lookup
	lookup =
	{
		{"BRK", &ref::BRK, },{"ORA", &ref::ORA, },{"COP", &ref::COP, },{"ORA", &ref::ORA, },{"TSB", &ref::TSB, },{"ORA", &ref::ORA, },{"ASL", &ref::ASL, },{"ORA", &ref::ORA, },{"PHP", &ref::PHP, },{"ORA", &ref::ORA, },{"ASL", &ref::ASL, },{"PHD", &ref::PHD, },{"TSB", &ref::TSB, },{"ORA", &ref::ORA, },{"ASL", &ref::ASL, },{"ORA", &ref::ORA, },  // 0
		{"BPL", &ref::BPL, },{"ORA", &ref::ORA, },{"ORA", &ref::ORA, },{"ORA", &ref::ORA, },{"TRB", &ref::TRB, },{"ORA", &ref::ORA, },{"ASL", &ref::ASL, },{"ORA", &ref::ORA, },{"CLC", &ref::CLC, },{"ORA", &ref::ORA, },{"INC", &ref::INC, },{"TCS", &ref::TCS, },{"TRB", &ref::TRB, },{"ORA", &ref::ORA, },{"ASL", &ref::ASL, },{"ORA", &ref::ORA, },  // 1
		{"JSR", &ref::JSR, },{"AND", &ref::AND, },{"JSL", &ref::JSL, },{"AND", &ref::AND, },{"BIT", &ref::BIT, },{"AND", &ref::AND, },{"ROL", &ref::ROL, },{"AND", &ref::AND, },{"PLP", &ref::PLP, },{"AND", &ref::AND, },{"ROL", &ref::ROL, },{"PLD", &ref::PLD, },{"BIT", &ref::BIT, },{"AND", &ref::AND, },{"ROL", &ref::ROL, },{"AND", &ref::AND, },  // 2
		{"BMI", &ref::BMI, },{"AND", &ref::AND, },{"AND", &ref::AND, },{"AND", &ref::AND, },{"BIT", &ref::BIT, },{"AND", &ref::AND, },{"ROL", &ref::ROL, },{"AND", &ref::AND, },{"SEC", &ref::SEC, },{"AND", &ref::AND, },{"DEC", &ref::DEC, },{"TSC", &ref::TSC, },{"BIT", &ref::BIT, },{"AND", &ref::AND, },{"ROL", &ref::ROL, },{"AND", &ref::AND, },  // 3
		{"RTI"},{"EOR"},{"WDM"},{"EOR"},{"MVP"},{"EOR"},{"LSR"},{"EOR"},{"PHA"},{"EOR"},{"LSR"},{"PHK"},{"JMP"},{"EOR"},{"LSR"},{"EOR"},  // 4
		{"BVC"},{"EOR"},{"EOR"},{"EOR"},{"MVN"},{"EOR"},{"LSR"},{"EOR"},{"CLI"},{"EOR"},{"PHY"},{"TCD"},{"JMP"},{"EOR"},{"LSR"},{"EOR"},  // 5
		{"RTS"},{"ADC"},{"PER"},{"ADC"},{"STZ"},{"ADC"},{"ROR"},{"ADC"},{"PLA"},{"ADC"},{"ROR"},{"RTL"},{"JMP"},{"ADC"},{"ROR"},{"ADC"},  // 6
		{"BVS"},{"ADC"},{"ADC"},{"ADC"},{"STZ"},{"ADC"},{"ROR"},{"ADC"},{"SEI"},{"ADC"},{"PLY"},{"TDC"},{"JMP"},{"ADC"},{"ROR"},{"ADC"},  // 7
		{"BRA"},{"STA"},{"BRL"},{"STA"},{"STY"},{"STA"},{"STX"},{"STA"},{"DEY"},{"BIT"},{"TXA"},{"PHB"},{"STY"},{"STA"},{"STX"},{"STA"},  // 8
		{"BCC"},{"STA"},{"STA"},{"STA"},{"STY"},{"STA"},{"STX"},{"STA"},{"TYA"},{"STA"},{"TXS"},{"TXY"},{"STZ"},{"STA"},{"STZ"},{"STA"},  // 9
		{"LDY"},{"LDA"},{"LDX"},{"LDA"},{"LDY"},{"LDA"},{"LDX"},{"LDA"},{"TAY"},{"LDA"},{"TAX"},{"PLB"},{"LDY"},{"LDA"},{"LDX"},{"LDA"},  // A
		{"BCS"},{"LDA"},{"LDA"},{"LDA"},{"LDY"},{"LDA"},{"LDX"},{"LDA"},{"CLV"},{"LDA"},{"TSX"},{"TYX"},{"LDY"},{"LDA"},{"LDX"},{"LDA"},  // B
		{"CPY"},{"CMP"},{"REP"},{"CMP"},{"CPY"},{"CMP"},{"DEC"},{"CMP"},{"INY"},{"CMP"},{"DEX"},{"WAI"},{"CPY"},{"CMP"},{"DEC"},{"CMP"},  // C
		{"BNE"},{"CMP"},{"CMP"},{"CMP"},{"PEI"},{"CMP"},{"DEC"},{"CMP"},{"CLD"},{"CMP"},{"PHX"},{"STP"},{"JML"},{"CMP"},{"DEC"},{"CMP"},  // D
		{"CPX"},{"SBC"},{"SEP"},{"SBC"},{"CPX"},{"SBC"},{"INC"},{"SBC"},{"INX"},{"SBC"},{"NOP"},{"XBA"},{"CPX"},{"SBC"},{"INC"},{"SBC"},  // E
		{"BEQ"},{"SBC"},{"SBC"},{"SBC"},{"PEA"},{"INC"},{"SBC"},{"SBC"},{"SED"},{"SBC"},{"PLX"},{"XCE"},{"JSR"},{"SBC"},{"INC"},{"SBC"}   // F
	};
}