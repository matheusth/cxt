#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *text = "Hello World!";

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

    SDL_Surface *font_surface = TTF_RenderText_Blended(font, buffer, color);
    SDL_Texture *font_texture =
        SDL_CreateTextureFromSurface(renderer, font_surface);

    SDL_QueryTexture(font_texture, NULL, NULL, &textW, &textH);
    SDL_Rect dstRect = {0, 0, textW, textH};

    SDL_RenderCopy(renderer, font_texture, NULL, &dstRect);
    SDL_FreeSurface(font_surface);
    SDL_DestroyTexture(font_texture);
}
#define BUFFER_CAPACITY 1024


int main() {

    char buffer[BUFFER_CAPACITY] = {0};
    size_t buffer_size = 0;
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
                    if(buffer_size > 0){
                        buffer_size -= 1;
                        buffer[buffer_size] = '\0';
                    }
                    break;
                }break;
            case SDL_TEXTINPUT: {
                size_t text_size = strlen(event.text.text);
                size_t free_space = BUFFER_CAPACITY - buffer_size;
                if (text_size > free_space) {
                    text_size = free_space;
                }
                memcpy(buffer + buffer_size, event.text.text, text_size);
                buffer_size += text_size;
            } break;
            }
        }
        scc(SDL_SetRenderDrawColor(renderer, 100, 0, 0, 0));
        scc(SDL_RenderClear(renderer));
        render_text(font, renderer, buffer, font_color);
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyRenderer(renderer);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
