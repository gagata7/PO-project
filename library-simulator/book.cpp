#include <bits/stdc++.h>
#include "book.h"

using namespace std;

const vector<Transaction> &Book::getTransactions()
{
    // to może być baaaardzo duży wektor, dlatego zwracam referencję do niego
    return transactions;
}
unsigned int Book::getId()
{
    return id;
}
void Book::borrowBook(BorrowTransaction transaction)
{
    if (is_borrowed)
        throw runtime_error("Book borrowed twice.");
    Transaction *transaction_ptr = dynamic_cast<Transaction *>(&transaction);
    transactions.push_back(*transaction_ptr);
    is_borrowed = true;
    lastBorrowTransaction.emplace_back(transaction);
}

void Book::returnBook(ReturnTransaction transaction)
{
    if (!is_borrowed)
        throw runtime_error("Book has not been borrowed.");
    Transaction *transaction_ptr = dynamic_cast<Transaction *>(&transaction);
    transactions.push_back(*transaction_ptr);
    is_borrowed = false;
    lastBorrowTransaction.clear();
}

string Book::getTitle()
{
    return title;
}
string Book::getAuthor()
{
    return author;
}
string Book::getISBN()
{
    return ISBN;
}
bool Book::isBorrowed()
{
    return is_borrowed;
}
BorrowTransaction Book::getLastBorrowTransaction()
{
    return lastBorrowTransaction.back();
}
unsigned int Book::nextFreeBookId = 0;
