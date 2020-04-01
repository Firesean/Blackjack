#pragma once
#include <iostream>
#include <string>
using namespace std;

enum class Suits { CLUBS, DIAMONDS, HEARTS, SPADES};

enum class Values { ACE = 1, TWO = 2, THREE = 3, FOUR = 4, FIVE = 5, SIX = 6, SEVEN = 7, EIGHT = 8, NINE = 9, TEN = 10, JACK = 11, QUEEN = 12, KING = 13};

string retrieveSuitStr(Suits suit) {
    switch (suit) {
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

int retrieveValueInt(Values value) {
    switch(value){
    case Values::ACE:
        return 1;
    case Values::TWO:
        return 2;
    case Values::THREE:
        return 3;
    case Values::FOUR:
        return 4;
    case Values::FIVE:
        return 5;
    case Values::SIX:
        return 6;
    case Values::SEVEN:
        return 7;
    case Values::EIGHT:
        return 8;
    case Values::NINE:
        return 9;
    default:
        return 10;
    }
}

class Card
{
    private:
        Suits suit;
        Values value;
    public:
        Card() {
            this->value = Values(0);
            this->suit = Suits(0);
        }; // Default Constructor
        Card(Values value, Suits suit) {
            this->value = value;
            this->suit = suit;
        };

        string getSuit() {
            return retrieveSuitStr(this->suit);
        };

        int getValue() {
            return retrieveValueInt(this->value);
        };

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
                };

            };
};

