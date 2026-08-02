#pragma once

#include <cstdint>
#include <array>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>

struct CPU{
public:
	uint8_t A = 0x00; // accumulator
	uint8_t X = 0x00; // x register
	uint8_t Y = 0x00; // y register
	uint8_t STKP = 0x00; // stack pointer
	uint8_t PC = 0x00; // program counter 
	uint8_t status = 0x00; // status register

	struct flags {
		uint8_t N : 1; //negative
		uint8_t V : 1; // overflow
		uint8_t M : 1; // accumulator size
		uint8_t X : 1; // index size
		uint8_t D : 1; // decimal 
 		uint8_t Z : 1; // IRQ
		uint8_t C : 1; // carry
		uint8_t B : 1; // break during emulation
 	};

private:

};