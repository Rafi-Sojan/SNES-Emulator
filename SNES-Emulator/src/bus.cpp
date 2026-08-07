#include "../include/bus.h"
#include "../include/cpu.h"

Bus::Bus()
{
}

uint8_t Bus::read(uint16_t address) {
	if (address <= 0xFFFF)
		return wram.read(address);
	return 0xFF;
}

void Bus::write(uint16_t address, uint8_t data) {
	if (address <= 0xFFFF)
		wram.write(address, data);
}