
// TicTacToe Game
// Copyright (c) 2016 William G.  All rights reserved.

#ifndef _TIC_TAC_TOE_
#define _TIC_TAC_TOE_

#include <iostream>
#include <vector>

//#define TTT_DEBUG

namespace TicTacToe {

    // 0: empty, 1: player 1, 2: player 2
    class Board {
    private:
        std::vector <std::vector <unsigned int> > board;
    public:
        Board() {
            for (std::size_t i=0; i<3; ++i) {
                std::vector <unsigned int> t;
                for (std::size_t j=0; j<3; ++j) {  // setup board
                    t.push_back(0);
                }
                board.push_back(t);
            }
        }
        bool move(unsigned int in_player, unsigned int in_x, unsigned int in_y) {
#ifdef TTT_DEBUG
                std::cout << "DEBUG: Board::move()" << std::endl;
#endif
            if (in_x > 2 || in_y > 2) {  //error
                std::cout << "\n" << "NOTE: invalid move: space is not on board" << std::endl;
            }
            else if (in_player == 1 || in_player == 2) {
                if (board.at(in_x).at(in_y) == 0) {
                    board.at(in_x).at(in_y) = in_player;  // apply move
                    return true;
                } else {  // player error
                    std::cout << "\n" << "NOTE: invalid move: space is already taken" << std::endl;
                }
            } else {  // error
#ifdef TTT_DEBUG
                std::cout << "ERROR: player not valid" << std::endl;
#endif
            }
            return false;
        }
        unsigned int check_win() {
#ifdef TTT_DEBUG
                std::cout << "DEBUG: Board::check_win()" << std::endl;
#endif
            // check for all win conditions
            // 8 posible positions
            // horizontal
            for (std::size_t i=0; i<3; ++i) {  // row
                if (board.at(0).at(i) != 0) {
                    if (board.at(0).at(i) == board.at(1).at(i)) {
                        if (board.at(1).at(i) == board.at(2).at(i)) {
                            return board.at(0).at(i); // winnder
                        }
                    }
                }
            }
            // vertical
            for (std::size_t i=0; i<3; ++i) {  // row
                if (board.at(i).at(0) != 0) {
                    if (board.at(i).at(0) == board.at(i).at(1)) {
                        if (board.at(i).at(1) == board.at(i).at(2)) {
                            return board.at(i).at(0); // winner
                        }
                    }
                }
            }
            // diagonal
            // left to right
            if (board.at(0).at(0) != 0) {
                if (board.at(0).at(0) == board.at(1).at(1)) {
                    if (board.at(1).at(1) == board.at(2).at(2)) {
                        return board.at(0).at(0); // winner
                    }
                }
            }
            // right to left
            if (board.at(2).at(0) != 0) {
                if (board.at(2).at(0) == board.at(1).at(1)) {
                    if (board.at(1).at(1) == board.at(0).at(2)) {
                        return board.at(2).at(0); // winner
                    }
                }
            }
            return 0;  // no winner
        }
        void display_board() {
#ifdef TTT_DEBUG
                std::cout << "DEBUG: Board::display_board()" << std::endl;
#endif
            std::cout << std::endl;
            for (std::size_t i=0; i<3; ++i) {
                for (std::size_t j=0; j<3; ++j) {
                    std::cout << board.at(j).at(i) << " ";
                }
                std::cout << std::endl;
            }
        }
    };

    class Player {
    private:
        unsigned int score;
    public:
        Player() {
            score = 0;
        }
        // needs work
        std::vector <unsigned int> move_choice() {
#ifdef TTT_DEBUG
                std::cout << "DEBUG: Player::move_choice()" << std::endl;
#endif
            std::vector <unsigned int> t_move;
            std::cout << "Enter X position (0 to 2) : ";
            unsigned int t_x;
            std::cin >> t_x;
            std::cout << "Enter Y position (0 to 2) : ";
            unsigned int t_y;
            std::cin >> t_y;
            t_move.push_back(t_x);
            t_move.push_back(t_y);
            return t_move;
        }
    };

    class Game {
    private:
        /// Variables
        std::vector <Player> players;
        Board b;
        /// Functions
        void setup_players() {
#ifdef TTT_DEBUG
                std::cout << "DEBUG: Game::setup_players()" << std::endl;
#endif
            players.clear();
            for (std::size_t i=0; i<2; ++i) {
                Player p;
                players.push_back(p);
            }
        }
        void setup_board() {
#ifdef TTT_DEBUG
                std::cout << "DEBUG: Game::setup_board()" << std::endl;
#endif
            Board board;
            b = board;
        }
    public:
        Game() {
            setup_players();
            setup_board();
        }
        void play() {
#ifdef TTT_DEBUG
                std::cout << "DEBUG: Game::play()" << std::endl;
#endif
            std::cout << std::endl;
            std::cout << "Tic Tac Toe" << std::endl;
            b.display_board();
            unsigned t_win;
            bool game_on = true;
            while (game_on) {
#ifdef TTT_DEBUG
                std::cout << "DEBUG: Game::play() - new round" << std::endl;
#endif
                for (std::size_t i=0; i<2; ++i) {
                    while (true) {  // each player move
                        std::cout << "\n" << "Player " << i+1 << std::endl;
                        std::vector <unsigned int> t_move = players.at(i).move_choice();
                        if (b.move(i+1, t_move.at(0), t_move.at(1))) break;  // move accepted
                    }
                    b.display_board();
                    t_win = b.check_win();
                    if (t_win != 0) {
                        game_on = false;
                        break;
                    }
                }
            }
        std::cout << "\nWinner: player " << t_win << "\n" << std::endl;
        }
    };

}

#endif
