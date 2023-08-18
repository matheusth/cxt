#include "buffer.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_CAPACITY 1024
#define UNHEX(color)                                                           \
    ((color) >> (8 * 0)) & 0xFF, ((color) >> (8 * 1)) & 0xFF,                  \
        ((color) >> (8 * 2)) & 0xFF, ((color) >> (8 * 3)) & 0xFF

void scc(int code) {
    if (code < 0) {
        fprintf(stderr, "SDL ERROR: %s\n", SDL_GetError());
        exit(1);
    }
}

void *scp(void *ptr) {
    if (ptr == NULL) {
        fprintf(stderr, "SDL ERROR: %s\n", SDL_GetError());
        exit(1);
    }

    return ptr;
}
void render_text(TTF_Font *font, SDL_Renderer *renderer, char *buffer,
                 SDL_Color color) {
    int textW = 0, textH = 0;

    SDL_Surface *font_surface = TTF_RenderUTF8_Blended(font, buffer, color);
    SDL_Texture *font_texture =
        SDL_CreateTextureFromSurface(renderer, font_surface);

    SDL_QueryTexture(font_texture, NULL, NULL, &textW, &textH);
    SDL_Rect dstRect = {0, 0, textW, textH};

    SDL_RenderCopy(renderer, font_texture, NULL, &dstRect);
    SDL_FreeSurface(font_surface);
    SDL_DestroyTexture(font_texture);
}

void render_cursor(SDL_Renderer *renderer, size_t buffer_cursor, TTF_Font *font,
                   char *buffer, Uint32 color) {
    char *pre_cursor = (char *)calloc(buffer_cursor + 1, sizeof(char));
    memcpy(pre_cursor, buffer, buffer_cursor);
    int w = 0, h = 0;
    TTF_SizeUTF8(font, pre_cursor, &w, &h);
    SDL_Rect rect = {.x = w, .y = 0, .h = h, .w = 2};
    bool blink = false;
    SDL_SetRenderDrawColor(renderer, UNHEX(color));
    SDL_RenderFillRect(renderer, &rect);
    blink = !blink;
}

int main() {

    Line line = {0};
    size_t buffer_cursor = 0;
    scc(SDL_Init(SDL_INIT_VIDEO));
    scc(TTF_Init());
    TTF_Font *font = scp(TTF_OpenFont("FiraCodeNerdFontMono-Regular.ttf", 22));

    SDL_Window *window =
        SDL_CreateWindow("CXT", 0, 0, 800, 600, SDL_WINDOW_RESIZABLE);
    SDL_Renderer *renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Color font_color = {255, 255, 255, 0};

    bool quit = false;

    while (!quit) {
        SDL_Event event = {0};
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_BACKSPACE:
                    if (buffer_cursor > 0) {
                        if ((line.chars[buffer_cursor - 1] & 0x0C0) == 0x080) {
                            line_backspace(&line, buffer_cursor);
                            buffer_cursor--;
                        }
                        line_backspace(&line, buffer_cursor);
                        buffer_cursor--;
                    }
                    break;
                case SDLK_DELETE:
                    if ((line.chars[buffer_cursor + 1] & 0x0C0) == 0x080) {
                        line_delete(&line, buffer_cursor);
                    }
                    line_delete(&line, buffer_cursor);
                    break;
                case SDLK_LEFT: {
                    if ((line.chars[buffer_cursor - 1] & 0x0C0) == 0x080)
                        buffer_cursor--;
                    if (buffer_cursor > 0) {
                        buffer_cursor--;
                    }
                } break;
                case SDLK_RIGHT: {
                    if (buffer_cursor < line.size) {
                        if ((line.chars[buffer_cursor + 1] & 0x0C0) == 0x080)
                            buffer_cursor++;
                        buffer_cursor++;
                    }
                } break;
                }
                break;
            case SDL_TEXTINPUT: {
                line_insert_text(&line, event.text.text, buffer_cursor);
                buffer_cursor += strlen(event.text.text);
            } break;
            }
        }
        scc(SDL_SetRenderDrawColor(renderer, 100, 0, 0, 0));
        scc(SDL_RenderClear(renderer));
        render_text(font, renderer, line.chars, font_color);
        render_cursor(renderer, buffer_cursor, font, line.chars, 0xFFFFFFFF);
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyRenderer(renderer);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
