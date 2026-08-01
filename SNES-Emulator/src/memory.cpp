#include "../include/memory.h"

Memory::Memory() {
	ram.fill(0);
}

uint8_t Memory::read(uint32_t address) {
	return ram[address];
}

void Memory::write(uint32_t address, uint8_t value) {
	ram[address] = value;
}

