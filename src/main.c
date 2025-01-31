#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define DELAY 50000
#define JUMP_STRENGTH -3
#define PIPE_GAP 8
#define PIPE_WIDTH 3
#define GRAVITY 1

int bird_y, bird_x;
int pipe_x, pipe_length;

int velocity;
int score;
int game_over = 0;

void init_game() {
  initscr();
  noecho();
  curs_set(0);
  timeout(0);

  bird_y = LINES / 2;
  bird_x = COLS / 4;
  pipe_x = COLS - 1;
  pipe_length = rand() % (LINES - PIPE_GAP - 2) + 1;
}

void draw_bird() {
  mvprintw(bird_y, bird_x, "O");
}

void draw_pipe() {
  for (int y = 0; y < LINES; y++) {
    if (y < pipe_length || y > pipe_length + PIPE_GAP) {
      for (int i = 0; i < PIPE_WIDTH; i++) {
        mvprintw(y, pipe_x, "|");
      }
    }
  }
}

void update_game() {
  int ch = getch();
  if (ch == ' ' && !game_over) velocity = JUMP_STRENGTH;
  if (ch == 'q') game_over = 1;

  if (!game_over) {
    bird_y += velocity;
    velocity += GRAVITY;

    if (bird_y <= 0 || bird_y >= LINES - 1) game_over = 1;

    pipe_x--;
    if (pipe_x <= 0) {
      pipe_x = COLS - 1;
      pipe_length = rand() % (LINES - PIPE_GAP - 2) + 1;
      score++;
    }

    if (bird_x >= pipe_x && bird_x < pipe_x + PIPE_WIDTH) {
      if (bird_y < pipe_length || bird_y > pipe_length + PIPE_GAP) {
        game_over = 1;
      }
    }
  }
}

void draw_game() {
  clear();
  draw_bird();
  draw_pipe();
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
