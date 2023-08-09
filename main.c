#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

int main() {
    scc(SDL_Init(SDL_INIT_VIDEO));
    scc(TTF_Init());
    TTF_Font *font = scp(TTF_OpenFont("FiraCodeNerdFontMono-Regular.ttf", 32));

    SDL_Window *window =
        SDL_CreateWindow("CXT", 0, 0, 800, 600, SDL_WINDOW_RESIZABLE);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Color color = {255, 255, 255, 0};
    SDL_Surface * surface = TTF_RenderText_Blended(font, "Hello World!", color);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);

    bool quit = false;
    
    while (!quit) {
        SDL_Event event = {0};
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit=true;
                    break;
            }
        }
        scc(SDL_SetRenderDrawColor(renderer, 100, 0, 0, 0));
        scc(SDL_RenderClear(renderer));
        int textW = 0, textH=0;
        SDL_QueryTexture(texture, NULL, NULL, &textW, &textH);
        SDL_Rect dstRect = {0,0, textW, textH};
        SDL_RenderCopy(renderer, texture, NULL, &dstRect);
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    SDL_DestroyRenderer(renderer);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
