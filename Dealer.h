#pragma once
#include "Player.h"
#include <iostream>
#include <stdlib.h> // Random
#include <time.h>  // Time
#include <algorithm> // random_shuffle

using namespace std;

const int STARTHANDCOUNT = 2;
int AVGCARDPERHAND = 3;

class Dealer : public Player {
	public:
		
		Dealer() {
			this->setBalance(500);
		}

		Card* drawTop(Deck* deck) {
			Card* card;
			card = deck->removeTopCard();
			return card;
		}

		void shuffleDeck(Deck* deck) {
			srand(time(0)); // Randomizes seed based on current time
			random_shuffle(deck->getCards()->begin(), deck->getCards()->end());
		}

		void burnTop(Deck *deck) {
			deck->removeTopCard();
		}

		void dealCard(Dealer* dealer, Card* card) {
			dealer->addCard(card);
		}

		void dealCard(Player* player, Card* card) {
			player->addCard(card);
		}

		void dealCards(Player* player, Deck* deck) {
			if (AVGCARDPERHAND > deck->getSize() / 2) {
				deck->fillDeck();
				this->shuffleDeck(deck);
			}
			for (int i = 0; i < STARTHANDCOUNT; i++) {
				this->dealCard(player, this->drawTop(deck));
				this->dealCard(this, this->drawTop(deck));
				this->burnTop(deck);
			}
		}
};