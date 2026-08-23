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

	//instructions
	uint8_t ADC(), AND(), ADDW();
	uint8_t AND1(), ASL(), BBC();
	uint8_t BBS(), BBS(), BCC();
	uint8_t BCS(), BEQ(), BMI();
	uint8_t BNE(), BPL(), BVC();
	uint8_t BVS(), BRA(), BRK();
	uint8_t CALL(), CBNE(), CLR1();
	uint8_t CLRC(), CLRP(), CLRV();
	uint8_t CMP(), CMPW(), DAA();
	uint8_t DAS(), DBNZ(), DEC();
	uint8_t DECW(), DI(), DIV();
	uint8_t EI(), EOR(), EOR1();
	uint8_t INC(), INCW(), JMP();
	uint8_t LSR(), MOV(), MOV1();
	uint8_t MOVW(), MUL(), NOP();
	uint8_t NOT1(), NOTC(), OR();
	uint8_t OR1(), PCALL(), POP();
	uint8_t PUSH(), RET(), RETI();
	uint8_t ROL(), ROR(), SBC();
	uint8_t SET1(), SETC(), SETP();
	uint8_t SLEEP(), STOP(), SUBW();
	uint8_t TCALL(), TCLR1(), TSET();
	uint8_t XCN(), PCALL(), TCALL();

	uint8_t XXX();
	void clock();
	void reset();
	uint8_t fetch();
	uint16_t fetched = 0x0000;
	uint16_t addr_abs = 0x0000;
	uint8_t addr_rel = 0x00;
	uint8_t cycles = 0;

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
};
