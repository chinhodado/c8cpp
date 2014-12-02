using byte = unsigned char;
using u_short = unsigned short;

class Chip8 {
public:
	Chip8 ();
	~Chip8 ();
	static const int MEMORY_SIZE     = 4096;
	static const int NUM_REGISTERS   = 16;
	static const int NUM_LEVEL_STACK = 16;
	static const int SCREEN_WIDTH	 = 64;
	static const int SCREEN_HEIGHT	 = 32;

	u_short opcode;
	byte memory[MEMORY_SIZE];
	byte V[NUM_REGISTERS];   // CPU registers
	u_short I;               // index register
	u_short pc;              // program counter
	byte gfx[64][32];        // pixel state map
	byte delay_timer;
	byte sound_timer;
	u_short stack[NUM_LEVEL_STACK];
	u_short sp;              // stack pointer, points to first free space on the stack. sp = NUM_LEVEL_STACK if stack is full
	byte key[16];            // keypad
	
	void initialize();
	void emulateCycle();
private:

};

Chip8 ::Chip8 () {}

Chip8 ::~Chip8 () {}