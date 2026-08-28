#include "../include/cpu.h"
#include "../include/bus.h"

static constexpr  uint32_t RESET_VECTOR = 0x00FFFC;
static constexpr  uint16_t PROGRAM_START = 0x8000;

void setResetVector(Bus& bus, uint16_t address) {
	bus.write(RESET_VECTOR, static_cast<uint8_t>(address & 0x00FF));
	bus.write(RESET_VECTOR + 1, static_cast<uint8_t>((address >> 8) & 0x00FF));
}


// executes one cpu instruction and continues till the number of cycles drop to 0
void executeOneInstruction(CPU_65816& cpu) {
	cpu.clock();

	while (cpu.cycles != 0)
		cpu.clock();
}


// static casts the flag and sets the status
bool flagSet(const CPU_65816 cpu, CPU_65816::Flag flag) {
	const uint8_t mask = static_cast<uint8_t>(flag);

	return (cpu.status & mask) != 0;
}

// tests whether the buses are functioning properly and the cpu properly resets to default address 
void testReset()
{
    Bus bus;
    CPU_65816 cpu(bus);

    setResetVector(bus, PROGRAM_START);
    cpu.reset();

    assert(cpu.PC == PROGRAM_START);
    assert(cpu.A == 0);
    assert(cpu.X == 0);
    assert(cpu.Y == 0);
    assert(cpu.E == true);
    assert(flagSet(cpu, CPU_65816::Flag::M));
    assert(flagSet(cpu, CPU_65816::Flag::X));
    assert(flagSet(cpu, CPU_65816::Flag::I));
}


void testLDAImmediate()
{
    Bus bus;
    CPU_65816 cpu(bus);

    bus.write(PROGRAM_START + 0, 0xA9); // LDA #$42
    bus.write(PROGRAM_START + 1, 0x42);

    setResetVector(bus, PROGRAM_START);
    cpu.reset();

    executeOneInstruction(cpu);

    assert(cpu.A == 0x42);
    assert(cpu.PC == PROGRAM_START + 2);
    assert(!flagSet(cpu, CPU_65816::Flag::Z));
    assert(!flagSet(cpu, CPU_65816::Flag::N));
}

void testLDXZero()
{
    Bus bus;
    CPU_65816 cpu(bus);

    bus.write(PROGRAM_START + 0, 0xA2); // LDX #$00
    bus.write(PROGRAM_START + 1, 0x00);

    setResetVector(bus, PROGRAM_START);
    cpu.reset();

    executeOneInstruction(cpu);

    assert(cpu.X == 0x00);
    assert(flagSet(cpu, CPU_65816::Flag::Z));
    assert(!flagSet(cpu, CPU_65816::Flag::N));
}

void testADCImmediate()
{
    Bus bus;
    CPU_65816 cpu(bus);

    bus.write(PROGRAM_START + 0, 0xA9); // LDA #$10
    bus.write(PROGRAM_START + 1, 0x10);

    bus.write(PROGRAM_START + 2, 0x69); // ADC #$05
    bus.write(PROGRAM_START + 3, 0x05);

    setResetVector(bus, PROGRAM_START);
    cpu.reset();

    executeOneInstruction(cpu);
    executeOneInstruction(cpu);

    assert(cpu.A == 0x15);
    assert(!flagSet(cpu, CPU_65816::Flag::Z));
    assert(!flagSet(cpu, CPU_65816::Flag::C));
}

void testSTAZeroPage()
{
    Bus bus;
    CPU_65816 cpu(bus);

    bus.write(PROGRAM_START + 0, 0xA9); // LDA #$5A
    bus.write(PROGRAM_START + 1, 0x5A);

    bus.write(PROGRAM_START + 2, 0x85); // STA $20
    bus.write(PROGRAM_START + 3, 0x20);

    setResetVector(bus, PROGRAM_START);
    cpu.reset();

    executeOneInstruction(cpu);
    executeOneInstruction(cpu);

    assert(bus.read(0x0020) == 0x5A);
}

void testBNE()
{
    Bus bus;
    CPU_65816 cpu(bus);

    bus.write(PROGRAM_START + 0, 0xD0); // BNE +2
    bus.write(PROGRAM_START + 1, 0x02);

    setResetVector(bus, PROGRAM_START);
    cpu.reset();

    
    cpu.status &= static_cast<uint8_t>(~static_cast<uint8_t>(CPU_65816::Flag::Z));

    executeOneInstruction(cpu);

    assert(cpu.PC == PROGRAM_START + 4);
}


