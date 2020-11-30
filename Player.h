#pragma once
#include "Hand.h"

class Player {
private:
	Hand* hand;
	int balance;
public:

	Player() {
		this->balance = 500;
		this->hand = new Hand();
	}

	void addCard(Card* card) {
		this->hand->addCard(card);
	}

	void deposit(int money) {
		this->balance += money;
	}

	int getBalance() {
		return this->balance;
	}

	Hand* getHand() {
		return this->hand;
	}

	void setBalance(int money) {
		this->balance = money;
	}

	void withdraw(int money) {
		if (money <= this->balance) {
			this->balance -= money;
		}
	}
};