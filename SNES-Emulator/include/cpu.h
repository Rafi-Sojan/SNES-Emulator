#pragma once

#include <cstdint>
#include <array>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>

class CPU_65816{
public:

	uint16_t A = 0x0000; // accumulator
	uint16_t X = 0x0000; // x register
	uint16_t Y = 0x0000; // y register
	uint16_t STKP = 0x0000; // stack pointer
	uint8_t DBR = 0x00; //Data bank register
	uint16_t D = 0x0000; /// Direct page register
	uint16_t PC = 0x0000; // program counter 
	uint8_t status = 0x00; // status register

	struct flags {
		uint8_t N : 1; // negative
		uint8_t V : 1; // overflow
		uint8_t M : 1; // accumulator size (0 if 16bit,  1 if 8bit)
		uint8_t X : 1; // index size (0 if 16bit,  1 if 8bit)
		uint8_t D : 1; // decimal
		uint8_t I : 1; // irq disable
 		uint8_t Z : 1; // zero
		uint8_t C : 1; // carry
		uint8_t E : 1; // emulation mode
		uint8_t B : 1; // break during emulation
 	};

	//addressing modes
	uint8_t IMP(), IMPMF(), IMPIF(), IMP8B(); // implied, immediate[memoryflag], immediate[indexflag], immediate[8-bit]
	uint8_t RL(), RLL(); // relative, relative long
	uint8_t DR(), DRX(), DRY(), DRI(), DRID(), DRII(), DRIL(), DRIIL(); // direct, directindexedX, directIndexedY, directindirect, directindexedindirect, directindirectindexed, directindirectlong, directindirectindexedlong
	uint8_t AB(), ABX(), ABY(), ABL(), ABIL(); // absolute, absX, absY, abslong, absindexedlong
	uint8_t SR(), SRII(); // stackrelative, stackrelativeindirectindexed
	uint8_t ABD(), ABDI(), ABDIL(), ABDII(); // absindirect, absindirectlong, absindexedindirect
	uint8_t IMA(), BM(); // impaccumulator, blockmove

	//instructions 
	uint8_t ADC(), SBC(), AND(), EOR(); // add A with carry bit and put in A, subtract with carry bit, AND A with memory and put in A, Exclusive or 
	uint8_t ORA(), TSB(), TRB(), ASL(); // OR A with memory and put in A, Test and set bit, Test and Reset bit, Arithmetic shift left
	uint8_t LSR(), ROL(), ROR(), BIT(); // Logical shift right, Rotate left, Rotate right, test bits
	uint8_t CMP(), CPX(), CPY(), DEA(); // Compare accumulator with memory, Compare register X with memory, Compare register Y with memory, Decrement accumulator
	uint8_t DEC(), DEX(), DEY(), INA(); // Decrement check INC, Decrement X register, Decrement Y register, Increment Accumulator
	uint8_t INC(), INX(), INY(), NOP(); // Increment check DEC, increment X register, Increment Y register, No operation  
	uint8_t XBA(), LDA(), LDX(), LDY(); // Exchange bytes of accumulator, Load accumulator from memory, Load register X from memory, Load register Y from memory
	uint8_t STA(), STX(), STY(), STZ(); // Store accumulator in memory, Store register X in memory, Store register Y in memory, Store zero in memory 
	uint8_t TAX(), TAY(), TCD(), TCS(); // Transfer accumulator to index register X, Transfer accumulator to index register Y, Transfer 16bit accumulator to direct page, Transfer 16 bit accumulator to stack pointer
	uint8_t TSX(), TXA(), TXS(), TXY(); // Transfer stack pointer to index register X, Transfer index register X to accumulator, Transfer index X register to stack pointer, Transfer Index register X to Index register Y 
	uint8_t TYA(), TYX(), BCC(), BCS(); // Transfer index register Y to accumulator, Transfer index register Y to index register X, Branch if carry flag is clear, Branch if carry flag is set 
	uint8_t BNE(), BEQ(), BPL(), BMI(); // Branch if not equal, Branch if equal, Branch if plus, Branch if minus, 
	uint8_t BVC(), BVS(), BRA(), BRL(); // Branch if overflow flag is clear, Branch if overflow flag is set, Branch always, Branch always long
	uint8_t JMP(), JML(), JSR(), JSL(); // Jump, Jump long, Jump and save return address, Jump long and save return address
	uint8_t RTS(), RTL(), BRK(), COP(); // Return from subroutine, Return long from subroutine, Generate software interrupt, Generate coprocessor interrupt
	uint8_t RTI(), STP(), WAI(), CLC(); // Return from interrupt, Stop processor until RESET, Wait for hardware Interrupt, Clear carry flag
	uint8_t CLD(), CLI(), CLV(), REP(); // Clear decimal flag, Enable interrupt requests, Clear overflow flag, Reset status bits
	uint8_t SEC(), SED(), SEI(), XCE(), SEP(); // Set carry flag, Set decimal flag, Set, Disable interrupt requests, Exchange carry flag with emulation flag, Set status bits
	uint8_t PHA(), PHX(), PHY(), PHD(); // Push Accumulator, Push index register X, Push index register Y, Push direct page register
	uint8_t PHB(), PHK(), PHP(), PEA(); // Push data bank register, Push program bank register, Push processor status, Push effective address
	uint8_t PEI(), PER(), PLA(), PLX(); // Push effective indirect address, Push effective relative address, Pull Accumulator, Pull index register X
	uint8_t PLY(), PLP(), PLD(), PLB(); // Pull index register Y, Pull porcessor status, Pull direct page register, Pull data bank register
	uint8_t TSC();

	uint8_t XXX();

	void clock();
	void reset();
	void irq();
	void nmi();

	uint8_t fetch();
	uint8_t fetched = 0x00;


private:
	struct instruction 
	{
		std::string name;
		uint8_t(CPU_65816::* operate)(void) = nullptr;
		uint8_t(CPU_65816::* addrmode)(void) = nullptr;
		uint8_t cycles = 0;
	};

	std::vector<instruction> lookup;
};