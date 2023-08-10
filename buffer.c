#include "buffer.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
static void line_extends(Line * line, size_t n){
    size_t new_capacity = line->capacity; 
    assert(new_capacity >= line->size);
    while(new_capacity - line->size < n){
        if(new_capacity == 0){
            new_capacity = LINE_INIT_CAPACITY;
        }else{
            new_capacity *= 2;
        }
    }
    if(new_capacity != line->capacity){
        line->chars = realloc(line->chars, new_capacity);
    }
    line->capacity = new_capacity;

}

void line_insert_text(Line *line, const char * text, size_t col){
    size_t text_size = strlen(text);
    size_t shift = line->size - col;
    line_extends(line, text_size);
    memmove(line->chars + col + text_size, line->chars + col,
            shift);
    memcpy(line->chars + col, text, text_size);
    line->size += text_size;
    line->chars[line->size] = '\0';
}
void line_backspace(Line *line, size_t col){
    if (line->size > 0 && col > 0) {
        memmove(line->chars + col - 1, line->chars + col,
                line->size - col);
        line->size -= 1;
        line->chars[line->size] = '\0';
    }
}
void line_delete(Line *line, size_t col){
    if (line->size > 0 && col < line->size) {
        memmove(line->chars + col, line->chars + col + 1,
                line->size - col);
        line->chars[line->size] = '\0';
        line->size -= 1;
    }
}
