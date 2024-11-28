#include "chip8.h"
#include <cstdio>

void drawPixels(chip8 *ch) {
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

int main(int argc, char *argv[]) {
  chip8 ch = chip8();
  if (argc < 2) {
    printf("ROM not specified.\n");
    return 1;
  }
  ch.loadApplication(argv[1]);

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
