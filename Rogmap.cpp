#include "Rogmap.h"

Rogmap::Rogmap(int width, int height) {
    srand(10); // TODO Constant seed is only temporary

    map_t *map = malloc(sizeof(map_t));
    *map = (map_t) { .elements = malloc(height*width*sizeof(char)), .height=height, .width=width };
}

char* Rogmap::fill(float min_filling, float max_room_size) {
    fill_map(map, min_filling, max_room_size)
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
