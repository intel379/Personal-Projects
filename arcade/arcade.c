#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#define EASY 10
#define EASY_M 10

#define MEDM 18
#define MEDM_M 40

#define HARD 24
#define HARD_M 99

#define PAND 69
#define PAND_M 2121

#define MAX 99

#define EMPTY 0
#define UNSEEN_EMPTY -10
#define FLAGGED_EMPTY 10

#define MINE 9  
#define UNSEEN_MINE -9
#define FLAGGED_MINE 19

void welcome_txt ();

void select_difficulty (int *map_size, int *mine_num);
void print_map(int map[MAX][MAX], int map_size);
void add_mine(int (*map)[MAX][MAX], int map_size, int mine_num);
int reveal(int x, int y, int (*map)[MAX][MAX], int map_size);
int win_check(int correct_flag, int mine_num);
void flagging(int (*map)[MAX][MAX], int x, int y, int *correct_flag);
void final_reveal(int (*map)[MAX][MAX], int map_size);

int blackjack_player(int (*deck)[52], time_t t);
int blackjack_dealer(int (*deck)[52], time_t t);
void print_score(int player_score, int dealer_score);

int main (void) {

    welcome_txt();

    int game_mode = 0;
    scanf("%d", &game_mode);

///////////////////////////////////////////////////////////////////////////////////////

    if (game_mode == 1) {

        printf("Instructions \n <Action> <X> <Y>\n 1 - Sweep\n 2 - Flag\n"); // instructions

        int map_size = -1; // size of map
        int mine_num = -1; // number of mines
    
        // select the difficulty to determine map_size
        select_difficulty (&map_size, &mine_num);

        int map[MAX][MAX] = {0}; // map

        add_mine(&(map), map_size, mine_num);
        print_map(map, map_size);

        int mode = 0;
        int x = 0;
        int y = 0;

        int correct_flag = 0; // number of mines correctly flagged

            while (scanf("%d %d %d", &mode, &x, &y) == 3) {

                if (mode == 1) {
                    // reveal a tile, if its a mine game is lost
                    int i = reveal(y - 1, x - 1, &(map), map_size);
            
                    if (i == 1) {

                        final_reveal(&map, map_size);
                        printf("Oh no you hit a mine :(\n      Game Over\n");
                        return 1;
                    }

                print_map(map, map_size);

                } else if (mode == 2) {
                    // flagging potential mines
                    flagging(&(map), y - 1, x - 1, &correct_flag);
                    print_map(map, map_size);
                }
        
            if (1 == win_check(correct_flag, mine_num)) {
                
                print_map(map, map_size);
                printf("Congradulations you sweeped the entire minefield !!\n");
                return 1;
            }

            
        }
    }
///////////////////////////////////////////////////////////////////////////////////////
    else if (game_mode == 2) {
        printf("Instructions \n 1 = hit\n 2 - stay"); // instructions

        int dealer_score = 0;
        int player_score = 0;

        time_t t; // i got these from stack overflow I have no idea how this works
        srand((unsigned) time(&t));

        while (player_score < 11 && dealer_score < 11) {

            int deck[52] = {0};

            int player_total = blackjack_player(&deck, t); //calculates respective parties total

            int dealer_total = blackjack_dealer(&deck, t);

            printf("\033[0;34m");

            if (player_total > 21) {
                // player busts
                printf("player busts\n");
                dealer_score++;
                print_score(player_score, dealer_score);
            } else if (dealer_total > 21) {
                // dealer busts
                printf("dealer busts\n");
                player_score++;
                print_score(player_score, dealer_score);
            } else if (dealer_total == player_total) {
                // equal score
                printf("It is tied\n");
                print_score(player_score, dealer_score);
            } else if (player_total > dealer_total) {
                // player wins
                printf("Player wins\n");
                player_score++;
                print_score(player_score, dealer_score);
            } else if (dealer_total > player_total) {
                // dealer wins
                printf("Dealer wins\n");
                dealer_score++;
                print_score(player_score, dealer_score);
            }

            printf("----------------------------------\n");
        }

        if (player_score == 11) {
            // player wins
            printf("Congradulations you win!!\n");
            return 0;
        } else {
            // dealer wins

            
            printf("Unlucky the dealer won, good luck next time :(\n");
            return 0;
        }
    }
///////////////////////////////////////////////////////////////////////////////////////
    else if (game_mode == 3) {

        
    }

    scanf("%d");
}

void welcome_txt () {
    // prints the welcome text
    printf(" \\        /\\        /   ||=======  ||         ||=======  /-------\\      /\\        /\\      ||=======\n  \\      /  \\      /    ||         ||         ||         |       |     /  \\      /  \\     ||\n   \\    /    \\    /     ||=======  ||         ||         |       |    /    \\    /    \\    ||=======\n    \\  /      \\  /      ||         ||         ||         |       |   /      \\  /      \\   ||\n     \\/        \\/       ||=======  ||=======  ||=======  \\-------/  /        \\/        \\  ||=======\n Please select what game you would like?\nMinesweeper = 1 Blackjack = 2\n"); // lolololol, good job for scrolling here
}

void select_difficulty (int *map_size, int *mine_num) {

    while (*map_size == -1) {

        char input = getchar();

        if      (input == '1') {*map_size = EASY; *mine_num = EASY_M;}
        else if (input == '2') {*map_size = MEDM; *mine_num = MEDM_M;}
        else if (input == '3') {*map_size = HARD; *mine_num = HARD_M;}
        else if (input == '4') {*map_size = PAND; *mine_num = PAND_M;}
        else if (input == '\n') {printf("What difficulty would you like?\n\n  EASY = 1 MEDIUM = 2 HARD = 3 PANDEMONIUM = 4\n");}
        else    {printf("please chose a number between 1-4 for the difficulty!!\nWhat difficulty would you like?\n\n  EASY = 1 MEDIUM = 2 HARD = 3 PANDEMONIUM = 4\n");}
    }

}

void print_map(int map[MAX][MAX], int map_size) {
    
    for (int i = 0; i < 4; i++) { printf(" "); }
    for (int i = 0; i < map_size; i++) { printf("%d ", (i + 1) / 10);}

    printf("\n");

    for (int i = 0; i < 4; i++) { printf(" "); }
    for (int i = 0; i < map_size; i++) { printf("%d ", (i + 1) % 10);}

    printf("\n");

    for (int i = 0; i < 2 * map_size + 5; i++) { printf("_"); }

    printf("\n");

    for (int i = 0; i < map_size; i++) {

        printf("%02d| ", i + 1);
        for (int j = 0; j < map_size; j++) {
            
            if (map[i][j] == MINE) { // mine

                printf("\033[1;31m"); //Set the text to the color red
                printf("X "); //Print X in red
                printf("\033[0m"); //Resets the text to default color
            } else if (map[i][j] == 1) {

                printf("\033[1;35m"); //Set the text to the color magenta
                printf("%d ",map[i][j]);
                printf("\033[0m"); 
            } else if (map[i][j] == 2) {

                printf("\033[1;34m"); //Set the text to the color blue
                printf("%d ",map[i][j]);
                printf("\033[0m"); 
            } else if (map[i][j] == 3) {

                printf("\033[1;36m"); //Set the text to the color cyan
                printf("%d ",map[i][j]);
                printf("\033[0m"); 
            } else if (map[i][j] == 4) {

                printf("\033[1;32m"); //Set the text to the color green
                printf("%d ",map[i][j]); 
                printf("\033[0m"); 
            } else if (map[i][j] == 5) {

                printf("\033[1;33m"); //Set the text to the color yellow
                printf("%d ",map[i][j]); 
                printf("\033[0m"); 
            } else if (map[i][j] >= 10) {
                
                printf("\033[1;37m"); //Set the text to the color yellow
                printf("F "); 
                printf("\033[0m");
            }else if (map[i][j] >= 6) {

                printf("\033[1;31m"); //Set the text to the color red
                printf("%d ",map[i][j]); 
                printf("\033[0m"); 
            } else if (map[i][j] < 0) { // unseen areas

                printf("+ "); 
            }
            else { printf("%d ",map[i][j]); } // no mine or 0
        }
        printf("|/\n");
    }
}

void add_mine(int (*map)[MAX][MAX], int map_size, int mine_num) {
    // generates the entire map
    time_t t; // i got these from stack overflow I have no idea how this works
    srand((unsigned) time(&t));

    // setting the mines
    
    for(int i = 0 ; i < mine_num ; i++ ) {

        int x = rand() % map_size;
        int y = rand() % map_size;

        if ((*map)[x][y] == MINE) i--;
        (*map)[x][y] = MINE;
    } 

    // change surrounding values
    for (int i = 0; i < map_size; i++) {

        for (int j = 0; j < map_size; j++) {
            
            if ((*map)[i][j] != MINE) {

                int num = 0; // number of surrounding mines

                if (i < (map_size - 1) && (*map)[i + 1][j] == MINE) num++; // right
                if (i < (map_size - 1) && j < (map_size - 1) && (*map)[i + 1][j + 1] == MINE) num++; // top right
                if (j < (map_size - 1) && (*map)[i][j + 1] == MINE) num++; // top
                if (i > 0 && j < (map_size - 1) && (*map)[i - 1][j + 1] == MINE) num++; // top left
                if (i > 0 && (*map)[i - 1][j] == MINE) num++; // left
                if (i > 0 && j > 0 && (*map)[i - 1][j - 1] == MINE) num++; // bottom left
                if (j > 0 && (*map)[i][j - 1] == MINE) num++; // bottom
                if (i < (map_size - 1) && j > 0 && (*map)[i + 1][j - 1] == MINE) num++; // bottom right

                (*map)[i][j] = num;
            }
            
        }
    }

    // make all tiles unviewable
    for (int i = 0; i < map_size; i++) {
        for (int j = 0; j < map_size; j++) {
            
            if ((*map)[i][j] == EMPTY) (*map)[i][j] = UNSEEN_EMPTY;
            else (*map)[i][j] = (*map)[i][j] * -1;       
        }
    }
}

int reveal(int x, int y, int (*map)[MAX][MAX], int map_size) {
    // returns 1 if mine is hit, else it returns 0

    // yeah I wish I could tell you how this works
    if ((*map)[x][y] == UNSEEN_EMPTY) {

        (*map)[x][y] = EMPTY;
        
        if (x < (map_size - 1)) reveal(x + 1, y, (map), map_size); // right
        if (x < (map_size - 1) && y < (map_size - 1)) reveal(x + 1, y + 1, (map), map_size); // top right
        if (y < (map_size - 1)) reveal(x, y + 1, (map), map_size); // top
        if (x > 0 && y < (map_size - 1)) reveal(x - 1, y + 1, (map), map_size); // top left
        if (x > 0) reveal(x - 1, y, (map), map_size); // left
        if (x > 0 && y > 0) reveal(x - 1, y - 1, (map), map_size); // bottom left
        if (y > 0) reveal(x, y - 1, (map), map_size); // bottom
        if (x < (map_size - 1) && y > 0) reveal(x + 1, y - 1, (map), map_size); // bottom right
    }
    else if ((*map)[x][y] == (-1 * MINE)) {

        (*map)[x][y] = MINE;
        return 1;
    } else if ((*map)[x][y] == FLAGGED_EMPTY) printf("You flagged this tile\n");
    else if ((*map)[x][y] < 0) (*map)[x][y] = (*map)[x][y] * -1;
    
    return 0;
}

int win_check(int correct_flag, int mine_num) {
    // return 1 when all mines are flagged
    if (correct_flag == mine_num) return 1;

    return 0;
}

void flagging(int (*map)[MAX][MAX], int x, int y, int *correct_flag) {

    if ((*map)[x][y] >= 0 && (*map)[x][y] < 9) {
        // already revealed
        printf("you can't flag an already revealed tile!\n");
    } else if ((*map)[x][y] == UNSEEN_EMPTY) {
        // unflagged unseen 0
        (*map)[x][y] = FLAGGED_EMPTY;
    } else if ((*map)[x][y] == FLAGGED_EMPTY) {
        // flagged zero
        (*map)[x][y] = UNSEEN_EMPTY;
    } else if ((*map)[x][y] == UNSEEN_MINE) {
        // unseen mine, is correct
        (*correct_flag)++;
        (*map)[x][y] = FLAGGED_MINE;
    } else if ((*map)[x][y] == FLAGGED_MINE) {
        // flagged mine, is incorrect
        (*correct_flag)--;
        (*map)[x][y] = UNSEEN_MINE;
    } else if ((*map)[x][y] > EMPTY) {
        // unrevealed
        (*map)[x][y] = ((*map)[x][y] * -1) + 10;
        (*correct_flag)--;
    } else if ((*map)[x][y] < FLAGGED_EMPTY) {
        // flagged unrevealed
        (*map)[x][y] = ((*map)[x][y] - 10) * -1;
        (*correct_flag)++;
    }
}

void final_reveal(int (*map)[MAX][MAX], int map_size) {

    for (int i = 0; i < map_size; i++) {
        for (int j = 0; j < map_size; j++) {
            
            if ((*map)[i][j] == UNSEEN_EMPTY) (*map)[i][j] = EMPTY;
            else if ((*map)[i][j] < 0)(*map)[i][j] = (*map)[i][j] * -1;       
        }
    }

    for (int i = 0; i < 4; i++) { printf(" "); }
    for (int i = 0; i < map_size; i++) { printf("%d ", (i + 1) / 10);}

    printf("\n");

    for (int i = 0; i < 4; i++) { printf(" "); }
    for (int i = 0; i < map_size; i++) { printf("%d ", (i + 1) % 10);}

    printf("\n");

    for (int i = 0; i < 2 * map_size + 5; i++) { printf("_"); }

    printf("\n");

    for (int i = 0; i < map_size; i++) {

        printf("%02d| ", i + 1);
        for (int j = 0; j < map_size; j++) {
            
            if ((*map)[i][j] == MINE) { // mine

                printf("\033[1;31m"); //Set the text to the color red
                printf("X "); //Print X in red
                printf("\033[0m"); //Resets the text to default color
            } else if ((*map)[i][j] == 1) {

                printf("\033[1;35m"); //Set the text to the color magenta
                printf("%d ",(*map)[i][j]);
                printf("\033[0m"); 
            } else if ((*map)[i][j] == 2) {

                printf("\033[1;34m"); //Set the text to the color blue
                printf("%d ",(*map)[i][j]);
                printf("\033[0m"); 
            } else if ((*map)[i][j] == 3) {

                printf("\033[1;36m"); //Set the text to the color cyan
                printf("%d ",(*map)[i][j]);
                printf("\033[0m"); 
            } else if ((*map)[i][j] == 4) {

                printf("\033[1;32m"); //Set the text to the color green
                printf("%d ",(*map)[i][j]); 
                printf("\033[0m"); 
            } else if ((*map)[i][j] == 5) {

                printf("\033[1;33m"); //Set the text to the color yellow
                printf("%d ",(*map)[i][j]); 
                printf("\033[0m"); 
            } else if ((*map)[i][j] >= 10) {
                
                printf("\033[1;37m"); //Set the text to the color yellow
                printf("F "); 
                printf("\033[0m");
            }else if ((*map)[i][j] >= 6) {

                printf("\033[1;31m"); //Set the text to the color red
                printf("%d ",(*map)[i][j]); 
                printf("\033[0m"); 
            }
            else { printf("%d ",(*map)[i][j]); } // no mine or 0
        }
        printf("|/\n");
    }
}

int blackjack_player(int (*deck)[52], time_t t) {

    int total = 0;
    int hit = 1;

    while (total <= 21) {

        if (hit == 1) {

            int i = rand() % 52;
            while ((*deck)[i] == 1) {

                i = rand() % 52;
            }
            (*deck)[i] = 1;

            if (i >= 36) total = total + 10;
            else total = (i / 4) + total + 1;
            printf("\033[0;32m");
            printf("Your total is %d\n", total);
            printf("\033[0m");

            if (total >= 21) return total;
        } else { return total; }

        scanf("%d", &hit);
    }

    return total;
}

int blackjack_dealer(int (*deck)[52], time_t t) {

    int total = 0;

    while (total <= 21) {

        if (total <= 16) {
            // dealer must hit at 16 or under
            int i = rand() % 52;
            while ((*deck)[i] == 1) {

                i = rand() % 52;
            }
            (*deck)[i] = 1;

            if (i >= 36) total = total + 10;
            else total = (i / 4) + total + 1;

            if (total >= 21) {
                
                printf("\033[1;31m");
                printf("Dealer total is %d\n", total);
                printf("\033[0m");
                return total;
            }
        } else { 
            // dealer must stand at 17 or unde
            printf("\033[1;31m");
            printf("Dealer total is %d\n", total);
            printf("\033[0m");
            return total;
        }
    }

    return total;
}

void print_score(int player_score, int dealer_score) {

    printf("\033[0m");

    printf("player| ");
    printf("\033[1;33m");
    printf("%d\n", player_score);
    printf("\033[0m");

    printf("dealer| ");
    printf("\033[1;33m");
    printf("%d\n", dealer_score);
    printf("\033[0m");
}
