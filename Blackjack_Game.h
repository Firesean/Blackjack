#pragma once
#include "Dealer.h" // Includes Player.h -> Hand.h -> Deck.h -> Card.h
//#include "Seat.h"

class Blackjack {
private:
	int bet;
	Dealer* dealer;
	Deck *deck;
	Player* player;
	bool waiting;
public:

	Blackjack() {
		this->deck = new Deck();
		this->dealer = new Dealer();
		this->player = new Player();
		this->bet = 0;
		this->waiting = 1;
	}

	void clearHands() {
		this->player->getHand()->clearHand();
		this->dealer->getHand()->clearHand();
	}

	int getBet() {
		return this->bet;
	}

	int getCardScore(Card* card) {
		return card->getValue();
	}

	Dealer* getDealer() {
		return this->dealer;
	}

	Deck* getDeck() {
		return this->deck;
	}

	Player* getPlayer() {
		return this->player;
	}

	int getScore(Dealer* dealer) {
		vector<Card*>* cards;
		int total = 0;
		int aces = 0;
		int handSize = dealer->getHand()->getSize();
		cards = dealer->getHand()->getCards();
		for (int i = 0; i < handSize; i++) {
			total += getCardScore(cards->at(i));
			if (getCardScore(cards->at(i)) == ACE) {
				aces += 1;
			}
		}
		if (total <= 11 && aces > 0) { total += 10; }
		return total;
	}

	int getScore(Player* player) {
		vector<Card*>* cards;
		int total = 0;
		int aces = 0;
		int handSize = player->getHand()->getSize();
		cards = player->getHand()->getCards();
		for (int i = 0; i < handSize; i++) {
			total += getCardScore(cards->at(i));
			if (getCardScore(cards->at(i)) == ACE) {
				aces += 1;
			}
		}
		if (total <= 11 && aces > 0) { total += 10; }
		return total;
	}

	bool isBlackjack(Player* player) {
		if (this->getScore(player) == 21 && player->getHand()->getSize() == 2) {
			return 1;
		}
		return 0;
	};

	bool is21(Player* player) {
		if (this->getScore(player) == 21) {
			return 1;
		}
		return 0;
	};


	bool isBlackjack(Dealer* dealer) {
		if (this->getScore(dealer) == 21 && dealer->getHand()->getSize() == 2) {
			return 1;
		}
		return 0;
	};

	bool is21(Dealer* dealer) {
		if (this->getScore(dealer) == 21) {
			return 1;
		}
		return 0;
	};

	bool isBust(Dealer* dealer) {
		if (this->getScore(dealer) > 21) {
			return 1;
		}
		return 0;
	}

	bool isBust(Player* player) {
		if (this->getScore(player) > 21) {
			return 1;
		}
		return 0;
	}

	void runGame() {
		this->waiting = 1;
		if (this->bet > 0 && this->player->getBalance() >= this->bet) {
			this->player->withdraw(this->bet);
		}
		this->clearHands();
		this->deck->fillDeck(); // 52 Cards are added
		this->dealer->shuffleDeck(this->deck); // Randomizes deck
		this->dealer->dealCards(this->player, this->deck); // Deals to dealer and player // 2 to Dealer and player with 2 burned cards
	};

	void setBet(int bet) {
		this->bet = bet;
	}


	void stay() {
		this->waiting = 0;
		while (getScore(this->getDealer()) < 17 && !this->isBust(player)) {
			this->getDealer()->dealCard(this->getDealer(), this->getDealer()->drawTop(deck));
		}
	}

	bool waitingAction() {
		return this->waiting;
	}
};