#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define DELAY 100000
#define JUMP_STRENGTH -3
#define GRAVITY 1
#define PIPE_GAP 10 //6
#define PIPE_WIDTH 3
#define PIPE_SPACING 20
#define ITEM_COUNT 3
#define STAR_DURATION 100

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
struct Item items[ITEM_COUNT];

int score = 0;
int coin_score = 0;
int game_over = 0;
int star_mode = 0;
int star_timer = 0;
int pipe_count;

void init_game() {
    initscr();
    noecho();
    curs_set(FALSE);
    timeout(0);

    pipe_count = COLS / PIPE_SPACING;
    pipes = malloc(sizeof(struct Pipe) * pipe_count);

    bird.y = LINES / 2;
    bird.x = COLS / 4;
    bird.velocity = 0;

    for (int i = 0; i < pipe_count; i++) {
        pipes[i].x = COLS + i * PIPE_SPACING;
        pipes[i].hole_y = rand() % (LINES - PIPE_GAP - 2) + 1;
    }

    for (int i = 0; i < ITEM_COUNT; i++) {
        int pipe_index = i % pipe_count;
        items[i].x = pipes[pipe_index].x + PIPE_WIDTH + 2;
        items[i].y = pipes[pipe_index].hole_y + PIPE_GAP / 2;
        items[i].type = (rand() % 5 == 0) ? '*' : '0';
    }
}

void draw_bird() {
    mvprintw(bird.y, bird.x, "O");
}

void draw_pipes() {
    for (int i = 0; i < pipe_count; i++) {
        for (int y = 0; y < LINES; y++) {
            if (y < pipes[i].hole_y || y > pipes[i].hole_y + PIPE_GAP) {
                for (int w = 0; w < PIPE_WIDTH; w++) {
                    mvprintw(y, pipes[i].x + w, "###");
                }
            }
        }
    }
}

void draw_items() {
    for (int i = 0; i < ITEM_COUNT; i++) {
        if (items[i].x > 0) {
            mvprintw(items[i].y, items[i].x, "%c", items[i].type);
        }
    }
}

void update_game() {
    int ch = getch();
    if (ch == ' ' && !game_over) bird.velocity = JUMP_STRENGTH;
    if (ch == 'q') game_over = 1;

    if (!game_over) {
        bird.y += bird.velocity;
        bird.velocity += GRAVITY;

        if (bird.y <= 0 || bird.y >= LINES - 1) game_over = 1;

        for (int i = 0; i < pipe_count; i++) {
            pipes[i].x -= (star_mode ? 2 : 1);

            if (pipes[i].x <= 0) {
                pipes[i].x = COLS - 1;
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

        for (int i = 0; i < ITEM_COUNT; i++) {
            items[i].x -= (star_mode ? 2 : 1);

            if (items[i].x <= 0) {
                int pipe_index = rand() % pipe_count;
                items[i].x = pipes[pipe_index].x + PIPE_WIDTH + 2;
                items[i].y = pipes[pipe_index].hole_y + PIPE_GAP / 2;
                items[i].type = (rand() % 5 == 0) ? '*' : '0';
            }

            if (abs(bird.x - items[i].x) <= 1 && bird.y == items[i].y) {
                if (items[i].type == '0') {
                    coin_score++;
                } else if (items[i].type == '*') {
                    star_mode = 1;
                    star_timer = STAR_DURATION;
                }
                items[i].x = -1;
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

void draw_game() {
    clear();
    draw_bird();
    draw_pipes();
    draw_items();
    mvprintw(0, 0, "Score: %d | Coins: %d | %s", score, coin_score, star_mode ? "STAR MODE!" : "");
    if (game_over) mvprintw(LINES / 2, COLS / 2 - 5, "GAME OVER! Press 'q' to exit.");
    refresh();
}

void cleanup_game() {
    free(pipes);
    endwin();
}

int main() {
    srand(time(0));
    init_game();

    while (!game_over) {
        update_game();
        draw_game();
        usleep(DELAY);
    }

    cleanup_game();
    return 0;
}
