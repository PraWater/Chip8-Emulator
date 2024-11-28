#include "chip8.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

unsigned char fontset[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

chip8::chip8() {
  cout << "Hi" << endl;
  pc = 0x200;
  opcode = 0;
  I = 0;
  sp = 0;

  // Clearing pixels, stack, registers and memory. Is this really required?
  for (int i = 0; i < 64 * 32; ++i) {
    gfx[i] = false;
  }
  for (int i = 0; i < 16; ++i) {
    stack[i] = 0;
  }
  for (int i = 0; i < 16; ++i) {
    V[i] = 0;
  }
  for (int i = 0; i < 4096; ++i) {
    memory[i] = 0x00;
  }

  // Loading fontset.
  for (int i = 0; i < 80; ++i) {
    memory[i] = fontset[i];
  }
  drawFlag = true;

  srand(time(NULL));
}

chip8::~chip8() { cout << "Bye" << endl; }

void chip8::emulateCycle() {
  opcode = memory[pc] << 8 | memory[pc + 1];
  /*printf("pc: %d, opcode: 0x%x\n", pc, opcode);*/
  switch (opcode & 0xF000) {

  case 0x0000: // 1xxx calls jump to xxx.
    switch (opcode & 0x000F) {
    case 0x0000: // 00E0 clears the screen.
      for (int i = 0; i < 2048; ++i)
        gfx[i] = false;
      drawFlag = true;
      pc += 2;
      break;

    case 0x000E: // 00EE returns from subroutine.
      --sp;
      pc = stack[sp];
      break;

    default:
      printf("Opcode not handled yet: 0x%X\n", opcode);
      exit(1);
      break;
    }
    break;

  case 0x1000: // 1xxx calls jump to xxx.
    pc = opcode & 0x0FFF;
    break;

  case 0x2000: // 2xxx calls subroutine at xxx.
    stack[sp] = pc;
    ++sp;
    pc = opcode & 0x0FFF;
    break;

  case 0x3000: // 3xkk - if Vx == kk, next instruction is skipped.
    if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
      pc += 2;
    pc += 2;
    break;

  case 0x4000: // 4xkk - if Vx != kk, next instruction is skipped.
    if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
      pc += 2;
    pc += 2;
    break;

  case 0x5000: // 5xy0 - if Vx == Vy, next instruction is skipped.
    if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
      pc += 2;
    pc += 2;
    break;

  case 0x6000: // 6xkk puts kk into Vx
    V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
    pc += 2;
    break;

  case 0x7000: // 7xkk adds kk to Vx
    V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
    pc += 2;
    break;

  case 0x8000:
    switch (opcode & 0x000F) {

    case 0x0000: // 8xy0 - set Vx = Vy.
      V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
      pc += 2;
      break;

    case 0x0001: // 8xy1 - set Vx = Vx | Vy.
      V[(opcode & 0x0F00) >> 8] =
          V[(opcode & 0x0F00) >> 8] | V[(opcode & 0x00F0) >> 4];
      pc += 2;
      break;

    case 0x0002: // 8xy1 - set Vx = Vx & Vy.
      V[(opcode & 0x0F00) >> 8] =
          V[(opcode & 0x0F00) >> 8] & V[(opcode & 0x00F0) >> 4];
      pc += 2;
      break;

    case 0x0003: // 8xy1 - set Vx = Vx ^ Vy.
      V[(opcode & 0x0F00) >> 8] =
          V[(opcode & 0x0F00) >> 8] ^ V[(opcode & 0x00F0) >> 4];
      pc += 2;
      break;

    default:
      printf("Opcode not handled yet: 0x%X\n", opcode);
      exit(1);
      break;
    }
    break;

  case 0x9000: // 9xy0 - if Vx != Vy, next instruction is skipped.
    if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4])
      pc += 2;
    pc += 2;
    break;

  case 0xA000: // Axxx moves I to xxx.
    I = opcode & 0x0FFF;
    pc += 2;
    break;

  case 0xB000: // Bxxx moves pc to xxx + V0.
    pc = opcode & 0x0FFF;
    pc += V[0];
    break;

  case 0xC000: // Cxkk moves pc to xxx + V0.
    V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF) & (rand() % 256);
    pc += 2;
    break;

  case 0xD000: // Dxyn - draws n bytes starting from I at (Vx, Vy).
  {
    unsigned short x = V[(opcode & 0x0F00) >> 8];
    unsigned short y = V[(opcode & 0x00F0) >> 4];
    unsigned short height = opcode & 0x000F;
    unsigned short pixel;

    V[0xF] = 0;
    for (int yline = 0; yline < height; yline++) {
      pixel = memory[I + yline];
      for (int xline = 0; xline < 8; xline++) {
        if ((pixel & (0x80 >> xline)) != 0) {
          if (gfx[(x + xline + ((y + yline) * 64))] == 1) {
            V[0xF] = 1;
          }
          gfx[x + xline + ((y + yline) * 64)] ^= 1;
        }
      }
    }
    drawFlag = true;
    pc += 2;
  } break;

  case 0xF000:
    switch (opcode & 0x00FF) {
    case 0x0007: // Fx07 will place delay timer value in Vx.
      V[(opcode & 0x0F00) >> 8] = delay_timer;
      pc += 2;
      break;

    case 0x001E: // Fx1E will add Vx to I.
      I += V[(opcode & 0x0F00) >> 8];
      pc += 2;
      break;

    default:
      printf("Opcode at pc: %d not handled yet: 0x%X\n", pc, opcode);
      exit(1);
      break;
    }
    break;

  default:
    printf("Opcode at pc: %d not handled yet: 0x%X\n", pc, opcode);
    exit(1);
    break;
  }
}

bool chip8::loadApplication(const char *filename) {
  printf("Loading: %s\n", filename);

  FILE *pFile = fopen(filename, "rb");
  if (pFile == NULL) {
    fputs("File error", stderr);
    return false;
  }

  fseek(pFile, 0, SEEK_END);
  long lSize = ftell(pFile);
  rewind(pFile);
  printf("Filesize: %d\n", (int)lSize);

  char *buffer = (char *)malloc(sizeof(char) * lSize);
  if (buffer == NULL) {
    fputs("Memory error", stderr);
    return false;
  }

  size_t result = fread(buffer, 1, lSize, pFile);
  if (result != lSize) {
    fputs("Reading error", stderr);
    return false;
  }

  if ((4096 - 512) > lSize) {
    for (int i = 0; i < lSize; ++i)
      memory[i + 512] = buffer[i];
  } else
    printf("Error: ROM too big for memory");

  fclose(pFile);
  free(buffer);

  return true;
}
