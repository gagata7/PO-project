#pragma once
#include "library_card.h"
#include "additional.h"

using namespace std;

class User
{
protected:
    static unsigned int nextFreeUserId;
    string name, surname;
    const unsigned int user_id; // id w bazie użytkowników pozostaje niezmienny
    LibraryCard *card = nullptr;
    bool has_card = false; // to informuje czy użytkownik ma kartę
public:
    User() : user_id(0) {}
    User(const User &user) : name(user.name), surname(user.surname), user_id(user.user_id), card(user.card), has_card(user.has_card) {}
    User(string _name, string _surname) : name(_name), surname(_surname), user_id(nextFreeUserId++) {}
    string getName();
    string getSurname();
    unsigned int getUserId();
    LibraryCard *getCardPtr();
    bool doesHaveCard();
    void deleteCard();
    void addCard(Date today);
};
