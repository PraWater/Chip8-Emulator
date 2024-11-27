#include "chip8.h"

int main() {
  chip8 ch = chip8();

  ch.loadApplication("pong2.c8");

  while (1) {
    ch.emulateCycle();
    //TODO draw pixels on screen.
  }
  return 0;
}
