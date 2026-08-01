#include "../include/bus.h"

Bus::Bus()
{
}

uint8_t Bus::read(uint32_t address) {
	if (address <= 0x1FFFF)
		return wram.read(address);
	return 0xFF;
}

void Bus::write(uint32_t address, uint8_t data) {
	if (address <= 0x1FFFF)
		wram.write(address, data);
}