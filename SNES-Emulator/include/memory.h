#pragma once

#include <cstdint>
#include <array>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>

class Memory {
public:
	Memory();

	uint8_t read(uint32_t address);
	void write(uint32_t address, uint8_t value);

private:
	std::array<uint8_t, 128 * 1024> ram;
};