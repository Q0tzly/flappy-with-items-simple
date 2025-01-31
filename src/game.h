#ifndef GAME_H
#define GAME_H

extern int score;
extern int coin_score;
extern int game_over;
extern int star_mode;
extern int star_timer;
extern int pipe_count;
extern int item_count;

void init_game();
void update_game();
void restart_game();
void cleanup_game();

#endif
