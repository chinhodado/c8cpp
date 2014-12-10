#include <vector>
#include <SDL.h>

using byte    = unsigned char;
using u_short = unsigned short;

class Chip8 {
public:
	Chip8() {};
	~Chip8() {};
	static const int MEMORY_SIZE     = 4096;
	static const int NUM_REGISTERS   = 16;
	static const int NUM_LEVEL_STACK = 16;
	static const int SCREEN_WIDTH	 = 64;
	static const int SCREEN_HEIGHT	 = 32;

	u_short opcode;

	/* The memory of the system*/
	byte memory[MEMORY_SIZE];

	/* CPU registers*/
	byte V[NUM_REGISTERS];

	/* Index register*/
	u_short I;

	/* Program counter*/
	u_short pc;

	/* Pixel state map representing our screen. Each pixel is a byte in our map (not a bit) */
	byte gfx[SCREEN_HEIGHT][SCREEN_WIDTH];

	/* The delay timer */
	byte delay_timer;

	/* The sound timer*/
	byte sound_timer;

	/* The stack*/
	u_short stack[NUM_LEVEL_STACK];

	/* The stack pointer, points to first free space on the stack. sp = NUM_LEVEL_STACK if stack is full*/
	u_short sp;

	/* Keypad*/
	byte key[16];

	/* Draw flag */
	bool drawFlag;

	void initialize();
	void emulateCycle();
	void loadGame();
	void handleKey(SDL_Event e);
	void clearScreen();
private:
};
