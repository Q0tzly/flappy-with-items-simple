#ifndef STRUCT_H
#define STRUCT_H

struct Bird {
    int y, x;
    int velocity;
};

struct Pipe {
    int x;
    int hole_y;
};

struct Item {
    int x, y;
    char type;
};

struct Bird bird;
struct Pipe *pipes;
struct Item *items;

#endif
