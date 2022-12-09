
/*
 * Online Poker
 * version: 1.1
 * create: 01JAN17
 * by: WG
 *  
 * notes:
 * - this starts a new game of poker.  each time someone wins, a new round begins.
 * - 
 * - 
 * todo:
 * -
 * - 
 * changelog:
 * - add board, player, and dealer classes
 * - 
 * - 
 * 
 */

#ifndef _POKER_HPP_
#define _POKER_HPP_

#include <algorithm>	// remove_player
#include <stdlib.h>		// randon number
#include <time.h>		// random seed
#include <vector>		// vectors
#include <iostream>		// cout
#include <string>		// strings

void clear_the_screen() {
	std::cout << std::string(100, '\n');
}

namespace Poker {

	struct card {
		std::string face;
		std::string suit;
	};

	class Player {
	private:
		std::vector <card> hand;
		unsigned int money;
	public:
		Player(int in_m) {
			clear_hand();
			if (in_m >= 0) money = in_m;
			else money = 15;
		}
		void clear_hand() {
			hand.clear();
		}
		unsigned int return_balance() {
			return money;
		}
		bool give_money(unsigned int in_m) {
			if (money >= in_m) {
				money -= in_m;
				return true;
			} return false;
		}
		void give_blind(unsigned int in_m) {
			if (!give_money(in_m)) {
				money = 0;
			}
		}
		void take_money(unsigned int in_m) {
			money += in_m;
		}
		void take_card(card in_c) {
			hand.push_back(in_c);
		}
		void display_hand() {
			for (std::size_t i=0; i<hand.size(); ++i) {
				std::cout << hand.at(i).face;
				if (i+1<hand.size()) std::cout << ", ";
			} std::cout << std::endl;
			for (std::size_t i=0; i<hand.size(); ++i) {
				std::cout << hand.at(i).suit;
				if (i+1<hand.size()) std::cout << "  ";
				if (hand.at(i).face == "10") std::cout << " ";
			} std::cout << std::endl;
		}
	};

	class Board {
	private:
		std::vector <card> deck;
		std::vector <card> discard;
		std::vector <card> flop;
		//// functions
		void flop_once() {
			flop.push_back(deck.at(0));
			deck.erase(deck.begin());
		}
		void discard_once() {
			discard.push_back(deck.at(0));
			deck.erase(deck.begin());
		}
	public:
		void set_deck(std::vector <card> in_d) {
			deck = in_d;
		}
		card return_card(void) {
			card c = deck.at(0);
			deck.erase(deck.begin());
			return c;
		}
		void add_discard(card in_d) {
			discard.push_back(in_d);
		}
		void cycle(unsigned int in_c) {
			switch (in_c) {
				case 1:
					discard_once();
					for (std::size_t i=0; i<3; ++i) {
						flop_once();
					}
					break;
				case 2:
					discard_once();
					flop_once();
					break;
				case 3:
					discard_once();
					flop_once();
					break;
				default:
					break;
			}
		}
		void display_board() {
			for (std::size_t i=0; i<flop.size(); ++i) {
				std::cout << flop.at(i).face;
				if (i+1 < flop.size()) std::cout << ", ";
			} std::cout << std::endl;
			for (std::size_t i=0; i<flop.size(); ++i) {
				std::cout << flop.at(i).suit << "  ";
				if (flop.at(i).face == "10") std::cout << " ";
			} std::cout << std::endl;
		}
	};

	class Dealer {
	private:
		//// Variables
		Board board;					// the board
		std::vector <Player> players;	// all players
		std::vector <unsigned int> players_remaining;	// location of players remaining - players.at()
		std::vector <std::string> card_face = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
		std::vector <std::string> card_suit = {"s", "c", "h", "d"}; // spades (s), clubs (c), hearts (h), diamonds (d)
		unsigned int player_count;		// count of players plaing
		unsigned int current_player;	// player going first this round
		unsigned int raised_by;			// current raise
		std::vector <int> current_bets;	// each players current bet
		unsigned int current_antie;		// current antie (play cost)
		unsigned int current_round;		// current round
		unsigned int blind_big;			// value of big blind
		unsigned int blind_small;		// value of small blind
		//// Functions
		// at start of game, create all players
		void setup_players(unsigned int in_pc) {
			for (std::size_t i=0; i<in_pc; i++) {
				Player p;
				players.push_back(p);
			}
		}
		// at start of round, create shuffled deck and return it
		std::vector <card> create_deck(void) {
			std::vector <card> d;
			// create deck
			for (std::size_t i=0; i<card_face.size(); ++i) {
				for (std::size_t j=0; j<card_suit.size(); ++j) {
					card c;
					c.face = card_face.at(i);
					c.suit = card_suit.at(j);
					d.push_back(c);
				}
			}
			// shuffle deck
			std::vector <card> d_sh;
			while(d.size() > 0) {
				unsigned int c = rand() % d.size();
				d_sh.push_back(d.at(c));
				d.erase(d.begin()+c);
			}
			return d_sh;
		}
		// at start of round, deal 2 cards to all players from the boards deck,
		//	after clearing their hand
		void cards_to_players(void) {
			// deal first card
			for (std::size_t i = 0; i<players.size(); ++i) {
				unsigned int c = current_player + i;
				if (c > player_count) c -= player_count;
				players.at(c).clear_hand();
				players.at(c).take_card(board.return_card());
			}
			// deal second card
			for (std::size_t i = 0; i<players.size(); ++i) {
				unsigned int c = current_player + i;
				if (c > player_count) c -= player_count;
				players.at(c).take_card(board.return_card());
			}
		}
		// display useful information to the player
		void display_bet_menu(unsigned int in_p) {
			std::cout << "player " << in_p << " bet round" << std::endl;
			std::cout << "board:" << std::endl;
			board.display_board();
			std::cout << "hand: " << std::endl;
			players.at(in_p).display_hand();
			if (raised_by > 0)
				std::cout << "minimum bet: " << raised_by << std::endl;
			std::cout << "all bets: ";
			for (std::size_t i=0; i<current_bets.size(); ++i) {
				std::cout << current_bets.at(i);
				if (i+1<current_bets.size()) std::cout << ", ";
			} std::cout << std::endl;
		}
		// menu to aid in transfering the device to another player
		void transfer_device(void) {
			std::cout << "when done, type k and press enter" << std::endl;
			std::cin >> t; // clear screen
			clear_the_screen();
			std::cout << "pass the game to the next player." << std::endl;
			std::cout << "next player, type k and press enter to continue" << std::endl;
			std::cin >> t; // pass device
			clear_the_screen();
		}
		// determine current player with iterator
		unsigned int determine_position(unsigned int in_p, unsigned int in_i) {
			unsigned int c = in_p + in_i;
			if (c > player_count) c -= player_count;
			return c;
		}
		// set players 'current_bet'
		void set_players_bet(unsigned int in_p, unsigned int in_a) {
			if (current_bets.at(in_p) != -1) current_bets.at(in_p) = current_bets.at(in_p) + in_a;
			else current_bets.at(in_p) = in_a;
		}
		// apply big and little blind
		void apply_blind(unsigned int in_p) {
			unsigned int b;
			b = determine_position(in_p, 1);
			players.at(b).give_blind(blind_big);
			b = determine_position(in_p, 2);
			players.at(b).give_blind(blind_small);
		}
		// cycle all places for bet once
		unsigned int all_bet(unsigned int lptr) {
			for (std::size_t i = 0; i < players.size(); ++i) {
				if (take_bet(determine_position(current_player, i))) {
					// new raise
					lptr = determine_position(current_player, i);
				}
			}
			return lptr;
		}
		std::vector <unsigned int> bet_next(unsigned int lptp, unsigned int lptr) {
			std::vector <unsigned int> t;
			if (take_bet(determine_position(current_player, lptp))) {
				// new raise
				lptr = determine_position(current_player, lptp);
			}
			t.push_back(lptr);
			t.push_back(lptp);
			return t;
		}
		/* 
			unsigned int get_bet ( unsigned int )
			
			input: current player (uint)
			output: bet ammount (uint) (0 to wallet)

			info: menu to enter bet ammount
		*/
		unsigned int get_bet(unsigned int in_p) {
			unsigned int a = 0;
			while (true) {
				bool redo = false;
				std::cout << "how much would you like to bet? : ";
				std::cin >> a;
				if (a >= 0) {
					// handle player payment, if pass
					if (players.at(in_p).give_money(a)) {
						set_players_bet(in_p, a);
						std::cout << "\nyou placed a bet of : " << a << std::endl;
					} else {
						std::cout << "you can not afford this bet" << std::endl;
						redo = true;
					}
				}
				else if (a == 0) std::cout << "no bet placed" << std::endl;
				else {
					std::cout << "error with input" << std::endl;
					redo = true;
				}
				if (!redo) break;
			}
			return a;
		}
		/*
			bool take_bet( unsigned int )

			input: current player position (uint)
			output: raise updated (bool)

			info: handle a single players bet.
				- display bet menu
				- get bet ammount (optional)
				- remove if fail previous raise (action: fold)
				- transfer to next player

		*/
		bool take_bet(unsigned int in_p) {
			bool r = false;
			// for single player while they decide bet
			while(true) {
				bool redo = false;
				display_bet_menu(in_p);
				std::cout << "you have " << players.at(in_p).return_balance() << " credits" << std::endl;
				std::cout << "would you like to bet? (y/n): ";
				// take bet
				char t;
				std::cin >> t;
				int a = 0;
				// if betting
				if (t == 'y' || t == 'Y') {
					// get bet and check with raise
					if (get_bet(in_p) > raised_by) {
						r = true;  // raise updated, new highest
						raised_by = a;
					}
				// if not bettings
				} else if (t == 'n' || t == 'N') {
					std::cout << "no bet placed" << std::endl;
				// error with input
				} else {
					std::cout << "error with input" << std::endl;
					redo = true;
				}
				if (!redo) break;
			}
			// if previous raise
			if (raised_by > 0) {  // handle if previous raise
				if (a < raised_by) {  // bet less than raise, remove from round
					std::cout << "you have folded and are out of this round" << std::endl;
					remove_player(in_p);
				}
			}
			// exit
			transfer_device();
			return r;
		}
		/*
			void bet_cycle( bool )

			input: if new round
			output: NA

			info: manages the main game loop
				- antie and blind
				- all betting
				- returns after round is complete
		
		*/
		void bet_cycle(bool in_n) {
			if (in_n) {
				// reset bets
				if (current_bets.size() == 0) {
					for (std::size_t i=0; i<players.size(); ++i) {
						current_bets.push_back(-1);
					}
				}
				// apply antie
				raised_by = current_antie;
			} else raised_by = 0;
			// apply blind
			apply_blind(current_player);

			// #################################

			// each player take bets until no more raises
			while (true) {
				unsigned int lptp = current_player; // last player to play
				unsigned int lptr = current_player; // last player to raise
				bool first_round = true;
				// first round
				if (first_round) {
					lptr = all_bet(lptr);
					first_round = false;
				}
				// play until everyone has agreed to last raise
				if (lptr != lptp) {
					std::vector <unsigned int> t = bet_next(lptp, lptr);
					lptr = t.at(0);
					lptp = t.at(1);
				}
			}
			// #################################
		}
		// update the current_player variable
		void update_current_player() {
			if (current_player == player_count) current_player = 0;
			else current_player += 1;
		}
		// each round, everyone is back in
		void reset_players_remaining() {
			players_remaining.clear();
			for (std::size_t i = 0; i<players.size(); ++i) {
				players_remaining.push_back(i);
			}
		}
		// after someone folds, remove them
		void remove_player(unsigned int in_p) {
			players_remaining.erase(std::remove(players_remaining.begin(), players_remaining.end(), in_p), players_remaining.end());
		}
		// if there is only one player left, they win
		bool check_winner() {
			if (players_remaining.size() == 1) {
				unsigned int v;
				// give pot to winner
				for (std::size_t i=0; i<current_bets.size(); ++i) {
					v += current_bets.at(i);
				}
				players.at(players_remaining.at(0)).take_money(v);
				std::cout << "payout: " << v << std::endl;
				reset_players_remaining();
			}
			else return false;
		}
		// display exit info
		void display_exit() {
			clear_the_screen();
			std::cout << "\nEnd of game\n" << std::endl;
			std::cout << "player scores:" << std::endl;
			for (std::size_t i=0; i<players.size(); ++i) {
				std::cout << " player: " << i;
				std::cout << "; score: " << players.at(i).return_balance() << std::endl;
			} std::cout << std::endl;
		}
		// menu to enter desired player count
		void get_player_count() {
			while (true) {
				std::cout << "how many players? (1-8): " << std::endl;
				unsigned int p = 0;
				std::cin >> p;
				if (p > 0 && p < 9) {
					in_pc = p;
					break;
				}
				else std::cout << "error with player count" << std::endl;
			} clear_the_screen();
		}
		// cleanup for new round
		void new_round() {
			current_round++;
			board.set_deck(create_deck());
			cards_to_players();
			current_bets.clear();
			update_current_player();
		}
	public:	
		// settings
		Dealer() {
			unsigned int max_round = 2;	// max round
			bool infinate = false;		// infinate rounds
			current_antie = 1;			// cost to play
		}
		// main game loop
		void play(int in_pc) {
			current_round = 0;
			current_player = 0;
			srand(time(NULL));
			clear_the_screen();
			std::cout << "Welcome to MP Poker\n" << std::endl;
			if (in_pc == -1) get_player_count();
			player_count = in_pc;
			setup_players(player_count);
			bool game_on = true;
			while(game_on) {
				// setup
				new_round();
				// 1 - flop
				board.cycle(1);
				bet_cycle(true);
				if (check_winner())
				// 2 - 
				board.cycle(2);
				bet_cycle(false);
				if (check_winner())
				// 3 - river?
				board.cycle(3);
				bet_cycle(false);
				if (check_winner())
				// exit
				if (current_round >= max_round && !infinate) game_on = false;
			}
			display_exit();
		}
	};

};

#endif