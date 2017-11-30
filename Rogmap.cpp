#include <stdlib.h>
#include "Rogmap.h"

Rogmap::Rogmap(int width, int height, char char_room, char char_empty):
    char_room(char_room), char_empty(char_empty) {
    map = (map_t *) malloc(sizeof(map_t));
    int chunked_width = roundUp(width, sizeof(char) * 8) / (sizeof(char) * 8);
    *map = (map_t) { .width = width, .height = height,
                     .chunked_width = chunked_width,
                     .chunks = (chunk_t*) malloc(height * chunked_width * sizeof(char))};
}

void Rogmap::fill(float min_filling, float max_room_size) {
    fill_map(map, min_filling, max_room_size);
}

char Rogmap::get_char_at(int x, int y) {
    int nb = get_at_xy(map, x, y);
    char result;

    if (nb == CHAR_EMPTY) {
        result = char_empty;
    } else if (nb == CHAR_ROOM) {
        result = char_room;
    } else {
        result = '?';
    }

    return result;
}

char* Rogmap::as_char_array() {
    char* array = new char[map->height * map->width];

    for (int y = 0; y < map->height; y++) {
        for (int x = 0; y < map->width; x++) {
            array[(y * map->height) + x] = get_char_at(x, y);
        }
    }

    return array;
}

int Rogmap::get_height() {
    return map->height;
}

int Rogmap::get_width() {
    return map->width;
}
