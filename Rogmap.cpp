#include <stdlib.h>
#include "Rogmap.h"
#include "src/fillmap.h"

Rogmap::Rogmap(int width, int height) {
    map_t *map = malloc(sizeof(map_t));
    *map = (map_t) { .width=width, .height=height, .elements = malloc(height*width*sizeof(char)) };
}

void Rogmap::fill(float min_filling, float max_room_size) {
    fill_map(&map, min_filling, max_room_size);
}

char Rogmap::get_char_at(int x, int y) {
    return ACCESS_XY_IN_ARRAY(map, x, y);
}

char* Rogmap::as_char_array() {
    return map.elements;
}

int Rogmap::get_height() {
    return map.height;
}

int Rogmap::get_width() {
    return map.width;
}
