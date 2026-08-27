#include "../include/apu.h"

Spc_700::Spc_700()
{
	using ref = Spc_700;

	// 256-entry SPC700 opcode lookup.

	//screw this again, i need to waste 2 hours for building this opcode map like that 65c816

	lookup =
	{
		// 0                                // 1                                // 2                                // 3                                // 4                                // 5                                // 6                                // 7                                // 8                                // 9                                // A                                // B                                // C                                // D                                // E                                // F
		  {"NOP", &ref::, &ref::, }, {"JSTO", &ref::, &ref::, }, {"SET1", &ref::, &ref::, }, {"BBS", &ref::, &ref::, }, {"ORA", &ref::, &ref::, }, {"ORA", &ref::, &ref::, }, {"ORA", &ref::, &ref::, }, {"ORA", &ref::, &ref::, }, {"ORA", &ref::, &ref::, }, {"OR", &ref::, &ref::, }, {"ORC", &ref::, &ref::, }, {"ASL", &ref::, &ref::, }, {"ASL", &ref::, &ref::, }, {"PHP", &ref::, &ref::, }, {"SET1", &ref::, &ref::, }, {"BRK", &ref::, &ref::, },
		  {"BPL", &ref::, &ref::, }, {"JST1", &ref::, &ref::, }, {"CLR1", &ref::, &ref::, }, {"BBC", &ref::, &ref::, }, {"ORA", &ref::, &ref::, }, {"ORA", &ref::, &ref::, }, {"ORA", &ref::, &ref::, }, {"ORA", &ref::, &ref::, }, {"OR", &ref::, &ref::, }, {"OR", &ref::, &ref::, }, {"DEW", &ref::, &ref::, }, {"ASL", &ref::, &ref::, }, {"ASL", &ref::, &ref::, }, {"DEX", &ref::, &ref::, }, {"CPX", &ref::, &ref::, }, {"JMP", &ref::, &ref::, },
		  {"CPL", &ref::, &ref::, }, {"JST2", &ref::, &ref::, }, {"SET1", &ref::, &ref::, }, {"BBS", &ref::, &ref::, }, {"AND", &ref::, &ref::, }, {"AND", &ref::, &ref::, }, {"AND", &ref::, &ref::, }, {"AND", &ref::, &ref::, }, {"AND", &ref::, &ref::, }, {"AND", &ref::, &ref::, }, {"ORC", &ref::, &ref::, }, {"ROL", &ref::, &ref::, }, {"ROL", &ref::, &ref::, }, {"PHA", &ref::, &ref::, }, {"CBNE", &ref::, &ref::, }, {"BRA", &ref::, &ref::, },
		  {"BMI", &ref::, &ref::, }, {"JST3", &ref::, &ref::, }, {"CLR1", &ref::, &ref::, }, {"BBC", &ref::, &ref::, }, {"AND", &ref::, &ref::, }, {"AND", &ref::, &ref::, }, {"AND", &ref::, &ref::, }, {"AND", &ref::, &ref::, }, {"AND", &ref::, &ref::, }, {"AND", &ref::, &ref::, }, {"INW", &ref::, &ref::, }, {"ROL", &ref::, &ref::, }, {"ROL", &ref::, &ref::, }, {"INX", &ref::, &ref::, }, {"CPX", &ref::, &ref::, }, {"JSR", &ref::, &ref::, },
		  {"SEP", &ref::, &ref::, }, {"JST4", &ref::, &ref::, }, {"SET1", &ref::, &ref::, }, {"BBS", &ref::, &ref::, }, {"EORA", &ref::, &ref::, }, {"EORA", &ref::, &ref::, }, {"EORA", &ref::, &ref::, }, {"EORA", &ref::, &ref::, }, {"EORA", &ref::, &ref::, }, {"EOR", &ref::, &ref::, }, {"ANDC", &ref::, &ref::, }, {"LSR", &ref::, &ref::, }, {"LSR", &ref::, &ref::, }, {"PHX", &ref::, &ref::, }, {"CLR1", &ref::, &ref::, }, {"JSP", &ref::, &ref::, },
		  {"BVC", &ref::, &ref::, }, {"JST5", &ref::, &ref::, }, {"CLR1", &ref::, &ref::, }, {"BBC", &ref::, &ref::, }, {"EORA", &ref::, &ref::, }, {"EORA", &ref::, &ref::, }, {"EORA", &ref::, &ref::, }, {"EORA", &ref::, &ref::, }, {"EOR", &ref::, &ref::, }, {"EOR", &ref::, &ref::, }, {"CPW", &ref::, &ref::, }, {"LSR", &ref::, &ref::, }, {"LSR", &ref::, &ref::, }, {"TAX", &ref::, &ref::, }, {"CPY", &ref::, &ref::, }, {"JMP", &ref::, &ref::, },
		  {"CLC", &ref::, &ref::, }, {"JST6", &ref::, &ref::, }, {"SET1", &ref::, &ref::, }, {"BBS", &ref::, &ref::, }, {"CMP", &ref::, &ref::, }, {"CMP", &ref::, &ref::, }, {"CMP", &ref::, &ref::, }, {"CMP", &ref::, &ref::, }, {"CMP", &ref::, &ref::, }, {"CMP", &ref::, &ref::, }, {"ANDC", &ref::, &ref::, }, {"ROR", &ref::, &ref::, }, {"ROR", &ref::, &ref::, }, {"PHY", &ref::, &ref::, }, {"DBNZ", &ref::, &ref::, }, {"RTS", &ref::, &ref::, },

	};
}

