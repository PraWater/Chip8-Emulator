#include "chip8.h"
#include <SDL2/SDL.h>
#include <cstdio>
#include <cstdlib>

const int PIXEL_SIZE = 15;
const int PADDING = 15;
const int SCREEN_WIDTH = 64 * PIXEL_SIZE + 2 * PADDING;
const int SCREEN_HEIGHT = 32 * PIXEL_SIZE + 2 * PADDING;

void drawPixels(chip8 *ch, SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, 30, 30, 46, 255);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 166, 227, 161, 255);
  for (int i = 0; i < 32; ++i) {
    for (int j = 0; j < 64; ++j) {
      if (ch->gfx[i * 64 + j] || ch->gfx_buffer[i * 64 + j]) {
        SDL_Rect rect = {j * PIXEL_SIZE + PADDING, i * PIXEL_SIZE + PADDING,
                         PIXEL_SIZE, PIXEL_SIZE};
        SDL_RenderFillRect(renderer, &rect);
      }
    }
  }

  SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[]) {
  chip8 ch = chip8();
  if (argc < 2) {
    printf("ROM not specified.\n");
    return 1;
  }
  if (!ch.loadApplication(argv[1]))
    return 1;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return 1;
  }

  SDL_Window *window = SDL_CreateWindow(
      "CHIP-8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (!window) {
    printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer) {
    printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  bool quit = false;
  SDL_Event event;
  unsigned int prev_time = SDL_GetTicks();
  while (!quit) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
        case SDLK_1:
          ch.key[0x1] = true;
          break;
        case SDLK_2:
          ch.key[0x2] = true;
          break;
        case SDLK_3:
          ch.key[0x3] = true;
          break;
        case SDLK_4:
          ch.key[0xC] = true;
          break;
        case SDLK_q:
          ch.key[0x4] = true;
          break;
        case SDLK_w:
          ch.key[0x5] = true;
          break;
        case SDLK_e:
          ch.key[0x6] = true;
          break;
        case SDLK_r:
          ch.key[0xD] = true;
          break;
        case SDLK_a:
          ch.key[0x7] = true;
          break;
        case SDLK_s:
          ch.key[0x8] = true;
          break;
        case SDLK_d:
          ch.key[0x9] = true;
          break;
        case SDLK_f:
          ch.key[0xE] = true;
          break;
        case SDLK_z:
          ch.key[0xA] = true;
          break;
        case SDLK_x:
          ch.key[0x0] = true;
          break;
        case SDLK_c:
          ch.key[0xB] = true;
          break;
        case SDLK_v:
          ch.key[0xF] = true;
          break;
        case SDLK_ESCAPE:
          exit(1);
          break;
        }
      } else if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.sym) {
        case SDLK_1:
          ch.key[0x1] = false;
          break;
        case SDLK_2:
          ch.key[0x2] = false;
          break;
        case SDLK_3:
          ch.key[0x3] = false;
          break;
        case SDLK_4:
          ch.key[0xC] = false;
          break;
        case SDLK_q:
          ch.key[0x4] = false;
          break;
        case SDLK_w:
          ch.key[0x5] = false;
          break;
        case SDLK_e:
          ch.key[0x6] = false;
          break;
        case SDLK_r:
          ch.key[0xD] = false;
          break;
        case SDLK_a:
          ch.key[0x7] = false;
          break;
        case SDLK_s:
          ch.key[0x8] = false;
          break;
        case SDLK_d:
          ch.key[0x9] = false;
          break;
        case SDLK_f:
          ch.key[0xE] = false;
          break;
        case SDLK_z:
          ch.key[0xA] = false;
          break;
        case SDLK_x:
          ch.key[0x0] = false;
          break;
        case SDLK_c:
          ch.key[0xB] = false;
          break;
        case SDLK_v:
          ch.key[0xF] = false;
          break;
        }
      } else if (event.type == SDL_QUIT) {
        quit = true;
      }
    }

    ch.emulateCycle();

    if (ch.drawFlag) {
      drawPixels(&ch, renderer);
      ch.bufferPixels();
      ch.drawFlag = false;
    }

    unsigned int curr_time = SDL_GetTicks();
    unsigned int elapsed_time = curr_time - prev_time;
    if (elapsed_time < 4) {
      SDL_Delay(4 - elapsed_time);
    }
    prev_time = curr_time;
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
