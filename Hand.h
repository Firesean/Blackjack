#pragma once
#include <vector>
#include "Deck.h"

class Hand {
private:
	vector<Card*>* cards;
public:

	Hand() {
		this->cards = new vector<Card*>();
	}
	
	int getSize() {
		return this->cards->size();
	}

	void addCard(Card* card) {
		this->cards->push_back(card);
	}

	void clearHand() {
		this->cards = new vector<Card*>();
	}

	vector<Card*>* getCards() {
		return this->cards;
	}
};