
 /*
 * Poker
 * version: 1.2
 * start: 01JAN17
 * last: 05DEC18
 * by: WG
 *  
 * notes:
 * - this starts a new game of poker.  each time someone wins, a new round begins.
 * - 
 * - 
 *
 * todo:
 * - add networking
 * - test functions
 * - sort out game logic
 * - track banks money and buy-ins
 * - add timer to round and optional limit
 *
 * change log:
 * - add board, player, and dealer classes
 * - run through player and board classes
 * - 
 * 
 */

#ifndef _POKER_HPP_
#define _POKER_HPP_

#define POKER_DEBUG		// uncomment to enable debug
#define POKER_VERBOSE	// uncomment to enable verbose info
//#define CLEAR_SCREEN	// uncomment to enable clear screen

#include <algorithm>	// remove_player
#include <stdlib.h>		// random number
#include <time.h>		// random seed
#include <vector>		// vectors
#include <iostream>		// console print
#include <string>		// strings

namespace CardGame {
	
	void clear_the_screen() {
#ifdef CLEAR_SCREEN
		std::cout << std::string(100, '\n');
#endif
	}

	struct card {
		std::string face;
		std::string suit;
	};
	
	void display_deck(std::vector <card> in) {
		for (std::size_t i=0; i<in.size(); ++i) {
			std::cout << i << "\t";
		} std::cout << std::endl;
		for (std::size_t i=0; i<in.size(); ++i) {
			std::cout << in.at(i).face << "\t";
		} std::cout << std::endl;
		for (std::size_t i=0; i<in.size(); ++i) {
			std::cout << in.at(i).suit << "\t";
		} std::cout << std::endl;
	}

	class Board {
	private:
		std::vector <card> discard;
		std::vector <card> hand;
	public:
		void add_discard(card in) {
			discard.push_back(in);
		}
		void clear_discard() {
			discard.clear();
		}
		void add_hand(card in) {
			hand.push_back(in);
		}
		void clear_hand() {
			hand.clear();
		}
		void display_board() {
			// display discard
			std::cout << "Board discard" << std::endl;
			display_deck(discard);
			// display hand
			std::cout << "Board hand" << std::endl;
			display_hand(hand);
		}
		void clear_board() {
			clear_discard();
			clear_hand();
		}
	};

	class DeckAssistant {
	private:
		std::vector <std::string> card_face = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
					// 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, J, Q, K, A
		std::vector <std::string> card_suit = {"s", "c", "h", "d"};
					// spades(s), clubs(c), hearts(h), diamonds(d)
		// check
		std::vector <card> cut_deck(std::vector <card> in) {
			std::vector <card> t;
			unsigned int s = in.size();
			for (std:size_t i=s/2; i<s; ++i) {
				t.push_back(in.at(i));
			}
			for (std:size_t i=0; i<s/2; ++i) {
				t.push_back(in.at(i));
			}
			return t;
		}
	public:
		// shuffle deck
		std::vector <card> shuffle_deck(std::vector <card> in) {
			std::vector <card> t;
			while(in.size() > 0) {
				unsigned int c = rand() % in.size();
				t.push_back(in.at(c));
				in.erase(in.begin()+c);
			} return t;
		}
		// create shuffled deck and return it
		std::vector <card> create_deck(void) {
#ifdef POKER_DEBUG
	std::cout << "DEBUG: DeckCreator::create_deck()" << std::endl;
#endif
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
			// shuffle new deck
			d = shuffle_deck(d);
			d = shuffle_deck(d);
			d = cut_deck(d);
			d = shuffle_deck(d);
			return d;
		}
	};
	
	class Player {
	private:
		unsigned int player_id;
		std::string player_name;
		std::vector <card> hand;
		std::vector <card> discard;
		unsigned int credits;
	public:
		Player(unsigned int in_m, unsigned int in_id, std::string in_n) {
			player_id = in_id;
			player_name = in_n;
			hand.clear();
			discard.clear();
			if (in_m > 0) credits = in_m;
			else credits = 250;
		}
		unsigned int return_balance(void) { return credits; }
		void take_money(unsigned int in_m) { credits += in_m; }
		bool give_money(unsigned int in_m) {
			if (credits >= in_m) {
				credits -= in_m;
				return true;
			} credits = 0; return false;
		}
		void clear_hand(void) { hand.clear(); }
		void clear_discard(void) { discard.clear(); }
		void clear_cards(void) { hand.clear(); discard.clear(); }
		void take_card(card in_c) { hand.push_back(in_c); }
		unsigned int return_id(void) { return player_id; }
		std::string return_name(void) { return player_name; }
		void display_hand(void) {
			display_deck(hand);
		}
		void display_player(void) {
			std::count << "Player " << name << " has " << credits << " credits" << std::endl;
		}
	};
	
	// NEEDS WORK
	class PokerGame {
	private:
		Board board;					// the board
		DeckAssistant da;				// deck assistant
		std::vector <card> deck;		// the deck
		std::vector <Player> players;	// all players
		std::vector <unsigned int> players_remaining;
		
		unsigned int ante_amount;		// ante amount
		
		card get_deck_top_card() {
			card t = deck.at(0);
			deck.erase(deck.begin());
			return t;
		}
		
		void setup_players(unsigned int in_pc) {
			for (std::size_t i=0; i<in_pc; i++) {
				Player p(0);  // default money
				players.push_back(p, i);
				players_remaining.push_back(1);
			}
		}
		
		// determine current player with iterator
		// required because the first player changes each round
		// test: NA
		unsigned int determine_position(unsigned int in_p, unsigned int in_i) {
			unsigned int c = in_p + in_i;
			while (c >= player_count) c -= player_count;
			return c;
		}
		
		// update the current_player variable
		// test: NA
		void update_current_player() {
			current_player += 1;
			if (current_player >= player_count) current_player = 0;
		}
		
		// each round, everyone is back in
		void reset_players_remaining() {
			players_remaining.clear();
			for (std::size_t i = 0; i<players.size(); ++i) {
				players_remaining.push_back(1);
			}
		}
		
		// after someone folds, remove them
		void player_folds(unsigned int in_p) {
			players_remaining.at(in_p) = 0;
		}
		
		// if there is only one player left, they win
		// test: NA
		bool check_winner_by_fold() {
			unsigned int t = 0; // sum of players remaining elements
			unsigned int p = 0; // last checked player position still in game
			for (std::size_t i=0; i<players_remaining.size(); ++i) {
				t = t+players_remaining.at(i);
				if (players_remaining.at(i) == 1) p = i;
			} 
			if (t < 2) {
				payout_round();
			}
			else return false;
		}
		
		void clear_players_hand() {
			for (std::size_t i=0; i<players.size(); ++i) {
				players.at(i).clear_cards();
			}
		}
		
		// at start of round, deal 2 cards to all players from the boards deck,
		//		after clearing their hand
		// test: NA
		void cards_to_players(void) {
			// deal first card
			for (std::size_t i = 0; i<players.size(); ++i) {
				unsigned int c;
				c = determine_position(current_player, i);
				// if still in game, give card from hand
				if (players_remaining.at(c) == 1)
					players.at(c).take_card(get_deck_top_card());
			}
			// deal second card
			for (std::size_t i=0; i<players.size(); ++i) {
				unsigned int c;
				c = determine_position(current_player, i);
				if (players_remaining.at(c) == 1)
					players.at(c).take_card(get_deck_top_card());
			}
		}
		
		//
		void apply_ante(void) {
			for (int i=0; i<players.size(); i++) {
				if(!players.at(i).give_money(ante_amount)) player_folds(in_p);
			}
		}
		//
		void board_cycle(unsigned int in) {
			switch (in) {
				case 1:
					board.add_discard(get_deck_top_card()); // discard
					for (std::size_t i=0; i<3; ++i) {
						board.add_hand(get_deck_top_card()); // flop three times
					}
					break;
				case 2:
					board.add_discard(get_deck_top_card()); // discard
					board.add_hand(get_deck_top_card()); // turn
					break;
				case 3:
					board.add_discard(get_deck_top_card()); // discard
					board.add_hand(get_deck_top_card()); // river
					break;
				default:
					break;
			}
		}
		
		void bet_round() {
			
		}
		
		void round() {
			board.clear_board();
			deck = da.shuffle_deck(deck);
			reset_players_remaining();
			apply_ante();
			cards_to_players();
			bool t = true;
			while(t) {
				// bet round
				board_cycle(1);
				// bet round
				board_cycle(2);
				// bet round
				board_cycle(3);
				// bet round
			}
		}
	public:
		PokerGame() {
			game = true;
			ante_amount = 1;
			setup_players();
			deck = da.create_deck();
		}
		void play_poker() {
			while (game) {
				round();
			}
		}
	};
	
};

#endif
