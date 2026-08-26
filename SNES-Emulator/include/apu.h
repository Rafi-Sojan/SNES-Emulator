#pragma once

#include <cstdint>
#include <array>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>

//uses 8 bit Accumulator, X, Y, Stack Pointer, Program status unlike the 65c816 having the modes to switch between 8 bit and 16 bit i worked on last week

class Spc_700 {
public:
	uint8_t A = 0x00; // 8 bit Accumulator
	uint8_t X = 0x00; // 8 bit X register
	uint8_t Y = 0x00; // 8 bit Y Register
	uint8_t SP = 0x00; // 8 bit Stack Pointer
	uint8_t PC = 0x0000; // 16 bit Program Counter
	uint8_t PSW = 0x00; // 8 bit Program status

	enum class Flag : uint8_t {
		N = 0x80, // Negative
		V = 0x40, // Overflow
		P = 0x20, // Direct Page
		S = 0x10, // Break
		H = 0x08, // Half Carry
		I = 0x04, // Interrupt enabled 
		Z = 0x02, // Zero
		C = 0x01  // Carry
	};

	//addressing modes, i reused the addressing modes i declared in 65c816 saved my time a lot

	uint8_t IMP(), IMPMF(), IMPIF(), IMP8B(), IMM(); // implied, immediate[memoryflag], immediate[indexflag], immediate[8-bit]
	uint8_t RL(), RLL(); // relative, relative long
	uint8_t DR(), DRX(), DRY(), DRI(), DRID(), DRII(), DRIL(), DRIIL(); // direct, directindexedX, directIndexedY, directindirect, directindexedindirect, directindirectindexed, directindirectlong, directindirectindexedlong
	uint8_t AB(), ABX(), ABY(), ABL(), ABIL(); // absolute, absX, absY, abslong, absindexedlong
	uint8_t SR(), SRII(), ST(); // stackrelative, stackrelativeindirectindexed, Stack
	uint8_t ABD(), ABDI(), ABDIL(), ABDII(); // absindirect, absindirectlong, absindexedindirect
	uint8_t AC(), IMA(), BM(); // accumulator, impaccumulator, blockmove

	//instructions
	uint8_t ADC(), AND(), ADDW(); // Add with a Carry, Perform AND Operation, ADD to 16 bit 
	uint8_t AND1(), ASL(), BBC(); // AND with a single bit carry, Arithmetic shift left
	uint8_t BBS(), BSS(), BCC(); // Branch if bit is clear, Branch if bit is set, Branch if carry clear
	uint8_t BCS(), BEQ(), BMI(); // Branch if carry set, Branch if equal, Branch if Negative
	uint8_t BNE(), BPL(), BVC(); // Branch if not equal, Branch if Positive, Branch if overflow clear
	uint8_t BVS(), BRA(), BRK(); // Branch if overflow set, Branch always, interrupt
	uint8_t CALL(), CBNE(), CLR1(); // Call routine, Compare and branch if not equal, Clear memory bit
	uint8_t CLRC(), CLRP(), CLRV(); // Clear carry, Clear direct page flag, Clear overflow
	uint8_t CMP(), CMPW(), DAA(); // Compare, Compare with 16 bit, Decimal adjust after addition
	uint8_t DAS(), DBNZ(), DEC(); // Decimal adjust after subtraction, Decrement and bracnch if nonzero
	uint8_t DECW(), DI(), DIV(); // Decrememnt 16 bit, Disable Interrupts, performs division
	uint8_t EI(), EOR(), EOR1(); // enable interrupts, perform XOR operation, perform XOR with memory bit
	uint8_t INC(), INCW(), JMP(); // Increment, Increment for 16 bit, Jump 
	uint8_t LSR(), MOV(), MOV1(); // Shift right, Move byte, Move single bit
	uint8_t MOVW(), MUL(), NOP(); // Move 16 bit, multiply, No operation
	uint8_t NOT1(), NOTC(), OR(); // invert memory bit, invert carry, perform OR operation 
	uint8_t OR1(), PCALL(), POP(); // perform OR with memory bit, page call, pop stack
	uint8_t PUSH(), RET(), RETI(); // push stack, return from routine, return from interrupt
	uint8_t ROL(), ROR(), SBC(); // Rotate left through carry, Rotate right through carry, Subtract with carry
	uint8_t SET1(), SETC(), SETP(); // set memory bit, set carry, set page flag
	uint8_t SLEEP(), STOP(), SUBW(); // enter sleep state, stop cpu, subtract from 16 bit
	uint8_t TCALL(), TCLR1(), TSET(); // table based routine, test and clear bit, test and set 
	uint8_t XCN(); // exchange 

	uint8_t XXX(); // lookup 
	void clock(); // lookup the operand and perform the operation per cycles
	void reset(); // resets the addresses
	uint8_t fetch(); // fetches the address
	uint16_t fetched = 0x0000; // fetched external address 
	uint16_t addr_abs = 0x0000; // absolute addressing 
	uint8_t addr_rel = 0x00; // relative addressing
	uint8_t cycles = 0; // number of cycles

private:
	uint8_t read(uint16_t address); // reads the address
	void write(uint16_t address, uint8_t value); // writes the address
	uint8_t getflag(Flag flag) const; // gets the flag 
	void setflag(Flag flag, bool value); // sets the flag status true or false 
	uint8_t readaccumulator() const; // reads the address from accumulator
	void writeaccumulator(uint8_t value); // writes the accumulator address
	uint8_t readX() const; // reads the X register
	uint8_t readY() const; // reads the Y register
	void writeX(uint8_t value); // writes the X register
	void writeY(uint8_t value); // writes the Y register
	uint8_t fetch8(); // fetches the 8 bit address
	uint8_t fetch16(); // fetches the 16 bit address
	void push8(uint8_t value); // pushes the 8 bit address into the stack
	void push16(uint16_t value); // pushes the 16 bit address into the stack
	uint8_t pull8(); // pulls the 8 bit address if it's on the top of the stack
	uint16_t pull16(); // pulls the 16 bit address if it's on the top of the stack

	struct instruction // reads from the opcode map 
	{
		std::string name;
		uint8_t(Spc_700::* operate)(void) = nullptr;
		uint8_t(Spc_700::* addrmode)(void) = nullptr;
		uint8_t cycles = 0;
	};

	std::vector<instruction> lookup;
};
