#pragma once
#include <iostream>
#include <string>
using namespace std;

enum Suits { CLUBS, DIAMONDS, HEARTS, SPADES };

enum Values { ACE = 1, TWO = 2, THREE = 3, FOUR = 4, FIVE = 5, SIX = 6, SEVEN = 7, EIGHT = 8, NINE = 9, TEN = 10, JACK = 11, QUEEN = 12, KING = 13 };

class Card {
private:
    Suits suit;
    Values value;

public:
    Card() {
        this->value = Values::ACE;
        this->suit = Suits::CLUBS;
    } // Default Constructor
    Card(Values value, Suits suit) {
        this->value = value;
        this->suit = suit;
    }
    

    // Returns Value of Suit
    string getCardName() {
        string cardName = this->getValueStr() + " of " + this->getSuit();
        return cardName;
    }

    // Returns the suit
    string getSuit() {
        switch (this->suit) {
        case Suits::CLUBS:
            return "Clubs";
        case Suits::DIAMONDS:
            return "Diamonds";
        case Suits::HEARTS:
            return "Hearts";
        case Suits::SPADES:
            return "Spades";
        default:
            return "No Suit";
        }
    }


    // Returns the value
    int getValue() {
        switch (this->value) {
        case Values::JACK:
        case Values::QUEEN:
        case Values::KING:
            return 10;
        default:
            return this->value;
        }
    }


    // Returns the face value
    string getValueStr() {
        switch (this->value) {
        case Values::ACE:
            return "Ace";
        case Values::TWO:
            return "Two";
        case Values::THREE:
            return "Three";
        case Values::FOUR:
            return "Four";
        case Values::FIVE:
            return "Five";
        case Values::SIX:
            return "Six";
        case Values::SEVEN:
            return "Seven";
        case Values::EIGHT:
            return "Eight";
        case Values::NINE:
            return "Nine";
        case Values::TEN:
            return "Ten";
        case Values::JACK:
            return "Jack";
        case Values::QUEEN:
            return "Queen";
        case Values::KING:
            return "King";
        default:
            return "Invalid";
        }
    }
};