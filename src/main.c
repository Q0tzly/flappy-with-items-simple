#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define DELAY 100000
#define JUMP_STRENGTH -3
#define GRAVITY 1
#define PIPE_GAP 10
#define PIPE_WIDTH 3
#define PIPE_COUNT 3
#define PIPE_SPACING 50

struct Bird{
    int y, x;
    int velocity;
};

struct Pipe {
    int x;
    int hole_y;
};

struct Bird bird;
struct Pipe pipes[PIPE_COUNT];
int score = 0;
int game_over = 0;

void init_game() {
    initscr();
    noecho();
    curs_set(FALSE);
    timeout(0);

    bird.y = LINES / 2;
    bird.x = COLS / 4;
    bird.velocity = 0;

    for (int i = 0; i < PIPE_COUNT; i++) {
        pipes[i].x = COLS + i * PIPE_SPACING;
        pipes[i].hole_y = rand() % (LINES - PIPE_GAP - 2) + 1;
    }
}

void draw_bird() {
    mvprintw(bird.y, bird.x, "O");
}

void draw_pipes() {
    for (int i = 0; i < PIPE_COUNT; i++) {
        for (int y = 0; y < LINES; y++) {
            if (y < pipes[i].hole_y || y > pipes[i].hole_y + PIPE_GAP) {
                for (int w = 0; w < PIPE_WIDTH; w++) {
                    mvprintw(y, pipes[i].x + w, "###");
                }
            }
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

        for (int i = 0; i < PIPE_COUNT; i++) {
            pipes[i].x--;

            if (pipes[i].x <= 0) {
                pipes[i].x = COLS - 1;
                pipes[i].hole_y = rand() % (LINES - PIPE_GAP - 2) + 1;
                score++;
            }

            if (bird.x >= pipes[i].x && bird.x < pipes[i].x + PIPE_WIDTH) {
                if (bird.y < pipes[i].hole_y || bird.y > pipes[i].hole_y + PIPE_GAP) {
                    game_over = 1;
                }
            }
        }
    }
}

void draw_game() {
    clear();
    draw_bird();
    draw_pipes();
    mvprintw(0, 0, "Score: %d", score);
    if (game_over) mvprintw(LINES / 2, COLS / 2 - 5, "GAME OVER! Press 'q' to exit.");
    refresh();
}

void cleanup_game() {
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
