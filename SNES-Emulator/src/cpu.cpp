#include "../include/cpu.h"
#include "../include/bus.h"
#include "../include/memory.h"

CPU_65816::CPU_65816() {
	using ref = CPU_65816; //referential access

	// 256-entry opcode lookup. Cycle counts are the base instruction times
	lookup =
	{   //0                               //1                                //2                              //3                              //4                              //5                              //6                              //7                                //8                              //9                                 //A                              //B                              //C                              //D                              //E                              //F
		{"BRK", &ref::BRK, &ref::ST, 7}, {"ORA", &ref::ORA, &ref::DRID, 6}, {"COP", &ref::COP, &ref::ST, 7}, {"ORA", &ref::ORA, &ref::SR, 4}, {"TSB", &ref::TSB, &ref::DR, 5}, {"ORA", &ref::ORA, &ref::DR, 3}, {"ASL", &ref::ASL, &ref::DR, 5}, {"ORA", &ref::ORA, &ref::DRIL, 6}, {"PHP", &ref::PHP, &ref::ST, 3}, {"ORA", &ref::ORA, &ref::IMPMF, 2}, {"ASL", &ref::ASL, &ref::AC, 2}, {"PHD", &ref::PHD, &ref::ST, 4}, {"TSB", &ref::TSB, &ref::AB, 6}, {"ORA", &ref::ORA, &ref::AB, 4}, {"ASL", &ref::ASL, &ref::AB, 6}, {"ORA", &ref::ORA, &ref::ABL, 5}, // 0
		{"BPL", &ref::BPL, &ref::RL, 2}, {"ORA", &ref::ORA, &ref::DRII, 5}, {"ORA", &ref::ORA, &ref::DRI, 5}, {"ORA", &ref::ORA, &ref::SRII, 7}, {"TRB", &ref::TRB, &ref::DR, 5}, {"ORA", &ref::ORA, &ref::DRX, 4}, {"ASL", &ref::ASL, &ref::DRX, 6}, {"ORA", &ref::ORA, &ref::DRIIL, 6}, {"CLC", &ref::CLC, &ref::IMP, 2}, {"ORA", &ref::ORA, &ref::ABY, 4}, {"INC", &ref::INC, &ref::AC, 2}, {"TCS", &ref::TCS, &ref::IMP, 2}, {"TRB", &ref::TRB, &ref::AB, 6}, {"ORA", &ref::ORA, &ref::ABX, 4}, {"ASL", &ref::ASL, &ref::ABX, 7}, {"ORA", &ref::ORA, &ref::ABIL, 5}, // 1
		{"JSR", &ref::JSR, &ref::AB, 6}, {"AND", &ref::AND, &ref::DRID, 6}, {"JSL", &ref::JSL, &ref::ABL, 8}, {"AND", &ref::AND, &ref::SR, 4}, {"BIT", &ref::BIT, &ref::DR, 3}, {"AND", &ref::AND, &ref::DR, 3}, {"ROL", &ref::ROL, &ref::DR, 5}, {"AND", &ref::AND, &ref::DRIL, 6}, {"PLP", &ref::PLP, &ref::ST, 4}, {"AND", &ref::AND, &ref::IMPMF, 2}, {"ROL", &ref::ROL, &ref::AC, 2}, {"PLD", &ref::PLD, &ref::ST, 5}, {"BIT", &ref::BIT, &ref::AB, 4}, {"AND", &ref::AND, &ref::AB, 4}, {"ROL", &ref::ROL, &ref::AB, 6}, {"AND", &ref::AND, &ref::ABL, 5}, // 2
		{"BMI", &ref::BMI, &ref::RL, 2}, {"AND", &ref::AND, &ref::DRII, 5}, {"AND", &ref::AND, &ref::DRI, 5}, {"AND", &ref::AND, &ref::SRII, 7}, {"BIT", &ref::BIT, &ref::DRX, 4}, {"AND", &ref::AND, &ref::DRX, 4}, {"ROL", &ref::ROL, &ref::DRX, 6}, {"AND", &ref::AND, &ref::DRIIL, 6}, {"SEC", &ref::SEC, &ref::IMP, 2}, {"AND", &ref::AND, &ref::ABY, 4}, {"DEC", &ref::DEC, &ref::AC, 2}, {"TSC", &ref::TSC, &ref::IMP, 2}, {"BIT", &ref::BIT, &ref::ABX, 4}, {"AND", &ref::AND, &ref::ABX, 4}, {"ROL", &ref::ROL, &ref::ABX, 7}, {"AND", &ref::AND, &ref::ABIL, 5}, // 3
		{"RTI", &ref::RTI, &ref::ST, 7}, {"EOR", &ref::EOR, &ref::DRID, 6}, {"WDM", &ref::WDM, &ref::IMP8B, 2}, {"EOR", &ref::EOR, &ref::SR, 4}, {"MVP", &ref::MVP, &ref::BM, 7}, {"EOR", &ref::EOR, &ref::DR, 3}, {"LSR", &ref::LSR, &ref::DR, 5}, {"EOR", &ref::EOR, &ref::DRIL, 6}, {"PHA", &ref::PHA, &ref::ST, 3}, {"EOR", &ref::EOR, &ref::IMPMF, 2}, {"LSR", &ref::LSR, &ref::AC, 2}, {"PHK", &ref::PHK, &ref::ST, 3}, {"JMP", &ref::JMP, &ref::AB, 3}, {"EOR", &ref::EOR, &ref::AB, 4}, {"LSR", &ref::LSR, &ref::AB, 6}, {"EOR", &ref::EOR, &ref::ABL, 5}, // 4
		{"BVC", &ref::BVC, &ref::RL, 2}, {"EOR", &ref::EOR, &ref::DRII, 5}, {"EOR", &ref::EOR, &ref::DRI, 5}, {"EOR", &ref::EOR, &ref::SRII, 7}, {"MVN", &ref::MVN, &ref::BM, 7}, {"EOR", &ref::EOR, &ref::DRX, 4}, {"LSR", &ref::LSR, &ref::DRX, 6}, {"EOR", &ref::EOR, &ref::DRIIL, 6}, {"CLI", &ref::CLI, &ref::IMP, 2}, {"EOR", &ref::EOR, &ref::ABY, 4}, {"PHY", &ref::PHY, &ref::ST, 3}, {"TCD", &ref::TCD, &ref::IMP, 2}, {"JML", &ref::JML, &ref::ABL, 4}, {"EOR", &ref::EOR, &ref::ABX, 4}, {"LSR", &ref::LSR, &ref::ABX, 7}, {"EOR", &ref::EOR, &ref::ABIL, 5}, // 5
		{"RTS", &ref::RTS, &ref::ST, 6}, {"ADC", &ref::ADC, &ref::DRID, 6}, {"PER", &ref::PER, &ref::RLL, 6}, {"ADC", &ref::ADC, &ref::SR, 4}, {"STZ", &ref::STZ, &ref::DR, 3}, {"ADC", &ref::ADC, &ref::DR, 3}, {"ROR", &ref::ROR, &ref::DR, 5}, {"ADC", &ref::ADC, &ref::DRIL, 6}, {"PLA", &ref::PLA, &ref::ST, 4}, {"ADC", &ref::ADC, &ref::IMPMF, 2}, {"ROR", &ref::ROR, &ref::AC, 2}, {"RTL", &ref::RTL, &ref::ST, 6}, {"JMP", &ref::JMP, &ref::ABD, 5}, {"ADC", &ref::ADC, &ref::AB, 4}, {"ROR", &ref::ROR, &ref::AB, 6}, {"ADC", &ref::ADC, &ref::ABL, 5}, // 6
		{"BVS", &ref::BVS, &ref::RL, 2}, {"ADC", &ref::ADC, &ref::DRII, 5}, {"ADC", &ref::ADC, &ref::DRI, 5}, {"ADC", &ref::ADC, &ref::SRII, 7}, {"STZ", &ref::STZ, &ref::DRX, 4}, {"ADC", &ref::ADC, &ref::DRX, 4}, {"ROR", &ref::ROR, &ref::DRX, 6}, {"ADC", &ref::ADC, &ref::DRIIL, 6}, {"SEI", &ref::SEI, &ref::IMP, 2}, {"ADC", &ref::ADC, &ref::ABY, 4}, {"PLY", &ref::PLY, &ref::ST, 4}, {"TDC", &ref::TDC, &ref::IMP, 2}, {"JMP", &ref::JMP, &ref::ABDI, 6}, {"ADC", &ref::ADC, &ref::ABX, 4}, {"ROR", &ref::ROR, &ref::ABX, 7}, {"ADC", &ref::ADC, &ref::ABIL, 5}, // 7
		{"BRA", &ref::BRA, &ref::RL, 2}, {"STA", &ref::STA, &ref::DRID, 6}, {"BRL", &ref::BRL, &ref::RLL, 4}, {"STA", &ref::STA, &ref::SR, 4}, {"STY", &ref::STY, &ref::DR, 3}, {"STA", &ref::STA, &ref::DR, 3}, {"STX", &ref::STX, &ref::DR, 3}, {"STA", &ref::STA, &ref::DRIL, 6}, {"DEY", &ref::DEY, &ref::IMP, 2}, {"BIT", &ref::BIT, &ref::IMPMF, 2}, {"TXA", &ref::TXA, &ref::IMP, 2}, {"PHB", &ref::PHB, &ref::ST, 3}, {"STY", &ref::STY, &ref::AB, 4}, {"STA", &ref::STA, &ref::AB, 4}, {"STX", &ref::STX, &ref::AB, 4}, {"STA", &ref::STA, &ref::ABL, 5}, // 8
		{"BCC", &ref::BCC, &ref::RL, 2}, {"STA", &ref::STA, &ref::DRII, 6}, {"STA", &ref::STA, &ref::DRI, 5}, {"STA", &ref::STA, &ref::SRII, 7}, {"STY", &ref::STY, &ref::DRX, 4}, {"STA", &ref::STA, &ref::DRX, 4}, {"STX", &ref::STX, &ref::DRY, 4}, {"STA", &ref::STA, &ref::DRIIL, 6}, {"TYA", &ref::TYA, &ref::IMP, 2}, {"STA", &ref::STA, &ref::ABY, 5}, {"TXS", &ref::TXS, &ref::IMP, 2}, {"TXY", &ref::TXY, &ref::IMP, 2}, {"STZ", &ref::STZ, &ref::AB, 4}, {"STA", &ref::STA, &ref::ABX, 5}, {"STZ", &ref::STZ, &ref::ABX, 5}, {"STA", &ref::STA, &ref::ABIL, 5}, // 9
		{"LDY", &ref::LDY, &ref::IMPIF, 2}, {"LDA", &ref::LDA, &ref::DRID, 6}, {"LDX", &ref::LDX, &ref::IMPIF, 2}, {"LDA", &ref::LDA, &ref::SR, 4}, {"LDY", &ref::LDY, &ref::DR, 3}, {"LDA", &ref::LDA, &ref::DR, 3}, {"LDX", &ref::LDX, &ref::DR, 3}, {"LDA", &ref::LDA, &ref::DRIL, 6}, {"TAY", &ref::TAY, &ref::IMP, 2}, {"LDA", &ref::LDA, &ref::IMPMF, 2}, {"TAX", &ref::TAX, &ref::IMP, 2}, {"PLB", &ref::PLB, &ref::ST, 4}, {"LDY", &ref::LDY, &ref::AB, 4}, {"LDA", &ref::LDA, &ref::AB, 4}, {"LDX", &ref::LDX, &ref::AB, 4}, {"LDA", &ref::LDA, &ref::ABL, 5}, // A
		{"BCS", &ref::BCS, &ref::RL, 2}, {"LDA", &ref::LDA, &ref::DRII, 5}, {"LDA", &ref::LDA, &ref::DRI, 5}, {"LDA", &ref::LDA, &ref::SRII, 7}, {"LDY", &ref::LDY, &ref::DRX, 4}, {"LDA", &ref::LDA, &ref::DRX, 4}, {"LDX", &ref::LDX, &ref::DRY, 4}, {"LDA", &ref::LDA, &ref::DRIIL, 6}, {"CLV", &ref::CLV, &ref::IMP, 2}, {"LDA", &ref::LDA, &ref::ABY, 4}, {"TSX", &ref::TSX, &ref::IMP, 2}, {"TYX", &ref::TYX, &ref::IMP, 2}, {"LDY", &ref::LDY, &ref::ABX, 4}, {"LDA", &ref::LDA, &ref::ABX, 4}, {"LDX", &ref::LDX, &ref::ABY, 4}, {"LDA", &ref::LDA, &ref::ABIL, 5}, // B
		{"CPY", &ref::CPY, &ref::IMPIF, 2}, {"CMP", &ref::CMP, &ref::DRID, 6}, {"REP", &ref::REP, &ref::IMP8B, 3}, {"CMP", &ref::CMP, &ref::SR, 4}, {"CPY", &ref::CPY, &ref::DR, 3}, {"CMP", &ref::CMP, &ref::DR, 3}, {"DEC", &ref::DEC, &ref::DR, 5}, {"CMP", &ref::CMP, &ref::DRIL, 6}, {"INY", &ref::INY, &ref::IMP, 2}, {"CMP", &ref::CMP, &ref::IMPMF, 2}, {"DEX", &ref::DEX, &ref::IMP, 2}, {"WAI", &ref::WAI, &ref::IMP, 3}, {"CPY", &ref::CPY, &ref::AB, 4}, {"CMP", &ref::CMP, &ref::AB, 4}, {"DEC", &ref::DEC, &ref::AB, 6}, {"CMP", &ref::CMP, &ref::ABL, 5}, // C
		{"BNE", &ref::BNE, &ref::RL, 2}, {"CMP", &ref::CMP, &ref::DRII, 5}, {"CMP", &ref::CMP, &ref::DRI, 5}, {"CMP", &ref::CMP, &ref::SRII, 7}, {"PEI", &ref::PEI, &ref::DRI, 6}, {"CMP", &ref::CMP, &ref::DRX, 4}, {"DEC", &ref::DEC, &ref::DRX, 6}, {"CMP", &ref::CMP, &ref::DRIIL, 6}, {"CLD", &ref::CLD, &ref::IMP, 2}, {"CMP", &ref::CMP, &ref::ABY, 4}, {"PHX", &ref::PHX, &ref::ST, 3}, {"STP", &ref::STP, &ref::IMP, 3}, {"JML", &ref::JML, &ref::ABDIL, 6}, {"CMP", &ref::CMP, &ref::ABX, 4}, {"DEC", &ref::DEC, &ref::ABX, 7}, {"CMP", &ref::CMP, &ref::ABIL, 5}, // D
		{"CPX", &ref::CPX, &ref::IMPIF, 2}, {"SBC", &ref::SBC, &ref::DRID, 6}, {"SEP", &ref::SEP, &ref::IMP8B, 3}, {"SBC", &ref::SBC, &ref::SR, 4}, {"CPX", &ref::CPX, &ref::DR, 3}, {"SBC", &ref::SBC, &ref::DR, 3}, {"INC", &ref::INC, &ref::DR, 5}, {"SBC", &ref::SBC, &ref::DRIL, 6}, {"INX", &ref::INX, &ref::IMP, 2}, {"SBC", &ref::SBC, &ref::IMPMF, 2}, {"NOP", &ref::NOP, &ref::IMP, 2}, {"XBA", &ref::XBA, &ref::IMP, 3}, {"CPX", &ref::CPX, &ref::AB, 4}, {"SBC", &ref::SBC, &ref::AB, 4}, {"INC", &ref::INC, &ref::AB, 6}, {"SBC", &ref::SBC, &ref::ABL, 5}, // E
		{"BEQ", &ref::BEQ, &ref::RL, 2}, {"SBC", &ref::SBC, &ref::DRII, 5}, {"SBC", &ref::SBC, &ref::DRI, 5}, {"SBC", &ref::SBC, &ref::SRII, 7}, {"PEA", &ref::PEA, &ref::AB, 5}, {"INC", &ref::INC, &ref::DRX, 6}, {"SBC", &ref::SBC, &ref::DRX, 4}, {"SBC", &ref::SBC, &ref::DRIIL, 6}, {"SED", &ref::SED, &ref::IMP, 2}, {"SBC", &ref::SBC, &ref::ABY, 4}, {"PLX", &ref::PLX, &ref::ST, 4}, {"XCE", &ref::XCE, &ref::IMP, 2}, {"JSR", &ref::JSR, &ref::ABDI, 8}, {"SBC", &ref::SBC, &ref::ABX, 4}, {"INC", &ref::INC, &ref::ABX, 7}, {"SBC", &ref::SBC, &ref::ABIL, 5}, // F
	};
}

CPU_65816::CPU_65816(Bus& connectedBus): CPU_65816()
{
	bus = &connectedBus;
}

uint8_t CPU_65816::read(uint32_t address)
{
	if (bus == nullptr)
		return 0xFF;

	return bus->read(address & 0xFFFFFF);
}

void CPU_65816::write(uint32_t address, uint8_t data)
{
	if (bus != nullptr)
		bus->write(address & 0xFFFFFF, data);
}

uint8_t CPU_65816::getflag(Flag flag) const
{
	return (status & static_cast<uint8_t>(flag)) != 0;
}

void CPU_65816::setflag(Flag flag, bool value)
{
	const uint8_t mask = static_cast<uint8_t>(flag);

	if (value)
		status |= mask;
	else
		status &= static_cast<uint8_t>(~mask);
}

void CPU_65816::reset()
{
	A = 0x0000;
	X = 0x0000;
	Y = 0x0000;
	STKP = 0x0000;
	DBR = 0x00;
	PBR = 0x00;
	D = 0x0000;
	PC = 0x0000;
	status = 0x00;
	fetched = 0x0000;
	addr_abs = 0x000000;
	addr_rel = 0x0000;
	opcode = 0x00;
	cycles = 0;
}

void CPU_65816::clock()
{
	if (cycles == 0)
	{
		const uint32_t programAddress =
			(static_cast<uint32_t>(PBR) << 16) | PC;

		opcode = read(programAddress);
		PC = static_cast<uint16_t>(PC + 1);

		cycles = lookup[opcode].cycles;

		const uint8_t additionalAddressCycles =
			(this->*lookup[opcode].addrmode)();
		const uint8_t additionalOperationCycles =
			(this->*lookup[opcode].operate)();

		cycles += additionalAddressCycles & additionalOperationCycles;
	}

	if (cycles > 0)
		--cycles;
}
