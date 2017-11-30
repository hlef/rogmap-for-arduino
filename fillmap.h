#ifndef ROGMAP_H
#define ROGMAP_H
#define CHAR_ROOM 1
#define CHAR_EMPTY 0

typedef struct {
  unsigned short x, y;
} coordinate;

typedef struct {
  char e1 : 1;
  char e2 : 1;
  char e3 : 1;
  char e4 : 1;
  char e5 : 1;
  char e6 : 1;
  char e7 : 1;
  char e8 : 1;
} chunk_t;

typedef struct {
  int width, height, chunked_width;
  chunk_t* chunks;
} map_t;

int fill_map(map_t* map_buffer, float min_filling, float max_room_size);

int add_point(map_t* map, coordinate point);

int is_suitable_initial_point(map_t* map, coordinate initial_point);

int generate_elliptic_room(map_t* map, coordinate* point, float max_room_size_factor);

void compute_room_dimensions(map_t* map, coordinate* point, int *height, int *width, float max_room_size_factor);

unsigned int roundUp(unsigned int value, unsigned int roundTo);
unsigned int roundDown(unsigned int value, unsigned int roundTo);

#endif
