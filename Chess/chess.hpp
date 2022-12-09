
/*

Chess Game

William Gregory (c) 2075 - all rights reserved

piece types:
    ID  name    points
    ------------------
    0   NULL     -
    1   king    (0)
    2   queen   (9)
    3   rook    (5)
    4   bishop  (3)
    5   knight  (3)
    6   pawn    (1)

player types:
    true: white
    false: black

board positions:
    1a, 1b, ...
    2a, 2b, ...
    ...
    ..., 8g, 8h

    black starts on side 1 and 2
    white starts on side 7 and 8

todo: **
- legal move check
- apply move
- end condition check

*/

#include <vector>
#include <iostream>

namespace Chess {

    class Piece {
    private:
        unsigned int type;
        bool player; // owner
        unsigned int score;  // worth
        bool moved;  // been moved?
    public:
        void setup(unsigned int in, bool in_o) {
            moved = false;
            type = in;
            player = in_o;
            // set score
            switch(in) {
                case 2:
                    score = 9;
                    break;
                case 3:
                    score = 5;
                    break;
                case 4:
                case 5:
                    score = 3;
                    break;
                case 6:
                    score = 1;
                    break;
                default:
                    score = 0;
            }
        }
        unsigned int return_score() {
            return score;
        }
        bool return_player() {
            return player;
        }
        unsigned int return_type() {
            return type;
        }
        char return_type_char() {
            char t = 'E';
            if (player) {
                switch(type) {
                    case 0:
                        t = ' ';
                        break;
                    case 1:
                        t = 'K';
                        break;
                    case 2:
                        t = 'Q';
                        break;
                    case 3:
                        t = 'R';
                        break;
                    case 4:
                        t = 'B';
                        break;
                    case 5:
                        t = 'N';
                        break;
                    case 6:
                        t = 'P';
                        break;
                }
            } else {
                switch(type) {
                    case 0:
                        t = ' ';
                        break;
                    case 1:
                        t = 'k';
                        break;
                    case 2:
                        t = 'q';
                        break;
                    case 3:
                        t = 'r';
                        break;
                    case 4:
                        t = 'b';
                        break;
                    case 5:
                        t = 'n';
                        break;
                    case 6:
                        t = 'p';
                        break;
                }
            }
            return t;
        }
    };

    class Player {
    private:
        bool type;
        unsigned int score;
        std::vector <Piece> captured;
    public:
        void setup(bool in) {
            type = in;
            score = 0;
            captured.clear();
        }
        unsigned int return_score() {
            return score;
        }
        void add_cap(Piece p) {
            captured.push_back(p);
            score += p.return_score();
        }
    };

    class Board {
    private:
        std::vector <std::vector <Piece> > board;
        std::vector <Player> players;
        bool player_current;
        bool game_on;
        ///
        std::vector <Player> new_players() {
            std::vector <Player> p;
            Player w;
            w.setup(true);
            Player b;
            b.setup(false);
            p.push_back(w);
            p.push_back(b);
            return p;
        }
        std::vector <std::vector <Piece> > new_board() {
            std::vector <std::vector <Piece> > b;
            // black
            // 1
            std::vector <Piece> b1;
            Piece p1a;
            p1a.setup(3, false);
            b1.push_back(p1a);
            Piece p1b;
            p1b.setup(5, false);
            b1.push_back(p1b);
            Piece p1c;
            p1c.setup(4, false);
            b1.push_back(p1c);
            Piece p1d;
            p1d.setup(1, false);
            b1.push_back(p1d);
            Piece p1e;
            p1e.setup(2, false);
            b1.push_back(p1e);
            Piece p1f;
            p1f.setup(4, false);
            b1.push_back(p1f);
            Piece p1g;
            p1g.setup(5, false);
            b1.push_back(p1g);
            Piece p1h;
            p1h.setup(3, false);
            b1.push_back(p1h);
            b.push_back(b1);
            // 2
            std::vector <Piece> b2;
            Piece p2a;
            p2a.setup(6, false);
            b2.push_back(p2a);
            Piece p2b;
            p2b.setup(6, false);
            b2.push_back(p2b);
            Piece p2c;
            p2c.setup(6, false);
            b2.push_back(p2c);
            Piece p2d;
            p2d.setup(6, false);
            b2.push_back(p2d);
            Piece p2e;
            p2e.setup(6, false);
            b2.push_back(p2e);
            Piece p2f;
            p2f.setup(6, false);
            b2.push_back(p2f);
            Piece p2g;
            p2g.setup(6, false);
            b2.push_back(p2g);
            Piece p2h;
            p2h.setup(6, false);
            b2.push_back(p2h);
            b.push_back(b2);
            // empty (3, 4, 5, 6)
            for (std::size_t i=0; i<4; ++i) {
                std::vector <Piece> be;
                // (a, b, c, d, e, f, g, h)
                for (std::size_t j=0; j<8; ++j) {
                    Piece pn;
                    pn.setup(0, false);  // type 0 (null) so owner does not matter
                    be.push_back(pn);
                }
                b.push_back(be);
            }
            // white
            // 7
            std::vector <Piece> b7;
            Piece p7a;
            p7a.setup(6, true);
            b7.push_back(p7a);
            Piece p7b;
            p7b.setup(6, true);
            b7.push_back(p7b);
            Piece p7c;
            p7c.setup(6, true);
            b7.push_back(p7c);
            Piece p7d;
            p7d.setup(6, true);
            b7.push_back(p7d);
            Piece p7e;
            p7e.setup(6, true);
            b7.push_back(p7e);
            Piece p7f;
            p7f.setup(6, true);
            b7.push_back(p7f);
            Piece p7g;
            p7g.setup(6, true);
            b7.push_back(p7g);
            Piece p7h;
            p7h.setup(6, true);
            b7.push_back(p7h);
            b.push_back(b7);
            // 8
            std::vector <Piece> b8;
            Piece p8a;
            p8a.setup(3, true);
            b8.push_back(p8a);
            Piece p8b;
            p8b.setup(5, true);
            b8.push_back(p8b);
            Piece p8c;
            p8c.setup(4, true);
            b8.push_back(p8c);
            Piece p8d;
            p8d.setup(1, true);
            b8.push_back(p8d);
            Piece p8e;
            p8e.setup(2, true);
            b8.push_back(p8e);
            Piece p8f;
            p8f.setup(4, true);
            b8.push_back(p8f);
            Piece p8g;
            p8g.setup(5, true);
            b8.push_back(p8g);
            Piece p8h;
            p8h.setup(3, true);
            b8.push_back(p8h);
            b.push_back(b8);
            return b;
        }
        unsigned int char_to_int(char in) {
            unsigned int t = 0;
            switch (in) {
                case 'a':
                    t = 1;
                    break;
                case 'b':
                    t = 2;
                    break;
                case 'c':
                    t = 3;
                    break;
                case 'd':
                    t = 4;
                    break;
                case 'e':
                    t = 5;
                    break;
                case 'f':
                    t = 6;
                    break;
                case 'g':
                    t = 7;
                    break;
                case 'h':
                    t = 8;
                    break;
            }
            return t;
        }
        bool check_obstacle(unsigned int in_r, char in_cc, unsigned int in_r2, char in_c2c) {
            // path clear?
        }        
        bool check_move(unsigned int in_r, unsigned int in_c) {
            // pawn at far end?
            // piece captured?
        }
        bool move_piece(unsigned int in_r, char in_cc, unsigned int in_r2, char in_c2c) {
            // with all exept knight: check for obstacales!!!
            unsigned int in_c = char_to_int(in_cc);
            unsigned int in_c2 = char_to_int(in_c2c);
            if (in_c == 0 || in_c2 == 0) return false;
            if (in_r > 8 || in_r < 1 || in_r2 > 8 || in_r2 < 1) return false;
            std::vector <unsigned int> legal_r;  // when paired with legal_c at same index
            std::vector <unsigned int> legal_c;
            unsigned int p = board.at(in_r).at(in_c).return_type();
            if (p == 1) {  // king
                // check for castle
                if (in_r + 1 < 9) {
                    legal_r.push_back(in_r+1);
                    legal_c.push_back(in_c);
                }
                if (in_c + 1 < 9) {
                    legal_r.push_back(in_r);
                    legal_c.push_back(in_c+1);
                }
                if (in_r - 1 > 0) {
                    legal_r.push_back(in_r-1);
                    legal_c.push_back(in_c);
                }
                if (in_c - 1 > 0) {
                    legal_r.push_back(in_r);
                    legal_c.push_back(in_c-1);
                }
            } else if (p==2) {  // queen
                // root moves
                for (std::size_t i=1; i<8-in_r) {
                    legal_r.push_back(in_r+i);
                    legal_c.push_back(in_c);
                }
                for (std::size_t i=1; i<in_r) {
                    legal_r.push_back(in_r-i);
                    legal_c.push_back(in_c);
                }
                for (std::size_t i=1; i<8-in_c) {
                    legal_r.push_back(in_r);
                    legal_c.push_back(in_c+i);
                }
                for (std::size_t i=1; i<in_c) {
                    legal_r.push_back(in_r);
                    legal_c.push_back(in_c-i);
                }
                // bishop moves
                for (std::size_t i=1; i<8) {
                    if (in_r+i<9 && in_c+i<9) {
                        legal_r.push_back(in_r+i);
                        legal_c.push_back(in_c+i);
                    }
                    if (in_r-i>0 && in_c-i>0) {
                        legal_r.push_back(in_r-i);
                        legal_c.push_back(in_c-i);
                    }
                    if (in_r-i>0 && in_c+i<9) {
                        legal_r.push_back(in_r-i);
                        legal_c.push_back(in_c+i);
                    }
                    if (in_r+i<9 && in_c-i>0) {
                        legal_r.push_back(in_r+i);
                        legal_c.push_back(in_c-i);
                    }
                }
            } else if (p==3) {  // rook
                for (std::size_t i=1; i<8-in_r) {
                    legal_r.push_back(in_r+i);
                    legal_c.push_back(in_c);
                }
                for (std::size_t i=1; i<in_r) {
                    legal_r.push_back(in_r-i);
                    legal_c.push_back(in_c);
                }
                for (std::size_t i=1; i<8-in_c) {
                    legal_r.push_back(in_r);
                    legal_c.push_back(in_c+i);
                }
                for (std::size_t i=1; i<in_c) {
                    legal_r.push_back(in_r);
                    legal_c.push_back(in_c-i);
                }
            } else if (p==4) {  // bishop
                for (std::size_t i=1; i<8) {
                    if (in_r+i<9 && in_c+i<9) {
                        legal_r.push_back(in_r+i);
                        legal_c.push_back(in_c+i);
                    }
                    if (in_r-i>0 && in_c-i>0) {
                        legal_r.push_back(in_r-i);
                        legal_c.push_back(in_c-i);
                    }
                    if (in_r-i>0 && in_c+i<9) {
                        legal_r.push_back(in_r-i);
                        legal_c.push_back(in_c+i);
                    }
                    if (in_r+i<9 && in_c-i>0) {
                        legal_r.push_back(in_r+i);
                        legal_c.push_back(in_c-i);
                    }
                }
            } else if (p==5) {  // knight

            } else if (p==6) {  // pawn
                // check for en passant
                // check if piece avilable for capture
                if (in_r+1 < 9) {
                    legal_r.push_back(in_r+1);
                    legal_c.push_back(in_c);
                } 
                if (in_r+2 < 9) {
                    legal_r.push_back(in_r+2);
                    legal_c.push_back(in_c);
                }
            } else return false;
            // check move
            for (std::size_t i=0; i<legal_r.size(); ++i) {
                if (in_r2 == legal_r.at(i)) {
                    if (in_c2 == legal_c.at(i)) return true;
                }
            }
            return false;
        }
        void display_board_white() {
            std::cout << "\nChess board" << std::endl;
            std::cout << "info: white is capital" << std::endl;
            std::cout << "W score: " << players.at(0).return_score() << std::endl;
            std::cout << "B score: " << players.at(1).return_score() << std::endl;
            if (player_current) {
                std::cout << "current player: white" << std::endl;
            } else {
                std::cout << "current player: black" << std::endl;
            }
            std::cout << "--------------------\n" << std::endl;
            for (std::size_t i=0; i<board.size(); ++i) {
                switch(i) {
                    case 0:
                        std::cout << "1| ";
                        break;
                    case 1:
                        std::cout << "2| ";
                        break;
                    case 2:
                        std::cout << "3| ";
                        break;
                    case 3:
                        std::cout << "4| ";
                        break;
                    case 4:
                        std::cout << "5| ";
                        break;
                    case 5:
                        std::cout << "6| ";
                        break;
                    case 6:
                        std::cout << "7| ";
                        break;
                    case 7:
                        std::cout << "8| ";
                        break;
                }
                for (std::size_t j=0; j<board.at(i).size(); ++j) {
                    std::cout << board.at(i).at(j).return_type_char() << " ";
                } std::cout << std::endl;
            }
            std::cout << "   _______________" << std::endl;
            std::cout << "   h g f e d c b a" << std::endl;
            std::cout << std::endl;
        }
        void display_board_black() {
            std::cout << "\nChess board" << std::endl;
            std::cout << "info: white is capital" << std::endl;
            std::cout << "W score: " << players.at(0).return_score() << std::endl;
            std::cout << "B score: " << players.at(1).return_score() << std::endl;
            if (player_current) {
                std::cout << "current player: white" << std::endl;
            } else {
                std::cout << "current player: black" << std::endl;
            }
            std::cout << "--------------------\n" << std::endl;
            for (std::size_t i=0; i<board.size(); ++i) {
                switch(i) {
                    case 0:
                        std::cout << "8| ";
                        break;
                    case 1:
                        std::cout << "7| ";
                        break;
                    case 2:
                        std::cout << "6| ";
                        break;
                    case 3:
                        std::cout << "5| ";
                        break;
                    case 4:
                        std::cout << "4| ";
                        break;
                    case 5:
                        std::cout << "3| ";
                        break;
                    case 6:
                        std::cout << "2| ";
                        break;
                    case 7:
                        std::cout << "1| ";
                        break;
                }
                for (std::size_t j=0; j<board.at(i).size(); ++j) {
                    std::cout << board.at(board.size()-i-1).at(board.at(i).size()-j-1).return_type_char() << " ";
                } std::cout << std::endl;
            }
            std::cout << "   _______________" << std::endl;
            std::cout << "   a b c d e f g h" << std::endl;
            std::cout << std::endl;
        }
    public:
        Board() {
            // setup players
            players = new_players();
            player_current = true;
            // setup board
            board = new_board();
            game_on = true;

            // TEST
            display_board_white();
            display_board_black();
        }
        void play_game() {
            while (game_on) {
                // white
                player_current = true;

                // black
                player_current = false;
            }
        }
    };

}
