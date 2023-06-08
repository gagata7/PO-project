#pragma once
#include <bits/stdc++.h>
#include "user.h"
#include "librarian.h"
#include "book.h"

using namespace std;

class Library
{
    map<unsigned int, User> users;           // baza użytkowników
    map<unsigned int, Librarian> librarians; // bibliotekarze
    map<unsigned int, Book> book_collection; // księgozbiór
public:
    Library() {}

    void extendBookCollection(Book new_book);
    void removeBook(unsigned int book_id);
    void hireNewLibrarian(Librarian new_librarian);
    void fireLibrarian(unsigned int user_id);
    void addNewUser(User new_user);
    void deleteUser(unsigned int user_id);
    void createCard(unsigned int user_id, Date date);
    map<unsigned int, User> &getUsers();
    map<unsigned int, Librarian> &getLibrarians();
    const map<unsigned int, Book> &getBookCollection();
    Book bookInfo(unsigned int book_id);
    Book retrieveBook(unsigned int book_id);
    void putBackBook(Book book);
};