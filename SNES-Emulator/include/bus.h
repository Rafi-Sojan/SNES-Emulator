#pragma once

#include <cstdint>
#include <array>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include "memory.h"


class Bus
{
public:
    Bus();

    uint8_t read(uint32_t address);
    void write(uint32_t address, uint8_t data);

private:
    Memory wram;
};