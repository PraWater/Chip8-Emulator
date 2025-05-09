#ifndef CHIP8_H
#define CHIP8_H
class chip8 {
public:
  chip8();
  ~chip8();

  bool drawFlag;

  void emulateCycle();
  void bufferPixels();
  /*void debugRender();*/
  bool loadApplication(const char *filename);

  bool gfx[64 * 32]; // Total amount of pixels: 2048
  bool gfx_buffer[64 * 32];
  bool key[16];

private:
  unsigned short pc;     // Program counter
  unsigned short opcode; // Current opcode
  unsigned short I;      // Index register
  unsigned short sp;     // Stack pointer

  unsigned char V[16];        // V-regs (V0-VF)
  unsigned short stack[16];   // Stack (16 levels)
  unsigned char memory[4096]; // Memory (size = 4k)

  unsigned char delay_timer; // Delay timer
  unsigned char sound_timer; // Sound timer
};
#endif
