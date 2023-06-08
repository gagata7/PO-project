#include <bits/stdc++.h>
#include "library_card.h"

using namespace std;

void LibraryCard::borrowABook(Book book, Date today, Date deadline)
{
    if (book.isBorrowed())
        throw runtime_error("Book has already been borrowed.");
    BorrowTransaction transaction = BorrowTransaction(today, id, book.getId(), deadline);
    book.borrowBook(transaction);
    transactions.push_back(transaction);
    borrowed_books.emplace(book.getId(), book); // emplace konstruuje obiekt typu Book w miejscu - bez kopiowania
}
pair<Book, bool> LibraryCard::returnABook(unsigned int book_id, Date today)
{
    if (!borrowed_books.count(book_id))
        throw runtime_error("There's no book with this ID.");
    Book book = borrowed_books[book_id];
    if (!book.isBorrowed())
        throw runtime_error("Book has not been borrowed.");
    ReturnTransaction transaction = book.getLastBorrowTransaction().createReturnTransaction(today);
    book.returnBook(transaction);
    transactions.push_back(transaction);
    borrowed_books.erase(book_id);
    return {book, transaction.isOvertime()};
}
unsigned int LibraryCard::getId() const
{
    return id;
}
Date LibraryCard::getStartDate()
{
    return start_date;
}
const vector<Transaction> &LibraryCard::getTransactions()
{
    return transactions;
}
const map<unsigned int, Book> &LibraryCard::getBorrowedBooks() const
{
    return borrowed_books;
}
void LibraryCard::applyFine(Fine f)
{
    fines.push_back(f);
}
const vector<Fine> &LibraryCard::getFines()
{
    return fines;
}
void LibraryCard::payFine(unsigned int money)
{
    unsigned int leftoverMoney = money;

    while (!fines.empty() && leftoverMoney > 0)
    {
        Fine lastFine = fines.back();
        fines.pop_back();
        if (leftoverMoney >= lastFine.getFineValue())
            leftoverMoney -= lastFine.getFineValue();
        else
        {
            fines.emplace_back(lastFine.getFineValue() - leftoverMoney);
            leftoverMoney = 0;
        }
    }

    if (fines.empty())
        cout << "All fines have been paid.\n";
    else
        cout << "Some fines have not been paid in full.\n";

    if (leftoverMoney > 0)
        cout << leftoverMoney << "$ has been returned\n";
}
string LibraryCard::getName()
{
    return name;
}
string LibraryCard::getSurname()
{
    return surname;
}
unsigned int LibraryCard::nextFreeId = 0;