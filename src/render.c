#include <ncurses.h>
#include <unistd.h>

#include "game.h"
#include "structs.h"
#include "config.h"

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
    for (int i = 0; i < item_count; i++) {
        if (items[i].x > 0 && items[i].x < COLS) {
            mvprintw(items[i].y, items[i].x, "%c", items[i].type);
        }
    }
}

void draw_game() {
    clear();
    draw_bird();
    draw_pipes();
    draw_items();

    if (star_mode) {
        if (star_timer <= 20) {
            mvprintw(1, 0, "⚠ STAR ENDING! %d", star_timer);
        } else {
            mvprintw(1, 0, "STAR TIME: %d", star_timer);
        }
    }

    mvprintw(0, 0, "Score: %d | Coins: %d", score, coin_score);
    refresh();
}

void draw_game_over_screen() {
    clear();
    mvprintw(LINES / 2 - 3, COLS / 2 - 5, "GAME OVER");
    mvprintw(LINES / 2, COLS / 2 - 10, "Final Score     : %d", score);
    mvprintw(LINES / 2 + 1, COLS / 2 - 10, "Coins Collected : %d", coin_score);
    mvprintw(LINES / 2 + 3, COLS / 2 - 15, "Press 'Enter' or 'q' to Title");
    refresh();

    int ch;
    while (1) {
        ch = getch();
        if (ch == '\n' || ch == 'q') {
            restart_game();
            return;
        }
        usleep(100000);
    }
}

void draw_help_screen() {
    clear();
    mvprintw(LINES / 2 - 2, COLS / 2 - 10, "--- How to Play ---");
    mvprintw(LINES / 2, COLS / 2 - 6, "Space: Jump");
    mvprintw(LINES / 2 + 2, COLS / 2 - 17, "Press 'h' or 'q', Enter to return");
    refresh();
    int ch;
    while (1) {
        ch = getch();
        if (ch == '\n' || ch == 'h' || ch == 'q') {
            return;
        }
        usleep(100000);
    }
}

void draw_ascii_title() {
    mvprintw(LINES / 2 - 9, (COLS - 40) / 2, "  ______ _                              ");
    mvprintw(LINES / 2 - 8, (COLS - 40) / 2, " /  ____| |         ___    ___   __  __ ");
    mvprintw(LINES / 2 - 7, (COLS - 40) / 2, " | |__  | |  __ _ ,/ _  \\,/ _  \\|  \\|  |");
    mvprintw(LINES / 2 - 6, (COLS - 40) / 2, " |  __| | | / _` || |_| /| |_| / \\_,  |`");
    mvprintw(LINES / 2 - 5, (COLS - 40) / 2, " | |    | || (_| || ,__/ | ,__/  _/  /  ");
    mvprintw(LINES / 2 - 4, (COLS - 40) / 2, " |_|    |_| \\__,_||_|    |_|    |___/   ");
    mvprintw(LINES / 2 - 2, (COLS - 12) / 2, "WITH ITEMS");
}

void draw_title_screen() {
    clear();
    game_over = 0;
    draw_ascii_title();
    mvprintw(LINES / 2 + 2, COLS / 2 - 12, "Press Enter to  Start");
    mvprintw(LINES / 2 + 3, COLS / 2 - 12, "Press 'h'   for Help");
    mvprintw(LINES / 2 + 4, COLS / 2 - 12, "Press 'q'   to  Quit");
    refresh();

    int ch;
    while (1) {
        ch = getch();
        if (ch == '\n') {
            restart_game();
            return;
        }
        if (ch == 'q') {
            game_over = 1;
            return;
        }
        if (ch == 'h') {
            draw_help_screen();
            draw_title_screen();
            return;
        }
        usleep(100000);
    }
}
