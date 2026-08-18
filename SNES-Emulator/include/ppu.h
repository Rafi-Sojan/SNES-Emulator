#pragma once

#include <array>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <cstdint>

class ppu {
public:
    ppu();

    // PPU register range 0x2100-0x213F.
    uint8_t readRegister(uint16_t address);
    void writeRegister(uint16_t address, uint8_t value);

    void reset(); // resets all the attributes to default

    // 0x2100-0x2104: display and OAM write ports
    void INIDISP(uint8_t value); // 0x2100
    void OBJSEL(uint8_t value); // 0x2101
    void OAMADDL(uint8_t value); // 0x2102
    void OAMADDH(uint8_t value); // 0x2103
    void OAMDATA(uint8_t value); // 0x2104

    // 0x2105-0x210C: background-mode and tile-map configuration
    void BGMODE(uint8_t value); // 0x2105
    void MOSAIC(uint8_t value); // 0x2106
    void BG1SC(uint8_t value); // 0x2107
    void BG2SC(uint8_t value); // 0x2108
    void BG3SC(uint8_t value); // 0x2109
    void BG4SC(uint8_t value); // 0x210A
    void BG12NBA(uint8_t value); // 0x210B
    void BG34NBA(uint8_t value); // 0x210C

    // 0x210D-0x2114: background scroll ports
    void BG1HOFS(uint8_t value); // 0x210D
    void BG1VOFS(uint8_t value); // 0x210E
    void BG2HOFS(uint8_t value); // 0x210F
    void BG2VOFS(uint8_t value); // 0x2110
    void BG3HOFS(uint8_t value); // 0x2111
    void BG3VOFS(uint8_t value); // 0x2112
    void BG4HOFS(uint8_t value); // 0x2113
    void BG4VOFS(uint8_t value); // 0x2114

    // 0x2115-0x2119: VRAM address and data ports
    void VMAIN(uint8_t value); // 0x2115
    void VMADDL(uint8_t value); // 0x2116
    void VMADDH(uint8_t value); // 0x2117
    void VMDATAL(uint8_t value); // 0x2118
    void VMDATAH(uint8_t value); // 0x2119

    // 0x211A-0x2120: Mode 7 configuration and matrix ports
    void M7SEL(uint8_t value); // 0x211A
    void M7A(uint8_t value); // 0x211B
    void M7B(uint8_t value); // 0x211C
    void M7C(uint8_t value); // 0x211D
    void M7D(uint8_t value); // 0x211E
    void M7X(uint8_t value); // 0x211F
    void M7Y(uint8_t value); // 0x2120

    // 0x2121-0x2122: CGRAM write ports
    void CGADD(uint8_t value); // 0x2121
    void CGDATA(uint8_t value); // 0x2122

    // 0x2123-0x212B: window configuration
    void W12SEL(uint8_t value); // 0x2123
    void W34SEL(uint8_t value); // 0x2124
    void WOBJSEL(uint8_t value); // 0x2125
    void WH0(uint8_t value); // 0x2126
    void WH1(uint8_t value); // 0x2127
    void WH2(uint8_t value); // 0x2128
    void WH3(uint8_t value); // 0x2129
    void WBGLOG(uint8_t value); // 0x212A
    void WOBJLOG(uint8_t value); // 0x212B

    // 0x212C-0x2133: screen designation, color math, and display configuration
    void TM(uint8_t value); // 0x212C
    void TS(uint8_t value); // 0x212D
    void TMW(uint8_t value); // 0x212E
    void TSW(uint8_t value); // 0x212F
    void CGWSEL(uint8_t value); // 0x2130
    void CGADSUB(uint8_t value); // 0x2131
    void COLDATA(uint8_t value); // 0x2132
    void SETINI(uint8_t value); // 0x2133

    // 0x2134-0x2136: Mode 7 multiplication result read ports
    uint8_t MPYL() const; // 0x2134
    uint8_t MPYM() const; // 0x2135
    uint8_t MPYH() const; // 0x2136

    // 0x2137-0x213F: latch, OAM/VRAM/CGRAM, counter, and status read ports
    void SLHV(uint8_t value); // 0x2137, write latches H/V counters
    uint8_t OAMDATAREAD(); // 0x2138
    uint8_t VMDATALREAD(); // 0x2139
    uint8_t VMDATAHREAD(); // 0x213A
    uint8_t CGDATAREAD(); // 0x213B
    uint8_t OPHCT(); // 0x213C
    uint8_t OPVCT(); // 0x213D
    uint8_t STAT77() const; // 0x213E
    uint8_t STAT78(); // 0x213F

private:
    void incrementVRAMAddress(); // increments the VRam Address
    uint16_t vramByteAddress() const; // gets the VRam Byte ADdress

    std::array<uint8_t, 0x40> registers{}; 
    std::array<uint8_t, 0x10000> vram{}; // Simulating the number of storable addresses for VRAM
    std::array<uint8_t, 544> oam{}; // Simulating the number of storable addresses for OAM
    std::array<uint8_t, 512> cgram{}; // Simulating the number of storable addresses for CGRAM

    uint16_t vramAddress = 0; // Vram Address
    uint16_t oamWordAddress = 0; // OAM Word Address
    uint16_t oamByteAddress = 0; // OAM Byte Address
    uint16_t cgramAddress = 0; // CGRAM Address

    uint8_t vmain = 0; // video port control register
    uint8_t oamLatch = 0; // handling sprite data
    uint8_t cgramLatch = 0; // handling pixel data
    bool cgramHighByte = false; 
    bool hCounterHighByte = false;
    bool vCounterHighByte = false;

    uint16_t hCounter = 0;
    uint16_t vCounter = 0;
    uint8_t openBus = 0;
};
