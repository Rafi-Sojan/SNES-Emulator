#include "../include/memory.h"

Memory::Memory() {
	ram.fill(0);
}

uint8_t Memory::read(uint32_t address) const {
	if (address >= ram.size())
		return 0xFF;

	return ram[address];
}


void Memory::write(uint32_t address, uint8_t value) {
	if (address >= ram.size())
		return;

	ram[address] = value;
}

