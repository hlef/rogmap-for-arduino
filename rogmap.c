#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "rogmap.h"

#define ARR_SIZE(arr) (sizeof((arr)) / sizeof((arr[0])) )

// Simulate map[x][y] for a 1-dimensional array
#define ACCESS_XY_IN_ARRAY(map, x, y) map->elements[y*map->width + x]

#define MIN(a,b) \
  ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b; })

int main() {
    srand((unsigned) time(NULL));

    int width = 80, height = 40;

    map_t *map = malloc(sizeof(map_t));
    *map = (map_t) { .elements = malloc(height*width*sizeof(char)), .height=height, .width=width };

    fill_map(map, RANDOM);

    // Display map
    for ( int i = 0; i < height*width; i++ ) {
        printf("%c", map->elements[i]);
        if ( (i+1) % width == 0 ) {
            printf("\n");
        }
    }

    free(map->elements);
    free(map);
    return 1;
}

int fill_map(map_t* map, map_type_t map_type) {
    // Allocate memory
    int map_size = map->width * map->height;

    listing_t* selectable_space = malloc(sizeof(listing_t));

    if ( !selectable_space ) {
        return -1;
    }

    *selectable_space = (listing_t) { .coordinates = malloc(map_size * sizeof(coordinate)),
                                      .size = map_size };

    if ( !selectable_space->coordinates ) {
        free(selectable_space);
        return -1;
    }

    listing_t* room_buffer = malloc(sizeof(listing_t));

    if ( !room_buffer ) {
        free(selectable_space->coordinates);
        free(selectable_space);
        return -1;
    }

    *room_buffer = (listing_t) { .coordinates = calloc(map_size, sizeof(coordinate)),
                                 .size = map_size };

    if ( !room_buffer->coordinates ) {
        free(selectable_space->coordinates);
        free(selectable_space);
        free(room_buffer);
        return -1;
    }

    // Initialize selectable_space
    for ( int i = 0, y = 0; y < map->height; y++ ) {
        for ( int x = 0; x < map->width; x++ ) {
            if( x < map->width && y < map->height ) {
                selectable_space->coordinates[i] = (coordinate) { .x = x, .y = y };
                i++;
            }
        }
    }

    // Initialize map
    memset(map->elements, CHAR_EMPTY, map_size * sizeof(char));

    // Define room_number
    int room_number;
    switch (map_type) {
        case RANDOM:
            room_number = randrange(15, 6);
            break;
        case SMALLROOMS:
            room_number = randrange(20, 6);
            break;
        case BIGROOMS:
            room_number = randrange(10, 5);
            break;
        case BOSS:
            room_number = 1;
            break;
        default:
            return -1;
    }

    // Generation loop
    int i = 0;
    do {
        switch (map_type) {
            case RANDOM:
                get_room_generator()(map, selectable_space, room_buffer, 0.5f);
                break;
            case BOSS:
                generate_elliptic_room(map, selectable_space, room_buffer, 0.6f);
                break;
            default:
                generate_rectangular_room(map, selectable_space, room_buffer, map_type == BIGROOMS ? 0.8f : 0.3f);
        }

        insert_room(map, room_buffer);
        memcpy(selectable_space->coordinates, room_buffer->coordinates, room_buffer->size * sizeof(coordinate));
        selectable_space->size = room_buffer->size;
        i++;
    } while (i < room_number);

    free(selectable_space->coordinates);
    free(selectable_space);
    free(room_buffer->coordinates);
    free(room_buffer);
    return 0;
}

void insert_room(map_t* map, listing_t* room) {
    for ( int i = 0; i < room->size; i++ ) {
        ACCESS_XY_IN_ARRAY(map, room->coordinates[i].x, room->coordinates[i].y) = CHAR_ROOM;
    }
}

int is_suitable_initial_point(map_t* map, coordinate initial_point, int dir_right, int dir_up) {
    int valid_height;

    if (dir_up) {
        valid_height = initial_point.y > 2;
    } else {
        valid_height = map->height - initial_point.y > 2;
    }

    if (dir_right) {
        return map->width - initial_point.x > 2 && valid_height;
    } else {
        return initial_point.x > 2 && valid_height;
    }
}

/* Return pseudo random number in range [min, max).
   Assumes min < max <= RAND_MAX */
int randrange(int max, int min) {
    max -= (min + 1);

    unsigned int num_bins = (unsigned int) max + 1,
                 num_rand = (unsigned int) RAND_MAX + 1,
                 bin_size = num_rand / num_bins,
                 defect   = num_rand % num_bins, x;

    do {
        x = rand();
    } while ( num_rand - defect <= (unsigned int) x );

    return (x / bin_size) + min;
}

/* Generate a rectangular room in passed map, with at least one point in passed
   selectable_space array. Write the coordinates of the generated room's points
   to passed room_buffer */
void generate_rectangular_room(map_t* map, listing_t* selectable_space, listing_t* room_buffer, float max_room_size_factor) {
    coordinate initial_point;

    // 1 for right, 0 for left
    int dir_right = randrange(2, 0);

    // 1 for up, 0 for down
    int dir_up = randrange(2, 0);

    do {
        initial_point = selectable_space->coordinates[rand() % selectable_space->size];
    } while ( !is_suitable_initial_point(map, initial_point, dir_right, dir_up) );

    int width, height;

    if (dir_up) {
        height = randrange(MIN(map->height * max_room_size_factor, initial_point.y), 2);
    } else {
        height = randrange(MIN(map->height * max_room_size_factor, map->height - initial_point.y), 2);
    }

    if (dir_right) {
        width = randrange(MIN(map->width * max_room_size_factor, map->width - initial_point.x), 2);
    } else {
        width = randrange(MIN(map->width * max_room_size_factor, initial_point.x), 2);
    }

    int x, y = initial_point.y, i = 0;
    for ( int h = 0; h < height; h++ ) {
        x = initial_point.x;

        for ( int w = 0; w < width; w++ ) {
            room_buffer->coordinates[i] = (coordinate) { .x = x, .y = y };
            dir_right ? x++ : x--;
            i++;
        }

        dir_up ? y-- : y++;
    }

    room_buffer->size = i;
}

/* Generate an elliptic room in passed map, with at least one point in passed
   selectable_space array. Write the coordinates of the generated room's points
   to passed room_buffer */
void generate_elliptic_room(map_t* map, listing_t* selectable_space, listing_t* room_buffer, float max_room_size_factor) {
    coordinate initial_point;

    do {
        initial_point = selectable_space->coordinates[rand() % selectable_space->size];
    } while ( (initial_point.y <= 2) || (map->height - initial_point.y <= 2) ||
               (map->width - initial_point.x <= 2) || (initial_point.x <= 2) );

    int max_height = MIN(map->height - initial_point.y, initial_point.y);
    max_height = MIN(max_height, map->height * max_room_size_factor);

    int max_width = MIN(map->width - initial_point.x, initial_point.x);
    max_width = MIN(max_width, map->width * max_room_size_factor);

    int height = randrange(max_height, 2);
    int width = randrange(max_width, 2);

    int squarew = width*width;
    int squareh = height*height;

    int i = 0;
    for ( int y = -height; y <= height; y++ ) {
        for ( int x = -width; x <= width; x++ ) {
            if ( (x * x * squareh + (y * y) * squarew) <= (squareh * squarew) ) {
                room_buffer->coordinates[i] = (coordinate) { .x = initial_point.x + x, .y = initial_point.y + y };
                i++;
            }
        }
    }

    room_buffer->size = i;
}

/* Return a randomly-chosen element from the rooms_generators array */
void ( *get_room_generator() ) (map_t*, listing_t*, listing_t*, float) {
    return rooms_generators[randrange(ARR_SIZE(rooms_generators), 0)];
}
