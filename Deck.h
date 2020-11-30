#pragma once
#include <vector>
#include "Card.h"

class Deck {

private:
	vector<Card*>* cards;

public:

	Deck() {
		this->cards = new vector<Card*>();
	}

	void fillDeck() {
		this->cards = new vector<Card*>();
		Card* card;
		for (int suit = CLUBS; suit <= SPADES; suit++) {
			for (int value = ACE; value <= KING; value++) {
				card = new Card(Values(value), Suits(suit));
				cards->push_back(card);
			}
		}
	}

	vector<Card*>* getCards() {
		return this->cards;
	}

	int getSize() {
		return this->cards->size();
	}

	Card* removeTopCard() {
		Card* topCard = this->cards->at(0);
		this->cards->erase(this->cards->begin());
		return topCard;
	}

	void setCards(vector<Card*>* cards) {
		this->cards = cards;
	}
};