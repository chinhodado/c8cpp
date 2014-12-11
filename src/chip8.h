#pragma once
#include <string>

using byte    = unsigned char;
using u_short = unsigned short;

union SDL_Event;

class Chip8 {
public:
	Chip8() {};
	~Chip8() {};
	static const int MEMORY_SIZE     = 4096;	
	static const int NUM_REGISTERS   = 16;
	static const int NUM_LEVEL_STACK = 16;
	static const int SCREEN_WIDTH    = 64;
	static const int SCREEN_HEIGHT   = 32;
	static const int PROGRAM_START_LOC = 0x200;

    std::string gameName;

	u_short opcode;

	/* The memory of the system */
	byte memory[MEMORY_SIZE];

	/* CPU registers */
	byte V[NUM_REGISTERS];

	/* Index register */
	u_short I;

	/* Program counter */
	u_short pc;

	/* Pixel state map representing our screen. Each pixel is a byte in our map (not a bit) */
	byte gfx[SCREEN_HEIGHT][SCREEN_WIDTH];

	/* The delay timer */
	byte delay_timer;

	/* The sound timer */
	byte sound_timer;

	/* The stack */
	u_short stack[NUM_LEVEL_STACK];

	/* The stack pointer, points to first free space on the stack. sp = NUM_LEVEL_STACK if stack is full */
	u_short sp;

	/* Keypad, holds the keys' state */
	byte keys[16];

	/* Draw flag, set to true if we need to draw in the current cycle */
	bool drawFlag;

	/* Initialize the system */
	void initialize();

	/* Emulate one CPU cycle */
	void emulateCycle();

	/* Load the game into memory */
	void loadGame();

	/* Handle key event */
	void handleKey(const SDL_Event& e);

	/* Clear the mem-mapped screen */
	void clearScreen();
private:
};
