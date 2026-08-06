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
	uint16_t STKP = 0x00; // stack pointer
	uint8_t DBR = 0x00; //Data bank register
	uint16_t D = 0x00; /// Direct page register
	uint16_t PC = 0x00; // program counter 
	uint8_t status = 0x00; // status register

	struct flags {
		uint8_t N : 1; //negative
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


private:

};