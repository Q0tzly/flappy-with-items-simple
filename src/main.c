#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include "game.h"
#include "render.h"
#include "config.h"

int main() {
    srand(time(0));

    init_game();

    while (1) {
        draw_title_screen();
        if (game_over) {
            break;
        }
        
        while (!game_over) {
            update_game();
            draw_game();
            usleep(DELAY);
        }
        
        draw_game_over_screen();
        if (game_over) {
            continue;
        }
    }

    cleanup_game();
    return 0;
}
