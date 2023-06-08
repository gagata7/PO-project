#pragma once
#include <bits/stdc++.h>
#include "transaction.h"
#include "book.h"
#include "fine.h"

using namespace std;

class LibraryCard
{
    static unsigned int nextFreeId; // static - zmienna współdzielona między wszystkimi obiektami klasy, inaczej: należy do klasy
    const string name, surname;
    const unsigned int id;
    const Date start_date;
    vector<Transaction> transactions;
    map<unsigned int, Book> borrowed_books;
    vector<Fine> fines;

public:
    LibraryCard() : name(""), surname(""), id(0), start_date({0, 0, 0}) {}
    LibraryCard(string _name, string _surname, Date _start_date) : name(_name), surname(_surname), id(nextFreeId++), start_date(_start_date) {}
    void borrowABook(Book book, Date today, Date deadline);
    pair<Book, bool> returnABook(unsigned int book_id, Date today);
    unsigned int getId() const;
    Date getStartDate();
    const vector<Transaction> &getTransactions();
    const map<unsigned int, Book> &getBorrowedBooks() const;
    void applyFine(Fine f);
    const vector<Fine> &getFines();
    void payFine(unsigned int money);
    string getName();
    string getSurname();
};