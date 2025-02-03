#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>

#include "config.h"
#include "structs.h"

struct Bird bird;
struct Pipe *pipes;
struct Item *items;

int score = 0;
int coin_score = 0;
int game_over = 0;
int star_mode = 0;
int star_timer = 0;
int pipe_count;
int item_count;

void init_game() {
    initscr();
    noecho();
    curs_set(FALSE);
    timeout(0);

    pipe_count = COLS / PIPE_SPACING;
    pipes = malloc(sizeof(struct Pipe) * pipe_count);
    if (!pipes) {
        endwin();
        fprintf(stderr, "Failed to allocate memory for pipes\n");
        exit(1);
    }

    item_count = COLS / ITEM_SPACING;
    items = malloc(sizeof(struct Item) * item_count);
    if (!items) {
        free(pipes);
        endwin();
        fprintf(stderr, "Failed to allocate memory for items\n");
        exit(1);
    }

    bird.y = LINES / 2;
    bird.x = COLS / 4;
    bird.velocity = 0;

    for (int i = 0; i < pipe_count; i++) {
        pipes[i].x = COLS + i * PIPE_SPACING;
        pipes[i].hole_y = rand() % (LINES - PIPE_GAP - 2) + 1;
    }

    for (int i = 0; i < item_count; i++) {
        int pipe_index = i % pipe_count;
        items[i].x = pipes[pipe_index].x + PIPE_WIDTH + 2;
        items[i].y = pipes[pipe_index].hole_y + PIPE_GAP / 2;
        items[i].type = (rand() % 5 == 0) ? '*' : '0';
    }
}

void update_game() {
    int ch = getch();
    if (ch == ' ' && !game_over) bird.velocity = JUMP_STRENGTH;
    if (ch == 'q') game_over = 1;

    if (!game_over) {
        bird.y += bird.velocity;
        bird.velocity += GRAVITY;


        if (bird.y < 0) bird.y = 0;
        if (bird.y >= LINES - 1) game_over = 1;

        for (int i = 0; i < pipe_count; i++) {
            pipes[i].x -= (star_mode ? 2 : 1);

            if (pipes[i].x + PIPE_WIDTH < 0) {
                pipes[i].x = COLS;
                pipes[i].hole_y = rand() % (LINES - PIPE_GAP - 2) + 1;
                score++;
            }

            if (!star_mode) {
                if (bird.x >= pipes[i].x && bird.x < pipes[i].x + PIPE_WIDTH) {
                    if (bird.y < pipes[i].hole_y || bird.y > pipes[i].hole_y + PIPE_GAP) {
                        game_over = 1;
                    }
                }
            }
        }

        for (int i = 0; i < item_count; i++) {
            items[i].x -= (star_mode ? 2 : 1);

            if (items[i].x <= 0) {
                int pipe_index = rand() % pipe_count;
                items[i].x = pipes[pipe_index].x + PIPE_WIDTH + 2;
                items[i].y = pipes[pipe_index].hole_y + PIPE_GAP / 2;

                if (star_mode) {
                    items[i].type = '0';
                } else {
                    items[i].type = (rand() % 5 == 0) ? '*' : '0';
                }
            }

            if (abs(bird.x - items[i].x) <= 1 && bird.y == items[i].y) {
                if (items[i].type == '0') {
                    coin_score++;
                    items[i].x = -1;
                } else if (items[i].type == '*' && !star_mode) {
                    star_mode = 1;
                    star_timer = STAR_DURATION;
                    items[i].x = -1;
                }
            }
        }

        if (star_mode) {
            star_timer--;
            if (star_timer <= 0) {
                star_mode = 0;
            }
        }
    }
}

void restart_game() {
    game_over = 0;
    score = 0;
    coin_score = 0;
    star_mode = 0;
    star_timer = 0;

    bird.y = LINES / 2;
    bird.x = COLS / 4;
    bird.velocity = 0;

    for (int i = 0; i < pipe_count; i++) {
        pipes[i].x = COLS + i * PIPE_SPACING;
        pipes[i].hole_y = rand() % (LINES - PIPE_GAP - 2) + 1;
    }

    for (int i = 0; i < item_count; i++) {
        int pipe_index = i % pipe_count;
        items[i].x = pipes[pipe_index].x + PIPE_WIDTH + 2;
        items[i].y = pipes[pipe_index].hole_y + PIPE_GAP / 2;
        items[i].type = (rand() % 5 == 0) ? '*' : '0';
    }
}

void cleanup_game() {
    free(pipes);
    free(items);
    endwin();
}
