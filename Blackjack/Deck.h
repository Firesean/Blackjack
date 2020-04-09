#include <vector>
#include "Card.h"
class Deck
{
	public:
		vector<Card> cards;

		void generateDeck() {
			this->cards = {};
			Card card;
			for (int suit = CLUBS; suit <= SPADES; suit++) {
				for (int value = ACE; value <= KING; value++) {
					card = Card(Values(value), Suits(suit));
					cards.push_back(card);
				};
			};
		};

		vector<Card> getCards() {
			return this->cards;
		};

		int getSize() {
			return this->cards.size();
		};
};