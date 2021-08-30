/*
*   Checkers
*    a two player game
*
*   change log:
*   - 
*   -
*
*/

#ifndef _CHECKERS_HPP_
#define _CHECKERS_HPP_

#define DEBUG

//#include <stdlib.h>
//#include <time.h>
#include <iostream>
#include <vector>

namespace Checkers {

    void clear_screen() {
        std::cout << std::string(100, '\n');
    }

    struct piece {
        bool type;
    };

    class Board {
    public:
        void display_board() {

        }
    
    };

    class Player {
    private:
    public:
        Player() {
        }
    };

    class Game {
    private:
        Board board;
        Player player1;
        Player player2;
        void reset() {
            board = Board();
            player1 = Player();
            player2 = Player();
        }
        void player_life() {
            // view board
            // action
        }
    public:
        Game() {
        }
        void run() {
            reset();
            bool game_on = true;
            std::cout << "hello";
            while(game_on) {
                // player 1
                // player 2

                // detect legal moves
                // detect win/loss
                // input can end game

                return; // temp exit
            }
        }
    };
};

#endif