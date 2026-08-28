#pragma once

#include <cstdint>
#include <cassert>
#include <array>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include "memory.h"

const int WRAM_SIZE = 128 * 1024; // work ram size
const int VRAM_SIZE = 64 * 1024; // video ram size
const int ARAM_SIZE = 64 * 1024; // audio ram size

struct Bus
{
public:
	Bus();

	uint8_t read(uint32_t address); // reading a 24-bit SNES address
	void write(uint32_t address, uint8_t data); // writing a 24-bit SNES address
	uint8_t wram_alloc[WRAM_SIZE]; // simulating the work ram size
	uint8_t vram_alloc[VRAM_SIZE]; // simulating the video ram size
	uint8_t aram_alloc[ARAM_SIZE]; // simulating the audio ram size

private:
	Memory wram;
};
