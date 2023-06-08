#pragma once
#include <bits/stdc++.h>
#include "transaction.h"

using namespace std;

class Book
{
    static unsigned int nextFreeBookId; // automatycznie przydziela świeże ID każdemu egzemplarzowi
    const unsigned int id;
    vector<Transaction> transactions;

    bool is_borrowed = false;
    vector<BorrowTransaction> lastBorrowTransaction;

    const string title, author, ISBN;

public:
    Book() : id(0) {}
    Book(const Book &book) : id(book.id), title(book.title), author(book.author), ISBN(book.ISBN),
                             transactions(book.transactions), is_borrowed(book.is_borrowed), lastBorrowTransaction(book.lastBorrowTransaction) {}

    Book(string _title, string _author, string _ISBN) : id(nextFreeBookId++), title(_title), author(_author), ISBN(_ISBN) {}

    const vector<Transaction> &getTransactions();
    unsigned int getId();
    void borrowBook(BorrowTransaction transaction);
    void returnBook(ReturnTransaction transaction);
    string getTitle();
    string getAuthor();
    string getISBN();
    bool isBorrowed();
    BorrowTransaction getLastBorrowTransaction();
};
