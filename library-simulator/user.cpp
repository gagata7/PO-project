#include <bits/stdc++.h>
#include "user.h"
using namespace std;

string User::getName()
{
    return name;
}
string User::getSurname()
{
    return surname;
}
unsigned int User::getUserId()
{
    return user_id;
}
LibraryCard *User::getCardPtr()
{
    if (!has_card)
        throw runtime_error("This user's card is inactive - cannot get card info.");
    return card;
}
bool User::doesHaveCard()
{
    return has_card;
}
void User::deleteCard()
{
    has_card = false;
    delete card;
}
void User::addCard(Date today)
{
    if (has_card)
        throw runtime_error("User already has a card.");
    has_card = true;
    card = new LibraryCard(
        name,
        surname,
        today);
}

unsigned int User::nextFreeUserId = 0;
