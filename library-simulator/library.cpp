#include <bits/stdc++.h>
#include "library.h"

using namespace std;


void Library::extendBookCollection(Book new_book)
{
    if (book_collection.count(new_book.getId()))
        throw runtime_error("There is already book with this ID.");
    book_collection.emplace(new_book.getId(), new_book);
}
void Library::removeBook(unsigned int book_id)
{
    if (!book_collection.count(book_id))
        throw runtime_error("There's no book with this ID.");
    book_collection.erase(book_id);
}
void Library::hireNewLibrarian(Librarian new_librarian)
{
    if (librarians.count(new_librarian.getUserId()))
        throw runtime_error("There is already hired librarian with this ID.");
    librarians.emplace(new_librarian.getUserId(), new_librarian);
}
void Library::fireLibrarian(unsigned int user_id)
{
    if (!librarians.count(user_id))
        throw runtime_error("There's no librarian with this ID.");
    librarians.erase(user_id);
}
void Library::addNewUser(User new_user)
{
    if (users.count(new_user.getUserId()))
        throw runtime_error("There is already registered user with this ID.");
    users.emplace(new_user.getUserId(), new_user);
}
void Library::deleteUser(unsigned int user_id)
{
    if (!users.count(user_id))
        throw runtime_error("There is no registered user with this ID.");
    users[user_id].deleteCard();
    users.erase(user_id);
}
void Library::createCard(unsigned int user_id, Date date)
{
    if (!users.count(user_id))
        throw runtime_error("There is no registered user with this ID.");
    users[user_id].addCard(date);
}
map<unsigned int, User> &Library::getUsers()
{
    return users;
}
map<unsigned int, Librarian> &Library::getLibrarians()
{
    return librarians;
}
const map<unsigned int, Book> &Library::getBookCollection()
{
    return book_collection;
}
Book Library::bookInfo(unsigned int book_id)
{
    return book_collection[book_id];
}
Book Library::retrieveBook(unsigned int book_id)
{
    if (!book_collection.count(book_id))
        throw runtime_error("There is no book with this ID.");
    Book book = book_collection[book_id];
    book_collection.erase(book_id);
    return book;
}
void Library::putBackBook(Book book)
{
    book_collection.emplace(book.getId(), book);
}