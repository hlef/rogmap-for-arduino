#include <stdlib.h>
#include "Rogmap.h"

Rogmap::Rogmap(int width, int height) {
    map = (map_t *) malloc(sizeof(map_t));
    int chunked_width = roundUp(width, sizeof(char) * 8) / (sizeof(char) * 8);
    *map = (map_t) { .width = width, .height = height,
                     .chunked_width = chunked_width,
                     .elements = (chunk_t*) malloc(height * chunked_width * sizeof(char))};
}

void Rogmap::fill(float min_filling, float max_room_size) {
    fill_map(map, min_filling, max_room_size);
}

char Rogmap::get_char_at(int x, int y) {
    return ACCESS_XY_IN_ARRAY(map, x, y);
}

char* Rogmap::as_char_array() {
    return map->elements;
}

int Rogmap::get_height() {
    return map->height;
}

int Rogmap::get_width() {
    return map->width;
}
