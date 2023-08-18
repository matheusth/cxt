#include <stdlib.h>
#include <SDL2/SDL_types.h>
#ifndef BUFFER_H_
#define BUFFER_H_

#define LINE_INIT_CAPACITY 1024

typedef struct {
    size_t capacity;
    size_t size;
    char *chars;
} Line;

void line_insert_text(Line *line, const char * text, size_t col);
void line_backspace(Line *line, size_t col);
void line_delete(Line *line, size_t col);
#endif
