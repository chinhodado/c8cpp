#include "stdafx.h"
#include <stdlib.h>
#include <time.h>
#include "chip8.h"

Chip8 chip8;

int _tmain(int argc, _TCHAR* argv[]) {
	// Set up render system and register input callbacks
	setupGraphics();
	setupInput();

	// Initialize the Chip8 system and load the game into the memory  
	chip8.initialize();
	chip8.loadGame("pong");

	// Emulation loop
	for (;;) {
		// Emulate one cycle
		chip8.emulateCycle();

		// If the draw flag is set, update the screen
		if (chip8.drawFlag)
			drawGraphics();

		// Store key press state (Press and Release)
		chip8.setKeys();
	}

	return 0;
}

void Chip8::initialize() {
	// Initialize registers and memory once
	pc     = 0x200;  // Program counter starts at 0x200
	opcode = 0;      // Reset current opcode	
	I      = 0;      // Reset index register
	sp     = 0;      // Reset stack pointer

	// Clear display	
	// Clear stack
	// Clear registers V0-VF
	// Clear memory

	// Load fontset
	for (int i = 0; i < 80; ++i)
		memory[i] = chip8_fontset[i];

	// Reset timers

	// initialize random seed
	srand(time(NULL));
}

void Chip8::emulateCycle() {
	// Fetch Opcode
	opcode = memory[pc] << 8 | memory[pc + 1];

	// Decode & execute opcode
	switch (opcode & 0xF000) { // switch on first bit
		case 0x0000:
			switch (opcode & 0x000F) {
				case 0x0000: // 0x00E0: clears the screen        
					for (int i = 0; i < SCREEN_WIDTH; i++) {
						for (int j = 0; j < SCREEN_HEIGHT; j++) {
							gfx[i][j] = 0;
						}
					}
					pc += 2;
					break;
				case 0x000E: // 0x00EE: returns from subroutine          
					pc = stack[--sp];
					stack[sp] = 0;
					break;
				default:
					printf("Unknown opcode [0x0000]: 0x%X\n", opcode);
			}
			break;
		case 0x1000: // 1NNN: jumps to address NNN.
			pc = opcode & 0x0FFF;
			break;
		case 0x2000: // 2NNN: calls the subroutine at address NNN
			// TODO: make sure we don't get stack overflow
			stack[sp] = pc;
			++sp;
			pc = opcode & 0x0FFF;
			break;
		case 0x3000: // 3XNN: skips the next instruction if VX equals NN
			byte nn = opcode & 0x00FF;
			byte x = (opcode & 0x0F00) >> 8;
			if (V[x] == nn) {
				pc += 4;
			}
			else {
				pc += 2;
			}
			break;
		case 0x4000: // 4XNN: skips the next instruction if VX doesn't equal NN
			byte nn = opcode & 0x00FF;
			byte x = (opcode & 0x0F00) >> 8;
			if (V[x] != nn) {
				pc += 4;
			}
			else {
				pc += 2;
			}
			break;
		case 0x5000: // 5XY0: skips the next instruction if VX equals VY
			byte x = (opcode & 0x0F00) >> 8;
			byte y = (opcode & 0x00F0) >> 4;
			if (V[x] == V[y]) {
				pc += 4;
			}
			else {
				pc += 2;
			}
			break;
		case 0x6000: // 6XNN: sets VX to NN
			byte x = (opcode & 0x0F00) >> 8;
			byte nn = opcode & 0x00FF;
			V[x] = nn;
			pc += 2;
			break;
		case 0x7000: // 7XNN: adds NN to VX
			byte x = (opcode & 0x0F00) >> 8;
			byte nn = opcode & 0x00FF;
			V[x] += nn; // TODO: what if overflow??
			pc += 2;
			break;
		case 0x8000:
			byte x = (opcode & 0x0F00) >> 8;
			byte y = (opcode & 0x00F0) >> 4;
			switch (opcode & 0x000F) {
				case 0x0000: // 8XY0: sets VX to the value of VY
					V[x] = V[y];
					pc += 2;
					break;
				case 0x0001: // 8XY1: sets VX to VX or VY
					V[x] |= V[y];
					pc += 2;
					break;
				case 0x0002: // 8XY2: sets VX to VX and VY
					V[x] &= V[y];
					pc += 2;
					break;
				case 0x0003: // 8XY3: sets VX to VX xor VY
					V[x] ^= V[y];
					pc += 2;
					break;
				case 0x0004: // 8XY4: adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't.
					V[0xF] = (V[x] + V[y] > 0xFF) ? 1 : 0;
					V[x] += V[y];
					pc += 2;
					break;
				case 0x0005: // 8XY5: VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't
					V[0xF] = (V[x] > V[y]) ? 1 : 0;
					V[x] -= V[y];
					pc += 2;
					break;
				case 0x0006: // 8XY6: shifts VX right by one. VF is set to the value of the least significant bit of VX before the shift
					V[0xF] = V[x] & 0x01;
					V[x] >>= 1;
					pc += 2;
					break;
				case 0x0007: // 8XY7: sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't
					V[0xF] = V[y] > V[x] ? 1 : 0;
					V[x] = V[y] - V[x];
					pc += 2;
					break;
				case 0x0008: // 8XYE: shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift
					V[0xF] = V[x] & 0x80;
					V[x] <<= 1;
					pc += 2;
					break;
				default:
					printf("Unknown opcode: 0x%X\n", opcode);
			}
			break;
		case 0x9000: // 9XY0: skips the next instruction if VX doesn't equal VY
			byte x = (opcode & 0x0F00) >> 8;
			byte y = (opcode & 0x00F0) >> 4;
			if (V[x] != V[y]) {
				pc += 4;
			}
			else {
				pc += 2;
			}
			break;
		case 0xA000: // ANNN: sets I to the address NNN
			I = opcode & 0x0FFF;
			pc += 2;
			break;
		case 0xB000: // BNNN: jumps to the address NNN plus V0
			byte nnn = opcode & 0x0FFF;
			pc = nnn + V[0x0];
			break;
		case 0xC000: // CXNN: sets VX to a random number and NN
			byte nn = opcode & 0x00FF;
			V[x] = nn & rand();
			pc += 2;
			break;
		case 0xF000:
			switch (opcode & 0x00FF) {
				case 0x0033: // FX33: stores the Binary-coded decimal representation of VX at the addresses I, I plus 1, and I plus 2
					memory[I] = V[(opcode & 0x0F00) >> 8] / 100;
					memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
					memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;
					pc += 2;
					break;
				default:
					printf("Unknown opcode: 0x%X\n", opcode);
			}		
		default:
			printf("Unknown opcode: 0x%X\n", opcode);
	}

	// Update timers
	if (delay_timer > 0)
		--delay_timer;

	if (sound_timer > 0) {
		if (sound_timer == 1)
			printf("BEEP!\n");
		--sound_timer;
	}
}

