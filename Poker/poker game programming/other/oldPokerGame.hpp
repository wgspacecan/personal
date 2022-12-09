class PokerGame {
	private:
		Board board;					// the board
		std::vector <Player> players;	// all players

		// cycle
		std::vector <unsigned int> players_remaining;	// location of players remaining - ex (0, 0, 1)
		unsigned int first_player;		// player going first this round
		unsigned int current_player;	// player currently placing bets
		unsigned int current_bet_amount;// amount each player has to pay
		std::vector <int> current_bets;	// each players current bet
		
		unsigned int current_round;		// current round
		unsigned int current_cycle;		// current cycle, per round
		
		unsigned int fee_type;			// 0:none, 1: ante, 2: blind
		unsigned int ante_val;			// current ante (play cost)
		unsigned int blind_big_val;		// value of big blind
		unsigned int blind_small_val;	// value of small blind
		unsigned int max_round;			// max round (0 for no limit)
		
		bool player_won;				// true if player has won round
		//// Functions
		// at start of game, create all players
		void setup_players(unsigned int in_pc) {
#ifdef POKER_DEBUG
	std::cout << "DEBUG: Dealer::setup_players()" << std::endl;
#endif
			for (std::size_t i=0; i<in_pc; i++) {
				Player p(0);  // default money
				players.push_back(p, i);
				players_remaining.push_back(1);
			}
		}
		
		// at start of round, deal 2 cards to all players from the boards deck,
		//		after clearing their hand
		// test: NA
		void cards_to_players(void) {
#ifdef POKER_DEBUG
	std::cout << "DEBUG: Dealer::cards_to_players()" << std::endl;
#endif
			// deal first card
			for (std::size_t i = 0; i<players.size(); ++i) {
				unsigned int c;
				c = determine_position(current_player, i);
				players.at(c).clear_hand();
				players.at(c).take_card(board.return_card());
			}
			// deal second card
			for (std::size_t i = 0; i<players.size(); ++i) {
				unsigned int c;
				c = determine_position(current_player, i);
				players.at(c).take_card(board.return_card());
			}
		}
		// display useful information to the player
		// test: NA
		void display_bet_menu(unsigned int in_p) {
#ifdef POKER_DEBUG
	std::cout << "DEBUG: Dealer::display_bet_menu()" << std::endl;
#endif
			std::cout << "player " << in_p << " bet round" << std::endl;
			std::cout << "credits: " << players.at(in_p).return_balance() << std::endl;
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
		// menu to aid in transferring the device to another player
		// test: NA
		void transfer_device(void) {
#ifdef POKER_DEBUG
	std::cout << "DEBUG: Dealer::transfer_device()" << std::endl;
#endif
			char t;
			std::cout << "when done, type k and press enter" << std::endl;
			std::cin >> t; // clear screen
			clear_the_screen();
			std::cout << "pass the game to the next player." << std::endl;
			std::cout << "next player, type k and press enter to continue" << std::endl;
			std::cin >> t; // pass device
			clear_the_screen();
		}
		// determine current player with iterator
		// required because the first player changes each round
		// test: NA
		unsigned int determine_position(unsigned int in_p, unsigned int in_i) {
#ifdef POKER_DEBUG
	std::cout << "DEBUG: Dealer::determine_position()" << std::endl;
#endif
			unsigned int c = in_p + in_i;
			while (c >= player_count) c -= player_count;
			return c;
		}
		// set players 'current_bet'
		// test: NA
		void set_players_bet(unsigned int in_p, unsigned int in_a) {
#ifdef POKER_DEBUG
	std::cout << "DEBUG: Dealer::set_players_bet()" << std::endl;
#endif
			if (current_bets.at(in_p) != -1) current_bets.at(in_p) = current_bets.at(in_p) + in_a;
			else current_bets.at(in_p) = in_a;
		}
		// apply big and little blind
		// test: NA
		void apply_blind(unsigned int in_p) {
#ifdef POKER_DEBUG
	std::cout << "DEBUG: Dealer::apply_blind()" << std::endl;
#endif
			unsigned int b;
			b = determine_position(in_p, 1);
			players.at(b).give_blind(blind_big);
			b = determine_position(in_p, 2);
			players.at(b).give_blind(blind_small);
		}
		//
		void apply_ante(void) {
#ifdef POKER_DEBUG
	std::cout << "DEBUG: Dealer::apple_ante()" << std::endl;
#endif
			for (int i=0; i<players.size(); i++) {
				if(!players.at(i).give_money(ante_amount)) player_fold(in_p);
			}
		}
		// 
		void apply_fee(unsigned int in_p) {
#ifdef POKER_DEBUG
	std::cout << "DEBUG: Dealer::apple_fee()" << std::endl;
#endif
			if (fee_type == 1) { // ante
				apply_ante();
			} else if (fee_type == 2) { // blind
				apply_blind(in_p)
			}
		}
		// cycle all places for bet once
		// test: NA
		unsigned int all_bet(unsigned int lptr) {
#ifdef POKER_DEBUG
	std::cout << "DEBUG: Dealer::all_bet()" << std::endl;
#endif
			for (std::size_t i = 0; i < players.size(); ++i) {
				if (take_bet(determine_position(current_player, i))) {
					// new raise
					lptr = determine_position(current_player, i);
				}
				if (check_winner()) return lptr;
			}
			return lptr;
		}
		std::vector <unsigned int> bet_next(unsigned int lptp, unsigned int lptr) {
#ifdef POKER_DEBUG
	std::cout << "DEBUG: Dealer::bet_next()" << std::endl;
#endif
			std::vector <unsigned int> t;
			if (take_bet(determine_position(current_player, lptp))) {
				// new raise
				lptr = determine_position(current_player, lptp);
			}
			t.push_back(lptr);
			t.push_back(lptp);
			if (check_winner()) player_wins = true;
			return t;
		}
		/* 
			unsigned int get_bet ( unsigned int )
			
			input: current player (uint)
			output: bet ammount (uint) (0 to wallet)

			info: menu to enter bet ammount
		*/
		// test: NA
		unsigned int get_bet(unsigned int in_p) {
#ifdef POKER_DEBUG
	std::cout << "DEBUG: Dealer::get_bet()" << std::endl;
#endif
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
		// test: NA
		bool take_bet(unsigned int in_p) {
#ifdef POKER_DEBUG
	std::cout << "DEBUG: Dealer::take_bet()" << std::endl;
#endif
#ifdef POKER_VERBOSE
			display_useful_stats();
#endif
			bool r = false;
			int a = 0;
			// for single player while they decide bet
			while(true) {
				bool redo = false;
				display_bet_menu(in_p);
				std::cout << "you have " << players.at(in_p).return_balance() << " credits" << std::endl;
				std::cout << "would you like to bet? (y/n): ";
				// take bet
				char t;
				std::cin >> t;
				// if betting
				if (t == 'y' || t == 'Y') {
					// get bet and check with raise
					a = get_bet(in_p);
					if (a > raised_by) {
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
				- ante and blind
				- all betting
				- returns after round is complete
		
		*/
		// test: NA
		void bet_cycle(bool in_n) {
#ifdef POKER_DEBUG
	std::cout << "DEBUG: Dealer::bet_cycle()" << std::endl;
#endif
			// if new round,
			if (in_n) {
				// reset bets
				if (current_bets.size() == 0) {
					for (std::size_t i=0; i<players.size(); ++i) {
						current_bets.push_back(-1);
					}
				}
				// apply ante
				if(fee_type == 1) raised_by = current_ante;
				else raised_by = 0;
			} else raised_by = 0;
			// apply blind
			if (fee_type == 2) apply_blind(current_player);


			// #################################

			// each player take bets until no more raises
			while (true) {
				unsigned int lptp = current_player; // last player to play
				unsigned int lptr = current_player; // last player to raise
				bool first_round = true;
				// first round
				if (first_round) {
					lptr = all_bet(lptr);
					if (player_wins) break;
					first_round = false;
				}
				// play until everyone has agreed to last raise
				if (lptr != lptp) {
					std::vector <unsigned int> t = bet_next(lptp, lptr);
					lptr = t.at(0);
					lptp = t.at(1);
					if (player_wins) break;
				}
				break;
			}
			// #################################
		}
		// update the current_player variable
		// test: NA
		void update_current_player() {
#ifdef POKER_DEBUG
	std::cout << "DEBUG: Dealer::update_current_player()" << std::endl;
#endif
			current_player += 1;
			if (current_player >= player_count) current_player = 0;
		}
		// each round, everyone is back in
		// test: NA
		void reset_players_remaining() {
#ifdef POKER_DEBUG
	std::cout << "DEBUG: Dealer::reset_players_remaining()" << std::endl;
#endif
			players_remaining.clear();
			for (std::size_t i = 0; i<players.size(); ++i) {
				players_remaining.push_back(1);
			}
		}
		// after someone folds, remove them
		// test: NA
		void remove_player(unsigned int in_p) {
#ifdef POKER_DEBUG
	std::cout << "DEBUG: Dealer::remove_plaeyer()" << std::endl;
#endif
			players_remaining.at(in_p) = 0;
			//players_remaining.erase(std::remove(players_remaining.begin(), players_remaining.end(), in_p), players_remaining.end());
		}
		// if there is only one player left, they win
		// test: NA
		bool check_winner_by_fold() {
#ifdef POKER_DEBUG
	std::cout << "DEBUG: Dealer::check_winner_by_fold()" << std::endl;
#endif
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
		bool check_winner_by_hand() {
#ifdef POKER_DEBUG
	std::cout << "DEBUG: Dealer::check_winner_by_hand()" << std::endl;
#endif
			// uhh
		}
		void bet_cycle() {
			// show board
			// show hand
			// choose bet, check, fold
			
		}
		void payout_round() {
#ifdef POKER_DEBUG
	std::cout << "DEBUG: Dealer::payout_round()" << std::endl;
#endif			
			unsigned int v = 0;
			// give pot to winner
			for (std::size_t i=0; i<current_bets.size(); ++i) {
				if (current_bets.at(i) > 0) v += current_bets.at(i);
			}
			players.at(p).take_money(v);
			std::cout << "payout: " << v << std::endl;
			player_wins = true;
		}
		// display exit info
		// test: NA
		void display_exit() {
#ifdef POKER_DEBUG
	std::cout << "DEBUG: Dealer::display_exit()" << std::endl;
#endif
			clear_the_screen();
			std::cout << "\nEnd of game\n" << std::endl;
			std::cout << "player scores:" << std::endl;
			for (std::size_t i=0; i<players.size(); ++i) {
				std::cout << " player: " << i;
				std::cout << "; score: " << players.at(i).return_balance() << std::endl;
			} std::cout << std::endl;
		}
		// menu to enter desired player count
		// test: NA
		unsigned int get_player_count() {
#ifdef POKER_DEBUG
	std::cout << "DEBUG: Dealer::get_player_count()" << std::endl;
#endif
			while (true) {
				std::cout << "how many players? (1-8): " << std::endl;
				unsigned int p = 0;
				std::cin >> p;
				if (p > 0 && p < 9) {
					clear_the_screen();
					return p;
				}
				else std::cout << "error with player count" << std::endl;
			}
		}
		// cleanup for new round
		// test: NA
		void new_round() {
#ifdef POKER_DEBUG
	std::cout << "DEBUG: Dealer::new_round()" << std::endl;
#endif
			reset_players_remaining();
			current_round++;
			board.set_deck(create_deck());
			cards_to_players();
			current_bets.clear();
			update_current_player();
		}
		void display_useful_stats() {
			std::cout << "INFO: current_round: " << current_round << std::endl;
			std::cout << "INFO: current_player: " << current_player << std::endl;
			std::cout << "INFO: player_count: " << player_count << std::endl;
			std::cout << "INFO: players.size(): " << players.size() << std::endl;
			std::cout << "INFO: raised_by: " << raised_by << std::endl;
			std::cout << "INFO: players_remaining: " << players_remaining.size() << std::endl;
			std::cout << "INFO: current_cycle: " << current_cycle << std::endl;
//			std::cout << "INFO: " << std::endl;
//			std::cout << "INFO: " << std::endl;
			std::cout << std::endl;
		}
	public:	
		// settings
		Dealer() {
			max_round = 0;		// max round
			ante_val = 1;	// cost to play
			blind_big_val = 2;
			blind_small_val = 1;
			fee_type = 1;
		}
		// main game loop
		void play(int in_pc) {
			current_round = 0;
			current_player = 0
			srand(time(NULL));
			clear_the_screen();
			std::cout << "Welcome to MP Poker\n" << std::endl;
			if (in_pc == -1) player_count = get_player_count();
			else player_count = in_pc;
			setup_players(player_count);
			bool game_on = true;
			while(game_on) {  // end game
			while(game_on) {  // end round
				// exit
#ifdef POKER_DEBUG
	std::cout << "DEBUG: play() exit" << std::endl;
#endif
				if (current_round >= max_round) game_on = false;
				// setup
#ifdef POKER_DEBUG
	std::cout << "DEBUG: play() game setup" << std::endl;
#endif	
				player_wins = false;
				new_round();
				// 1 - flop
#ifdef POKER_DEBUG
	std::cout << "DEBUG: play() flop" << std::endl;
#endif
				board.cycle(1);
				bet_cycle(true);
				if (player_wins) break;
				// 2 - 
#ifdef POKER_DEBUG
	std::cout << "DEBUG: play() 2" << std::endl;
#endif
				board.cycle(2);
				bet_cycle(false);
				if (player_wins) break;
				// 3 - river
#ifdef POKER_DEBUG
	std::cout << "DEBUG: river" << std::endl;
#endif
				board.cycle(3);
				bet_cycle(false);
				if (player_wins) break;
			}
			}
			display_exit();
		}
	};
	