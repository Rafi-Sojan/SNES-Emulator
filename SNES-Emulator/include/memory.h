#pragma once

#include <cstdint>
#include <array>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>

struct Memory {
public:
	Memory();

	uint8_t read(uint32_t address) const;
	void write(uint32_t address, uint8_t value);

private:
	std::array<uint8_t, 256 * 1024> ram; // simulating onboard ram size 256Kb
};
