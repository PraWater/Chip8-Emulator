#include "chip8.h"
#include <cstdio>

void drawPixels(chip8* ch) {
  for (int i = 0; i < 32; ++i) {
    for (int j = i * 64; j < (i + 1) * 64; ++j) {
      if (ch->gfx[j])
        printf("$$");
      else
        printf("  ");
    }
    printf("\n");
  }
}

int main() {
  chip8 ch = chip8();
  ch.loadApplication("invaders.c8");

  while (1) {
    ch.emulateCycle();
    if (ch.drawFlag) {
      drawPixels(&ch);
      /*printf("drawing\n");*/
      ch.drawFlag = false;
    }
  }
  return 0;
}
