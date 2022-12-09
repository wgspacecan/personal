// NEEDS WORK - HARD
	class PokerScorer {
	private:
		// high card
		unsigned int check_high_card(std::vector <card> in_hand) {
			unsigned int score = 0;
			for (std:size_t i=0; i<in_hand.size(); ++i) {
				unsigned int t = card_to_score(in_hand.at(i));
				if (t > score) score = t;
			}
			return score;
		}
		// pair
		unsigned int check_pair(std::vector <card> in_hand) {
			unsigned int score = 0;
			std::vector <unsigned int> score_vec = create_score_vector(in_hand);
			std::vector <unsigned int> heat_map = create_null_heatmap();
			for (std::size_t i=0; i<score_vec.size(); ++i) {
				heat_map.at(score_vec.at(i)) = heat_map.at(score_vec.at(i)) + 1;
			}
			return score;
		}
		// three of a kind
		unsigned int check_three_kind(std::vector <card> in_hand) {
			unsigned int score = 0;
			
			return score;
		}
		// straight
		unsigned int check_traight(std::vector <card> in_hand) {
			unsigned int score = 0;
			
			return score;
		}
		// flush
		unsigned int check_flush(std::vector <card> in_hand) {
			unsigned int score = 0;
			
			return score;
		}
		// four of a kind
		unsigned int check_four_kind(std::vector <card> in_hand) {
			unsigned int score = 0;
			
			return score;
		}
		// two pair: check_pair
		// full house: check_three_kind, check_pair
		// straight flush: check_straight, check_flush
		// royal flush: check_straight*, check_flush
		std::vector <unsigned int> check_score(std::vector<unsigned int> in_score) {
			
		}
		std::vector <unsigned int> create_null_heatmap() {
			std::vector <unsigned int> t;
			for (std::size_t i=0; i<14; ++i) {
				t.push_back(0);
			} return t;
		}
		unsigned int check_heat_map(std::vector <unsigned int> in_h) {
			unsigned int t = 0;
			for (std::size_t i=0; i<in_h.size(); ++i) {
				if (in_h.at(i) > t) t = in_h.at(i);
			}
			return t;
		}
		std::vector <unsigned int> create_score_vector(std::vector <card> in_hand) {
			std::vector <unsigned int> score_vec;
			for (std::size_t i=0; i<in_hand.size(); i++) {
				unsigned int score = card_to_score(in_hand.at(i));
				score_vec.push_back(score);
			}
			return score_vec;
		}
		unsigned int card_to_score(card in_card) {
			unsigned int score = 0;
			std::string in_face = in_card.face;
			if(in_face == "1") score = 1; 
			else if(in_face == "2") score = 2;
			else if(in_face == "3") score = 3;
			else if(in_face == "4") score = 4;
			else if(in_face == "5") score = 5;
			else if(in_face == "6") score = 6;
			else if(in_face == "7") score = 7;
			else if(in_face == "8") score = 8;
			else if(in_face == "9") score = 9;
			else if(in_face == "10") score = 10;
			else if(in_face == "J") score = 11;
			else if(in_face == "Q") score = 12;
			else if(in_face == "K") score = 13;
			else if(in_face == "A") score = 14;
			return score;
		}
	public:
		unsigned int find_winner(std::vector <std::vector <card> > in_hands) {
			// royal flush
			// straight flush
			// four of a kind
			// full house
			// flush
			// straight
			// three of a kind
			// two pair
			// pair
			// high card
			std::vector<unsigned int> high_card;
			for (std::size_t i=0; i<in_hands.size(); ++i) {
				high_card.push_back(check_high_card(in_hands.at(i)));
			}
		}
	};