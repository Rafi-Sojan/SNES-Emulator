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

bool CPU_65816::accumulatoris8bit() const
{
	return E || getflag(Flag::M);
}

bool CPU_65816::indexis8bit() const
{
	return E || getflag(Flag::X);
}

uint16_t CPU_65816::accumulatorMask() const
{
	return accumulatoris8bit() ? 0x00FF : 0xFFFF;
}

uint16_t CPU_65816::indexMask() const
{
	return indexis8bit() ? 0x00FF : 0xFFFF;
}

uint16_t CPU_65816::accumulatorSignBit() const
{
	return accumulatoris8bit() ? 0x0080 : 0x8000;
}

uint16_t CPU_65816::indexSignBit() const
{
	return indexis8bit() ? 0x0080 : 0x8000;
}

uint16_t CPU_65816::readaccumulator() const
{
	return A & accumulatorMask();
}

void CPU_65816::writeaccumulator(uint16_t value)
{
	if (accumulatoris8bit())
		A = static_cast<uint16_t>((A & 0xFF00) | (value & 0x00FF));
	else
		A = value;
}

uint16_t CPU_65816::readX() const
{
	return X & indexMask();
}

uint16_t CPU_65816::readY() const
{
	return Y & indexMask();
}

void CPU_65816::writeX(uint16_t value)
{
	X = indexis8bit() ? value & 0x00FF : value;
}

void CPU_65816::writeY(uint16_t value)
{
	Y = indexis8bit() ? value & 0x00FF : value;
}

void CPU_65816::forceEmulationWidths()
{
	if (E)
	{
		status |= static_cast<uint8_t>(Flag::M) | static_cast<uint8_t>(Flag::X);
		STKP = static_cast<uint16_t>(0x0100 | (STKP & 0x00FF));
		X &= 0x00FF;
		Y &= 0x00FF;
	}
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
	STKP = 0x01FF;
	DBR = 0x00;
	PBR = 0x00;
	D = 0x0000;
	E = true;
	status = static_cast<uint8_t>(Flag::M) | static_cast<uint8_t>(Flag::X) | static_cast<uint8_t>(Flag::I);
	fetched = 0x0000;
	addr_abs = 0x000000;
	addr_rel = 0x0000;
	opcode = 0x00;
	cycles = 0;
	waiting = false;
	stopped = false;
	irqLine = false;
	nmiPendingFlag = false;
	operandKind = OperandKind::None;
	forceEmulationWidths();
	PC = read16(0x00FFFC);
}

void CPU_65816::clock()
{
	if (stopped)
		return;

	if (cycles > 0)
	{
		--cycles;
		return;
	}

	if (waiting)
	{
		if (!nmiPendingFlag && !irqPending())
			return;

		waiting = false;
	}

	servicePendingInterrupts();
	if (cycles > 0)
	{
		--cycles;
		return;
	}

	{
		operandKind = OperandKind::None;
		fetched = 0x0000;
		addr_abs = 0x000000;
		addr_rel = 0x0000;

		opcode = fetch8();
		cycles = lookup[opcode].cycles;
		const uint8_t additionalAddressCycles = (this->*lookup[opcode].addrmode)();
		const uint8_t additionalOperationCycles = (this->*lookup[opcode].operate)();

		cycles = static_cast<uint8_t>(cycles +
			(additionalAddressCycles & additionalOperationCycles));
	}

	if (cycles > 0)
	{
		--cycles;
	}
}

uint32_t CPU_65816::make24bitaddress(uint8_t bank, uint16_t offset) {
	return (static_cast<uint32_t>(bank) << 16) | offset; // creates 24 bit address by performing bitwise OR operation between the 8 bit data bank address and 16 bit offset address
}

uint16_t CPU_65816::read16(uint32_t address) {
	const uint32_t bank = address & 0xFF0000;
	const uint16_t offset = static_cast<uint16_t>(address & 0xFFFF);
	uint8_t low = read(bank | offset);
	uint8_t high = read(bank | static_cast<uint16_t>(offset + 1));

	return static_cast<uint16_t>(low) | (static_cast<uint16_t>(high) << 8);
}

uint32_t CPU_65816::read24(uint32_t address) {
	const uint32_t bank = address & 0xFF0000;
	const uint16_t offset = static_cast<uint16_t>(address & 0xFFFF);
	uint32_t b0 = read(bank | offset);
	uint32_t b1 = read(bank | static_cast<uint16_t>(offset + 1));
	uint32_t b2 = read(bank | static_cast<uint16_t>(offset + 2));

	return b0 | (b1 << 8) | (b2 << 16);
}

uint8_t CPU_65816::fetch8()
{
	const uint8_t value = read(make24bitaddress(PBR, PC));
	PC = static_cast<uint16_t>(PC + 1);
	return value;
}

uint16_t CPU_65816::fetch16()
{
	const uint16_t low = fetch8();
	const uint16_t high = fetch8();
	return static_cast<uint16_t>(low | (high << 8));
}

uint32_t CPU_65816::fetch24()
{
	const uint32_t low = fetch8();
	const uint32_t middle = fetch8();
	const uint32_t high = fetch8();
	return low | (middle << 8) | (high << 16);
}

void CPU_65816::push8(uint8_t value)
{
	const uint32_t address = E ? make24bitaddress(0x00, static_cast<uint16_t>(0x0100 | (STKP & 0x00FF))) : make24bitaddress(0x00, STKP);
	write(address, value);

	if (E)
		STKP = static_cast<uint16_t>(0x0100 | ((STKP - 1) & 0x00FF));
	else
		STKP = static_cast<uint16_t>(STKP - 1);
}

uint8_t CPU_65816::pull8()
{
	if (E)
		STKP = static_cast<uint16_t>(0x0100 | ((STKP + 1) & 0x00FF));
	else
		STKP = static_cast<uint16_t>(STKP + 1);

	const uint32_t address = E
		? make24bitaddress(0x00, static_cast<uint16_t>(0x0100 | (STKP & 0x00FF)))
		: make24bitaddress(0x00, STKP);
	return read(address);
}

void CPU_65816::push16(uint16_t value)
{
	push8(static_cast<uint8_t>(value >> 8));
	push8(static_cast<uint8_t>(value & 0x00FF));
}

uint16_t CPU_65816::pull16()
{
	const uint16_t low = pull8();
	const uint16_t high = pull8();
	return static_cast<uint16_t>(low | (high << 8));
}

uint16_t CPU_65816::readOperandWidth(bool indexWidth)
{
	const uint16_t mask = indexWidth ? indexMask() : accumulatorMask();

	if (operandKind == OperandKind::Immediate)
		return fetched & mask;

	if (operandKind == OperandKind::Accumulator)
		return readaccumulator();

	if (operandKind == OperandKind::Memory)
		return mask == 0x00FF ? read(addr_abs) : read16(addr_abs);

	return fetched & mask;
}

uint16_t CPU_65816::readOperand()
{
	return readOperandWidth(false);
}

void CPU_65816::writeOperand(uint16_t value)
{
	value &= accumulatorMask();

	if (operandKind == OperandKind::Accumulator)
	{
		writeaccumulator(value);
		return;
	}

	if (operandKind != OperandKind::Memory)
		return;

	write(addr_abs, static_cast<uint8_t>(value & 0x00FF));
	if (!accumulatoris8bit())
		write((addr_abs & 0xFF0000) | static_cast<uint16_t>((addr_abs + 1) & 0xFFFF), static_cast<uint8_t>(value >> 8));
}

void CPU_65816::updateNZ(uint16_t value, uint16_t mask, uint16_t signBit)
{
	value &= mask;
	setflag(Flag::Z, value == 0);
	setflag(Flag::N, (value & signBit) != 0);
}

void CPU_65816::compareValues(uint16_t left, uint16_t right,
	uint16_t mask, uint16_t signBit)
{
	left &= mask;
	right &= mask;
	const uint32_t result = static_cast<uint32_t>(left) - right;
	setflag(Flag::C, left >= right);
	updateNZ(static_cast<uint16_t>(result), mask, signBit);
}

// Addressing modes 

uint8_t CPU_65816::IMP()
{
	operandKind = OperandKind::None;
	return 0;
}

uint8_t CPU_65816::IMPMF()
{
	operandKind = OperandKind::Immediate;
	fetched = accumulatoris8bit() ? fetch8() : fetch16();
	return 0;
}

uint8_t CPU_65816::IMPIF()
{
	operandKind = OperandKind::Immediate;
	fetched = indexis8bit() ? fetch8() : fetch16();
	return 0;
}

uint8_t CPU_65816::IMP8B()
{
	operandKind = OperandKind::Immediate;
	fetched = fetch8();
	return 0;
}

uint8_t CPU_65816::IMM()
{
	return IMP8B();
}

uint8_t CPU_65816::RL()
{
	operandKind = OperandKind::Relative;
	addr_rel = static_cast<int8_t>(fetch8());
	return 0;
}

uint8_t CPU_65816::RLL()
{
	operandKind = OperandKind::Relative;
	addr_rel = static_cast<int16_t>(fetch16());
	return 0;
}

uint8_t CPU_65816::DR()
{
	const uint16_t offset = static_cast<uint16_t>(D + fetch8());
	addr_abs = make24bitaddress(DBR, offset);
	operandKind = OperandKind::Memory;
	return 0;
}

uint8_t CPU_65816::DRX()
{
	const uint16_t offset = static_cast<uint16_t>(D + fetch8() + readX());
	addr_abs = make24bitaddress(DBR, offset);
	operandKind = OperandKind::Memory;
	return 0;
}

uint8_t CPU_65816::DRY()
{
	const uint16_t offset = static_cast<uint16_t>(D + fetch8() + readY());
	addr_abs = make24bitaddress(DBR, offset);
	operandKind = OperandKind::Memory;
	return 0;
}

uint8_t CPU_65816::DRI()
{
	const uint16_t pointerAddress = static_cast<uint16_t>(D + fetch8());
	fetched = read16(pointerAddress);
	addr_abs = make24bitaddress(DBR, fetched);
	operandKind = OperandKind::Memory;
	return 0;
}

uint8_t CPU_65816::DRID()
{
	const uint16_t pointerAddress = static_cast<uint16_t>(D + fetch8() + readX());
	fetched = read16(pointerAddress);
	addr_abs = make24bitaddress(DBR, fetched);
	operandKind = OperandKind::Memory;
	return 0;
}

uint8_t CPU_65816::DRII()
{
	const uint16_t pointerAddress = static_cast<uint16_t>(D + fetch8());
	fetched = read16(pointerAddress);
	addr_abs = make24bitaddress(DBR, static_cast<uint16_t>(fetched + readY()));
	operandKind = OperandKind::Memory;
	return 0;
}

uint8_t CPU_65816::DRIL()
{
	const uint16_t pointerAddress = static_cast<uint16_t>(D + fetch8());
	addr_abs = read24(pointerAddress);
	fetched = static_cast<uint16_t>(addr_abs);
	operandKind = OperandKind::Memory;
	return 0;
}

uint8_t CPU_65816::DRIIL()
{
	const uint16_t pointerAddress = static_cast<uint16_t>(D + fetch8());
	addr_abs = (read24(pointerAddress) + readY()) & 0x00FFFFFF;
	fetched = static_cast<uint16_t>(addr_abs);
	operandKind = OperandKind::Memory;
	return 0;
}

uint8_t CPU_65816::AB()
{
	fetched = fetch16();
	addr_abs = make24bitaddress(DBR, fetched);
	operandKind = OperandKind::Memory;
	return 0;
}

uint8_t CPU_65816::ABX()
{
	fetched = fetch16();
	addr_abs = make24bitaddress(DBR, static_cast<uint16_t>(fetched + readX()));
	operandKind = OperandKind::Memory;
	return 0;
}

uint8_t CPU_65816::ABY()
{
	fetched = fetch16();
	addr_abs = make24bitaddress(DBR, static_cast<uint16_t>(fetched + readY()));
	operandKind = OperandKind::Memory;
	return 0;
}

uint8_t CPU_65816::ABL()
{
	addr_abs = fetch24();
	fetched = static_cast<uint16_t>(addr_abs);
	operandKind = OperandKind::Memory;
	return 0;
}

uint8_t CPU_65816::ABIL()
{
	addr_abs = (fetch24() + readX()) & 0x00FFFFFF;
	fetched = static_cast<uint16_t>(addr_abs);
	operandKind = OperandKind::Memory;
	return 0;
}

uint8_t CPU_65816::SR()
{
	const uint16_t offset = static_cast<uint16_t>(STKP + fetch8());
	addr_abs = make24bitaddress(0x00, offset);
	operandKind = OperandKind::Memory;
	return 0;
}

uint8_t CPU_65816::SRII()
{
	const uint16_t pointerAddress = static_cast<uint16_t>(STKP + fetch8());
	const uint16_t pointer = read16(pointerAddress);
	addr_abs = make24bitaddress(DBR, static_cast<uint16_t>(pointer + readY()));
	operandKind = OperandKind::Memory;
	return 0;
}

uint8_t CPU_65816::ST()
{
	operandKind = OperandKind::None;
	return 0;
}

uint8_t CPU_65816::ABD()
{
	const uint16_t pointerAddress = fetch16();
	fetched = read16(pointerAddress);
	addr_abs = make24bitaddress(DBR, fetched);
	operandKind = OperandKind::Memory;
	return 0;
}

uint8_t CPU_65816::ABDI()
{
	const uint16_t pointerAddress = static_cast<uint16_t>(fetch16() + readX());
	fetched = read16(make24bitaddress(PBR, pointerAddress));
	addr_abs = make24bitaddress(PBR, fetched);
	operandKind = OperandKind::None;
	return 0;
}

uint8_t CPU_65816::ABDIL()
{
	const uint16_t pointerAddress = fetch16();
	addr_abs = read24(pointerAddress);
	fetched = static_cast<uint16_t>(addr_abs);
	operandKind = OperandKind::None;
	return 0;
}

uint8_t CPU_65816::ABDII()
{
	return ABD();
}

uint8_t CPU_65816::AC()
{
	operandKind = OperandKind::Accumulator;
	return 0;
}

uint8_t CPU_65816::IMA()
{
	return AC();
}

uint8_t CPU_65816::BM()
{
	blockDestinationBank = fetch8();
	blockSourceBank = fetch8();
	operandKind = OperandKind::BlockMove;
	return 0;
}

// Arithmetic and logical instructions

void CPU_65816::adcBinary(uint16_t operand)
{
	const uint16_t mask = accumulatorMask();
	const uint16_t signBit = accumulatorSignBit();
	const uint16_t left = readaccumulator();
	const uint32_t result = static_cast<uint32_t>(left) + (operand & mask) + (getflag(Flag::C) ? 1u : 0u);
	const uint16_t value = static_cast<uint16_t>(result) & mask;

	setflag(Flag::C, result > mask);
	setflag(Flag::V, ((~(left ^ operand) & (left ^ value) & signBit) != 0));
	writeaccumulator(value);
	updateNZ(value, mask, signBit);
}

void CPU_65816::decimalAdc(uint16_t operand)
{
	const uint16_t mask = accumulatorMask();
	const uint16_t signBit = accumulatorSignBit();
	const uint16_t left = readaccumulator();
	const uint16_t right = operand & mask;
	const bool carryIn = getflag(Flag::C);
	uint16_t result = 0;
	uint16_t carry = carryIn ? 1 : 0;

	for (uint16_t shift = 0; shift <= (accumulatoris8bit() ? 4 : 12); shift += 4)
	{
		uint16_t digit = static_cast<uint16_t>(((left >> shift) & 0x0F) + ((right >> shift) & 0x0F) + carry);
		carry = digit > 9 ? 1 : 0;
		if (digit > 9)
			digit = static_cast<uint16_t>(digit - 10);
		result = static_cast<uint16_t>(result | (digit << shift));
	}

	const uint32_t binaryResult = static_cast<uint32_t>(left) + right + (carryIn ? 1u : 0u);
	const uint16_t binaryValue = static_cast<uint16_t>(binaryResult) & mask;
	setflag(Flag::C, carry != 0);
	setflag(Flag::V, ((~(left ^ right) & (left ^ binaryValue) & signBit) != 0));
	writeaccumulator(result);
	updateNZ(result, mask, signBit);
}

void CPU_65816::decimalSbc(uint16_t operand)
{
	const uint16_t mask = accumulatorMask();
	const uint16_t signBit = accumulatorSignBit();
	const uint16_t left = readaccumulator();
	const uint16_t right = operand & mask;
	const bool borrowIn = !getflag(Flag::C);
	uint16_t result = 0;
	int carry = borrowIn ? 0 : 1;

	for (uint16_t shift = 0; shift <= (accumulatoris8bit() ? 4 : 12); shift += 4)
	{
		int digit = static_cast<int>((left >> shift) & 0x0F) - static_cast<int>((right >> shift) & 0x0F) + carry - 1;
		if (digit < 0)
		{
			digit += 10;
			carry = 0;
		}
		else
			carry = 1;
		result = static_cast<uint16_t>(result | (static_cast<uint16_t>(digit) << shift));
	}

	const uint32_t binaryResult = static_cast<uint32_t>(left) - right - (borrowIn ? 1u : 0u);
	const uint16_t binaryValue = static_cast<uint16_t>(binaryResult) & mask;
	setflag(Flag::C, carry != 0);
	setflag(Flag::V, (((left ^ binaryValue) & (left ^ right) & signBit) != 0));
	writeaccumulator(result);
	updateNZ(result, mask, signBit);
}

uint8_t CPU_65816::ADC()
{
	const uint16_t operand = readOperand();
	if (getflag(Flag::D))
		decimalAdc(operand);
	else
		adcBinary(operand);
	return 0;
}

uint8_t CPU_65816::SBC()
{
	const uint16_t mask = accumulatorMask();
	const uint16_t signBit = accumulatorSignBit();
	const uint16_t left = readaccumulator();
	const uint16_t right = readOperand();

	if (getflag(Flag::D))
	{
		decimalSbc(right);
		return 0;
	}

	const uint32_t result = static_cast<uint32_t>(left) - (right & mask) - (getflag(Flag::C) ? 0u : 1u);
	const uint16_t value = static_cast<uint16_t>(result) & mask;
	setflag(Flag::C, result <= mask);
	setflag(Flag::V, (((left ^ value) & (left ^ right) & signBit) != 0));
	writeaccumulator(value);
	updateNZ(value, mask, signBit);
	return 0;
}

uint8_t CPU_65816::AND()
{
	const uint16_t value = readaccumulator() & readOperand();
	writeaccumulator(value);
	updateNZ(value, accumulatorMask(), accumulatorSignBit());
	return 0;
}

uint8_t CPU_65816::EOR()
{
	const uint16_t value = readaccumulator() ^ readOperand();
	writeaccumulator(value);
	updateNZ(value, accumulatorMask(), accumulatorSignBit());
	return 0;
}

uint8_t CPU_65816::ORA()
{
	const uint16_t value = readaccumulator() | readOperand();
	writeaccumulator(value);
	updateNZ(value, accumulatorMask(), accumulatorSignBit());
	return 0;
}

uint8_t CPU_65816::TSB()
{
	const uint16_t value = readOperand();
	setflag(Flag::Z, (value & readaccumulator()) == 0);
	writeOperand(static_cast<uint16_t>(value | readaccumulator()));
	return 0;
}

uint8_t CPU_65816::TRB()
{
	const uint16_t value = readOperand();
	setflag(Flag::Z, (value & readaccumulator()) == 0);
	writeOperand(static_cast<uint16_t>(value & ~readaccumulator()));
	return 0;
}

uint8_t CPU_65816::ASL()
{
	const uint16_t mask = accumulatorMask();
	const uint16_t value = readOperand();
	setflag(Flag::C, (value & accumulatorSignBit()) != 0);
	const uint16_t result = static_cast<uint16_t>(value << 1) & mask;
	writeOperand(result);
	updateNZ(result, mask, accumulatorSignBit());
	return 0;
}

uint8_t CPU_65816::LSR()
{
	const uint16_t mask = accumulatorMask();
	const uint16_t value = readOperand();
	setflag(Flag::C, (value & 0x0001) != 0);
	const uint16_t result = static_cast<uint16_t>(value >> 1) & mask;
	writeOperand(result);
	setflag(Flag::N, false);
	setflag(Flag::Z, result == 0);
	return 0;
}

uint8_t CPU_65816::ROL()
{
	const uint16_t mask = accumulatorMask();
	const uint16_t value = readOperand();
	const bool oldCarry = getflag(Flag::C);
	setflag(Flag::C, (value & accumulatorSignBit()) != 0);
	const uint16_t result = static_cast<uint16_t>((value << 1) |
		(oldCarry ? 1 : 0)) & mask;
	writeOperand(result);
	updateNZ(result, mask, accumulatorSignBit());
	return 0;
}

uint8_t CPU_65816::ROR()
{
	const uint16_t mask = accumulatorMask();
	const uint16_t value = readOperand();
	const bool oldCarry = getflag(Flag::C);
	setflag(Flag::C, (value & 0x0001) != 0);
	const uint16_t result = static_cast<uint16_t>((value >> 1) | (oldCarry ? accumulatorSignBit() : 0)) & mask;
	writeOperand(result);
	updateNZ(result, mask, accumulatorSignBit());
	return 0;
}

uint8_t CPU_65816::BIT()
{
	const uint16_t value = readOperand();
	const uint16_t accumulator = readaccumulator();
	setflag(Flag::Z, (value & accumulator) == 0);

	if (operandKind != OperandKind::Immediate)
	{
		setflag(Flag::N, (value & accumulatorSignBit()) != 0);
		setflag(Flag::V, (value & (accumulatorSignBit() >> 1)) != 0);
	}
	return 0;
}

uint8_t CPU_65816::CMP()
{
	compareValues(readaccumulator(), readOperand(), accumulatorMask(), accumulatorSignBit());
	return 0;
}

uint8_t CPU_65816::CPX()
{
	compareValues(readX(), readOperandWidth(true), indexMask(), indexSignBit());
	return 0;
}

uint8_t CPU_65816::CPY()
{
	compareValues(readY(), readOperandWidth(true), indexMask(), indexSignBit());
	return 0;
}

uint8_t CPU_65816::INC()
{
	const uint16_t mask = accumulatorMask();
	const uint16_t value = static_cast<uint16_t>(readOperand() + 1) & mask;
	writeOperand(value);
	updateNZ(value, mask, accumulatorSignBit());
	return 0;
}

uint8_t CPU_65816::DEC()
{
	const uint16_t mask = accumulatorMask();
	const uint16_t value = static_cast<uint16_t>(readOperand() - 1) & mask;
	writeOperand(value);
	updateNZ(value, mask, accumulatorSignBit());
	return 0;
}

uint8_t CPU_65816::INA()
{
	return INC();
}

uint8_t CPU_65816::DEA()
{
	return DEC();
}

uint8_t CPU_65816::INX()
{
	writeX(static_cast<uint16_t>(readX() + 1));
	updateNZ(readX(), indexMask(), indexSignBit());
	return 0;
}

uint8_t CPU_65816::DEX()
{
	writeX(static_cast<uint16_t>(readX() - 1));
	updateNZ(readX(), indexMask(), indexSignBit());
	return 0;
}

uint8_t CPU_65816::INY()
{
	writeY(static_cast<uint16_t>(readY() + 1));
	updateNZ(readY(), indexMask(), indexSignBit());
	return 0;
}

uint8_t CPU_65816::DEY()
{
	writeY(static_cast<uint16_t>(readY() - 1));
	updateNZ(readY(), indexMask(), indexSignBit());
	return 0;
}

// Loads and stores 

uint8_t CPU_65816::LDA()
{
	writeaccumulator(readOperand());
	updateNZ(readaccumulator(), accumulatorMask(), accumulatorSignBit());
	return 0;
}

uint8_t CPU_65816::LDX()
{
	writeX(readOperandWidth(true));
	updateNZ(readX(), indexMask(), indexSignBit());
	return 0;
}

uint8_t CPU_65816::LDY()
{
	writeY(readOperandWidth(true));
	updateNZ(readY(), indexMask(), indexSignBit());
	return 0;
}

uint8_t CPU_65816::STA()
{
	writeOperand(readaccumulator());
	return 0;
}

uint8_t CPU_65816::STX()
{
	if (operandKind == OperandKind::Memory)
	{
		const uint16_t value = readX();
		write(addr_abs, static_cast<uint8_t>(value));
		if (!indexis8bit())
			write((addr_abs & 0xFF0000) | static_cast<uint16_t>((addr_abs + 1) & 0xFFFF), static_cast<uint8_t>(value >> 8));
	}
	return 0;
}

uint8_t CPU_65816::STY()
{
	if (operandKind == OperandKind::Memory)
	{
		const uint16_t value = readY();
		write(addr_abs, static_cast<uint8_t>(value));
		if (!indexis8bit())
			write((addr_abs & 0xFF0000) | static_cast<uint16_t>((addr_abs + 1) & 0xFFFF), static_cast<uint8_t>(value >> 8));
	}
	return 0;
}

uint8_t CPU_65816::STZ()
{
	writeOperand(0);
	return 0;
}

// Transfers

uint8_t CPU_65816::TAX()
{
	writeX(readaccumulator());
	updateNZ(readX(), indexMask(), indexSignBit());
	return 0;
}

uint8_t CPU_65816::TAY()
{
	writeY(readaccumulator());
	updateNZ(readY(), indexMask(), indexSignBit());
	return 0;
}

uint8_t CPU_65816::TXA()
{
	writeaccumulator(readX());
	updateNZ(readaccumulator(), accumulatorMask(), accumulatorSignBit());
	return 0;
}

uint8_t CPU_65816::TYA()
{
	writeaccumulator(readY());
	updateNZ(readaccumulator(), accumulatorMask(), accumulatorSignBit());
	return 0;
}

uint8_t CPU_65816::TXY()
{
	writeY(readX());
	updateNZ(readY(), indexMask(), indexSignBit());
	return 0;
}

uint8_t CPU_65816::TYX()
{
	writeX(readY());
	updateNZ(readX(), indexMask(), indexSignBit());
	return 0;
}

uint8_t CPU_65816::TSX()
{
	writeX(STKP);
	updateNZ(readX(), indexMask(), indexSignBit());
	return 0;
}

uint8_t CPU_65816::TXS()
{
	STKP = indexis8bit() ? static_cast<uint16_t>(0x0100 | (X & 0x00FF)) : X;
	forceEmulationWidths();
	return 0;
}

uint8_t CPU_65816::TCS()
{
	STKP = A;
	forceEmulationWidths();
	return 0;
}

uint8_t CPU_65816::TCD()
{
	D = A;
	updateNZ(D, 0xFFFF, 0x8000);
	return 0;
}

uint8_t CPU_65816::TDC()
{
	A = D;
	updateNZ(A, 0xFFFF, 0x8000);
	return 0;
}

uint8_t CPU_65816::TSC()
{
	A = STKP;
	updateNZ(A, 0xFFFF, 0x8000);
	return 0;
}

uint8_t CPU_65816::XBA()
{
	const uint8_t low = static_cast<uint8_t>(A & 0x00FF);
	const uint8_t high = static_cast<uint8_t>(A >> 8);
	A = static_cast<uint16_t>((low << 8) | high);
	updateNZ(static_cast<uint16_t>(A & 0x00FF), 0x00FF, 0x0080);
	return 0;
}

void CPU_65816::branchIf(bool condition)
{
	if (condition)
		PC = static_cast<uint16_t>(PC + addr_rel);
}

uint8_t CPU_65816::BCC()
{
	branchIf(!getflag(Flag::C));
	return 0;
}

uint8_t CPU_65816::BCS()
{
	branchIf(getflag(Flag::C));
	return 0;
}

uint8_t CPU_65816::BNE()
{
	branchIf(!getflag(Flag::Z));
	return 0;
}

uint8_t CPU_65816::BEQ()
{
	branchIf(getflag(Flag::Z));
	return 0;
}

uint8_t CPU_65816::BPL()
{
	branchIf(!getflag(Flag::N));
	return 0;
}

uint8_t CPU_65816::BMI()
{
	branchIf(getflag(Flag::N));
	return 0;
}

uint8_t CPU_65816::BVC()
{
	branchIf(!getflag(Flag::V));
	return 0;
}

uint8_t CPU_65816::BVS()
{
	branchIf(getflag(Flag::V));
	return 0;
}

uint8_t CPU_65816::BRA()
{
	branchIf(true);
	return 0;
}

uint8_t CPU_65816::BRL()
{
	branchIf(true);
	return 0;
}

uint8_t CPU_65816::JMP()
{
	PC = fetched;
	return 0;
}

uint8_t CPU_65816::JML()
{
	PBR = static_cast<uint8_t>(addr_abs >> 16);
	PC = static_cast<uint16_t>(addr_abs);
	return 0;
}

uint8_t CPU_65816::JSR()
{
	push16(static_cast<uint16_t>(PC - 1));
	PC = fetched;
	return 0;
}

uint8_t CPU_65816::JSL()
{
	push8(PBR);
	push16(static_cast<uint16_t>(PC - 1));
	PBR = static_cast<uint8_t>(addr_abs >> 16);
	PC = static_cast<uint16_t>(addr_abs);
	return 0;
}

uint8_t CPU_65816::RTS()
{
	PC = static_cast<uint16_t>(pull16() + 1);
	return 0;
}

uint8_t CPU_65816::RTL()
{
	PC = static_cast<uint16_t>(pull16() + 1);
	PBR = pull8();
	return 0;
}

// Interrupts

bool CPU_65816::irqPending() const
{
	return irqLine;
}

void CPU_65816::serviceInterrupt(uint16_t vector, bool nativeFrame, bool softwareBreak)
{
	if (nativeFrame)
		push8(PBR);

	push16(PC);
	uint8_t pushedStatus = status;
	if (softwareBreak)
		pushedStatus |= 0x10;
	else if (E)
		pushedStatus &= static_cast<uint8_t>(~0x10);
	push8(pushedStatus);

	setflag(Flag::I, true);
	setflag(Flag::D, false);
	PBR = 0x00;
	PC = read16(vector);
	waiting = false;
	cycles = nativeFrame ? 8 : 7;
}

void CPU_65816::servicePendingInterrupts()
{
	if (nmiPendingFlag)
	{
		nmiPendingFlag = false;
		serviceInterrupt(E ? 0xFFFA : 0xFFEA, !E);
		return;
	}

	if (irqPending() && !getflag(Flag::I))
		serviceInterrupt(E ? 0xFFFE : 0xFFEE, !E);
}

void CPU_65816::irq()
{
	irqLine = true;
}

void CPU_65816::nmi()
{
	nmiPendingFlag = true;
}

uint8_t CPU_65816::BRK()
{
	fetch8();
	serviceInterrupt(E ? 0xFFFE : 0xFFE6, !E, true);
	return 0;
}

uint8_t CPU_65816::COP()
{
	fetch8();
	serviceInterrupt(E ? 0xFFF4 : 0xFFE4, !E, true);
	return 0;
}

uint8_t CPU_65816::RTI()
{
	status = pull8();
	PC = pull16();
	if (!E)
		PBR = pull8();
	forceEmulationWidths();
	return 0;
}

uint8_t CPU_65816::STP()
{
	stopped = true;
	return 0;
}

uint8_t CPU_65816::WAI()
{
	waiting = true;
	return 0;
}

uint8_t CPU_65816::CLC()
{
	setflag(Flag::C, false);
	return 0;
}

uint8_t CPU_65816::CLD()
{
	setflag(Flag::D, false);
	return 0;
}

uint8_t CPU_65816::CLI()
{
	setflag(Flag::I, false);
	return 0;
}

uint8_t CPU_65816::CLV()
{
	setflag(Flag::V, false);
	return 0;
}

uint8_t CPU_65816::SEC()
{
	setflag(Flag::C, true);
	return 0;
}

uint8_t CPU_65816::SED()
{
	setflag(Flag::D, true);
	return 0;
}

uint8_t CPU_65816::SEI()
{
	setflag(Flag::I, true);
	return 0;
}

uint8_t CPU_65816::REP()
{
	status &= static_cast<uint8_t>(~static_cast<uint8_t>(fetched));
	forceEmulationWidths();
	return 0;
}

uint8_t CPU_65816::SEP()
{
	status |= static_cast<uint8_t>(fetched);
	forceEmulationWidths();
	return 0;
}

uint8_t CPU_65816::XCE()
{
	const bool oldCarry = getflag(Flag::C);
	const bool oldEmulation = E;
	setflag(Flag::C, oldEmulation);
	E = oldCarry;
	forceEmulationWidths();
	return 0;
}

// Stack and data movement 

uint8_t CPU_65816::PHA()
{
	if (accumulatoris8bit())
		push8(static_cast<uint8_t>(A));
	else
		push16(A);
	return 0;
}

uint8_t CPU_65816::PHX()
{
	if (indexis8bit())
		push8(static_cast<uint8_t>(X));
	else
		push16(X);
	return 0;
}

uint8_t CPU_65816::PHY()
{
	if (indexis8bit())
		push8(static_cast<uint8_t>(Y));
	else
		push16(Y);
	return 0;
}

uint8_t CPU_65816::PHD()
{
	push16(D);
	return 0;
}

uint8_t CPU_65816::PHB()
{
	push8(DBR);
	return 0;
}

uint8_t CPU_65816::PHK()
{
	push8(PBR);
	return 0;
}

uint8_t CPU_65816::PHP()
{
	uint8_t value = status;
	if (E)
		value |= 0x10;
	push8(value);
	return 0;
}

uint8_t CPU_65816::PEA()
{
	push16(fetched);
	return 0;
}

uint8_t CPU_65816::PEI()
{
	push16(fetched);
	return 0;
}

uint8_t CPU_65816::PER()
{
	push16(static_cast<uint16_t>(PC + addr_rel));
	return 0;
}

uint8_t CPU_65816::PLA()
{
	writeaccumulator(accumulatoris8bit() ? pull8() : pull16());
	updateNZ(readaccumulator(), accumulatorMask(), accumulatorSignBit());
	return 0;
}

uint8_t CPU_65816::PLX()
{
	writeX(indexis8bit() ? pull8() : pull16());
	updateNZ(readX(), indexMask(), indexSignBit());
	return 0;
}

uint8_t CPU_65816::PLY()
{
	writeY(indexis8bit() ? pull8() : pull16());
	updateNZ(readY(), indexMask(), indexSignBit());
	return 0;
}

uint8_t CPU_65816::PLP()
{
	status = pull8();
	forceEmulationWidths();
	return 0;
}

uint8_t CPU_65816::PLD()
{
	D = pull16();
	updateNZ(D, 0xFFFF, 0x8000);
	return 0;
}

uint8_t CPU_65816::PLB()
{
	DBR = pull8();
	updateNZ(DBR, 0x00FF, 0x0080);
	return 0;
}

uint8_t CPU_65816::MVN()
{
	DBR = blockDestinationBank;
	write(make24bitaddress(blockDestinationBank, Y), read(make24bitaddress(blockSourceBank, X)));
	X = static_cast<uint16_t>(X + 1);
	Y = static_cast<uint16_t>(Y + 1);
	A = static_cast<uint16_t>(A - 1);
	if (A != 0xFFFF)
		PC = static_cast<uint16_t>(PC - 3);
	return 0;
}

uint8_t CPU_65816::MVP()
{
	DBR = blockDestinationBank;
	write(make24bitaddress(blockDestinationBank, Y), read(make24bitaddress(blockSourceBank, X)));
	X = static_cast<uint16_t>(X - 1);
	Y = static_cast<uint16_t>(Y - 1);
	A = static_cast<uint16_t>(A - 1);
	if (A != 0xFFFF)
		PC = static_cast<uint16_t>(PC - 3);
	return 0;
}

uint8_t CPU_65816::NOP()
{
	return 0;
}

uint8_t CPU_65816::WDM()
{
	return 0;
}

uint8_t CPU_65816::XXX()
{
	return 0;
}

uint8_t CPU_65816::fetch()
{
	return fetch8();
}
