#include "../include/ppu.h"

void ppu::incrementVRAMAddress() {
	uint16_t increment = 1;

	switch (vmain & 0x03)
	{

	case 0:
		increment = 1;
		break;

	case 1:
		increment = 32;
		break;

	case 2:

	case 3:
		increment = 128;
		break;
	}

	vramAddress = (vramAddress + increment) & 0x7FFF;
}

uint16_t ppu::vramByteAddress() const {
	return static_cast<uint16_t> ((vramAddress & 0x7FFF) << 1);
}

void ppu::VMAIN(uint8_t value) {
	vmain = value;
	registers[0x15] = value;
}

void ppu::VMADDL(uint8_t value) {
	registers[0x16] = value;

	vramAddress = (vramAddress & 0x7F00) | value;
}

void ppu::VMADDH(uint8_t value) {
	registers[0x17] = value;

	vramAddress = (vramAddress & 0x00FF) | ((value & 0x7F) << 8);
}

void ppu::VMDATAL(uint8_t value) {
	registers[0x18] = value;

	uint16_t address = vramByteAddress();
	vram[address] = value;

	if ((vmain & 0x80) == 0)
		incrementVRAMAddress();
}

void ppu::VMDATAH(uint8_t value) {
	registers[0x19] = value;

	uint16_t address = static_cast<uint16_t>(vramByteAddress() + 1);

	vram[address] = value;

	if ((vmain & 0x80) != 0)
		incrementVRAMAddress();
}
