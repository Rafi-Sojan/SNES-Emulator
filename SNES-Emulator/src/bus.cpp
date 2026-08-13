#include "../include/bus.h"
#include "../include/cpu.h"

Bus::Bus()
{
}

uint8_t Bus::read(uint32_t address) {
	address &= 0xFFFFFF;

	// Temporary mapping: only the first WRAM region is connected.
	// Cartridge, PPU, APU, and WRAM mirroring will be added later.
	if (address < WRAM_SIZE)
		return wram.read(address);

	return 0xFF;
}

void Bus::write(uint32_t address, uint8_t data) {
	address &= 0xFFFFFF;

	if (address < WRAM_SIZE)
		wram.write(address, data);
}
