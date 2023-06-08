#include <bits/stdc++.h>
#include "additional.h"
#include "book.h"
#include "fine.h"
#include "librarian.h"
#include "library_card.h"
#include "library.h"
#include "transaction.h"
#include "user.h"

using namespace std;

ostream &operator<<(ostream &os, Book b) // to sprawia, że cout będzie wypisywał obiekt typu Book
{
    os << "{ Book :\n\tTitle: " << b.getTitle() << "\n\tAuthor: " << b.getAuthor() << "\n\tID: " << b.getId() << "\n}\n";
    return os;
}

ostream &operator<<(ostream &os, User u) // to sprawia, że cout będzie wypisywał obiekt typu User
{
    os << "{ User :\n\tName: " << u.getName() << "\n\tSurname: " << u.getSurname() << "\n\tID: " << u.getUserId()
       << "\n\tHas Card: " << u.doesHaveCard();
    if (u.doesHaveCard())
    {
        os << "\n\tCard ID: " << u.getCardPtr()->getId();
    }
    os << "\n}\n";
    return os;
}

int main()
{
    Library library;
    while (true)
    {
        string command;
        cout << "> ";
        cin >> command;

        transform(command.begin(), command.end(), command.begin(), [](unsigned char c)
                  { return tolower(c); });
        if (command == "exit")
            return 0;
        if (command == "help")
        {
            cout << "Available commands: exit, help, lsbooks, lsusers, lsLibrarians,\n addBook, deleteBook, addUser, deleteUser, hireLibrarian, \n fireLibrarian, createCard, borrowBook, returnBook, bookInfo, \n userInfo, borrowedBooks, balance, lsfines, payFine\n";
            continue;
        }
        if (command == "lsbooks")
        { // wypisywanie info o książkach w księgozbiorze
            for (auto p : library.getBookCollection())
                cout << p.second;
            continue;
        }
        if (command == "lsusers")
        { // wypisuje użytkowników
            for (auto p : library.getUsers())
                cout << p.second;
            continue;
        }
        if (command == "lslibrarians")
        { // wypisuje użytkowników
            for (auto p : library.getLibrarians())
                cout << p.second;
            continue;
        }
        if (command == "addbook")
        { // dodanie książki do księgozbioru
            string title, author, isbn;
            cout << "Give a title, author and ISBN: ";
            cin >> title >> author >> isbn;
            library.extendBookCollection(Book(title, author, isbn));
            cout << "\n";
            cout << "Book has beed added successfully.\n";
            continue;
        }
        if (command == "deletebook")
        { // usunięcie egzemplaża z księgozbioru biblioteki
            unsigned int book_id;
            cout << "Give a bookID: ";
            cin >> book_id;
            if (!library.getBookCollection().count(book_id))
            {
                cout << "There's no book with this id.\n";
                continue;
            }
            library.removeBook(book_id);
            cout << "\n";
            cout << "Book has beed removed successfully.\n";
            continue;
        }
        if (command == "adduser")
        { // dodanie nowego użytkownika
            string name, surname;
            cout << "Give a name and surname: ";
            cin >> name >> surname;
            library.addNewUser(User(name, surname));
            cout << "User was registered successfully.\n";
            cout << "\n";
            continue;
        }
        if (command == "deleteuser")
        { // usunięcie użytkownika z bazy czytelników
            unsigned int user_id;
            cout << "Give a UserID: ";
            cin >> user_id;
            if (!library.getUsers().count(user_id))
            {
                cout << "This userID does not exist.\n";
                continue;
            }
            library.deleteUser(user_id);
            cout << "User was deleted successfully.\n";
            cout << "\n";
            continue;
        }
        if (command == "hirelibrarian")
        { // zatrudnienie nowego bibliotekarza
            string name, surname;
            map<DayOfTheWeek, pair<TimeOfTheDay, TimeOfTheDay>> schedule;
            cout << "Give a name and surname: ";
            cin >> name >> surname;
            unsigned int weekly = 0; // suma minut spędzonych na pracy bibliotekarza tygodniowo
            for (int i = 0; i < 7; i++)
            {
                string ans;
                cout << "Work on " << daysOfTheWeek[i] << "? ";
                cin >> ans;
                if (ans == "yes")
                {
                    unsigned int starth, startm, endh, endm;
                    cout << "Give hh mm of start: ";
                    cin >> starth >> startm;
                    if (!good_time(starth, startm))
                    {
                        cout << "Wrong time format.\n";
                        continue;
                    }
                    cout << "Give hh mm of end: ";
                    cin >> endh >> endm;
                    if (!good_time(endh, endm))
                    {
                        cout << "Wrong time format.\n";
                        continue;
                    }
                    weekly += minutes_between(endh, endm, starth, startm);
                    if (weekly > 40 * 60)
                    {
                        cout << "Librarians are not allowed to work more than 40h per week.\n";
                        break;
                    }
                    schedule[DayOfTheWeek(i)] = {TimeOfTheDay({starth, startm}), TimeOfTheDay({endh, endm})};
                }
                continue;
            }
            if (weekly < 60)
            {
                cout << "Librarians must work at least 1h per week, in order to be hired.\n";
                continue;
            }
            if (weekly > 40 * 60)
            {
                cout << "Hiring process not successfull.\n";
                continue;
            }
            library.hireNewLibrarian(Librarian(name, surname, schedule));
            cout << "New librarian was hired successfully.\n";
            cout << "\n";
            continue;
        }
        if (command == "firelibrarian")
        { // zwolnienie bibliotekarza
            unsigned int user_id;
            cout << "Give a UserID: ";
            cin >> user_id;
            if (!library.getLibrarians().count(user_id))
            {
                cout << "There's no librarian with this userID.\n";
                continue;
            }
            library.fireLibrarian(user_id);
            cout << "Librarian was fired successfully.\n";
            cout << "\n";
            continue;
        }
        if (command == "createcard")
        { // stworzenie nowej karty dla danego użytkownika
            unsigned int user_id;
            cout << "Give me userID: ";
            cin >> user_id;
            if (!library.getUsers().count(user_id))
            {
                cout << "Invalid userID.\n";
                continue;
            }
            unsigned int d, m, y;
            cout << "Give today's date in format YYYY MM DD: ";
            cin >> y >> m >> d;
            if (!good_date(y, m, d))
            {
                cout << "Wrong date format.\n";
                continue;
            }
            library.createCard(user_id, Date({y, m, d}));
            cout << "\n";
            cout << "Card was created successfully.\n";
            continue;
        }
        if (command == "borrowbook")
        { // transakcja wypożyczenia książki
            unsigned int user_id, book_id;
            cout << "Input your userID number: ";
            cin >> user_id;
            cout << "Input bookID: ";
            cin >> book_id;
            if (!library.getUsers().count(user_id))
            {
                cout << "This userID does not exist.\n";
                continue;
            }
            if (!library.getUsers()[user_id].doesHaveCard())
            {
                cout << "This user does not have a library card.\n";
                continue;
            }
            if (!library.getBookCollection().count(book_id))
            {
                cout << "This book does not exist or has already been borrowed.\n";
                continue;
            }
            unsigned int d, m, y;
            cout << "Give today's date in format YYYY MM DD: ";
            cin >> y >> m >> d;
            if (!good_date(y, m, d))
            {
                cout << "Wrong date format.\n";
                continue;
            }
            unsigned int d2, m2, y2;
            cout << "Give deadline's date in format YYYY MM DD: ";
            cin >> y2 >> m2 >> d2;
            if (!good_date(y2, m2, d2))
            {
                cout << "Wrong date format.\n";
                continue;
            }

            if (!Date({y2, m2, d2}).laterThan(Date({y, m, d})))
            {
                cout << "Wrong deadline date.\n";
                continue;
            }
            Book book = library.retrieveBook(book_id);
            library.getUsers()[user_id].getCardPtr()->borrowABook(book, Date({y, m, d}), Date({y2, m2, d2}));
            cout << "Transaction successful.\n";
            continue;
        }
        if (command == "returnbook")
        { // transakcja zwrotu egzemplarza do biblioteki
            unsigned int user_id, book_id;
            cout << "Input your userID number: ";
            cin >> user_id;
            cout << "Input bookID: ";
            cin >> book_id;
            if (!library.getUsers().count(user_id))
            {
                cout << "This userID does not exist.\n";
                continue;
            }
            User &u = library.getUsers()[user_id];
            if (!u.doesHaveCard())
            {
                cout << "This user does not have a library card.\n";
                continue;
            }
            if (!u.getCardPtr()->getBorrowedBooks().count(book_id))
            {
                cout << "This book has not been borrowed by this user.\n";
                continue;
            }
            unsigned int d, m, y;
            cout << "Give today's date in format YYYY MM DD: ";
            cin >> y >> m >> d;
            if (!good_date(y, m, d))
            {
                cout << "Wrong date format.\n";
                continue;
            }
            pair<Book, bool> p = u.getCardPtr()->returnABook(book_id, Date({y, m, d}));
            library.putBackBook(p.first);
            bool is_overtime = p.second;
            if (p.second)
            {
                cout << "User has missed the deadline - how much should they be fined?: ";
                unsigned int fine_value;
                cin >> fine_value;
                u.getCardPtr()->applyFine(Fine(fine_value));
            }
            cout << "Book has been successfully returned to the library.\n";
            continue;
        }
        if (command == "userinfo")
        { // wypisuje info o użytkowniku
            unsigned int user_id;
            cout << "Input your userID number: ";
            cin >> user_id;
            if (!library.getUsers().count(user_id))
            {
                cout << "This userID does not exist.\n";
                continue;
            }
            cout << library.getUsers()[user_id];
            continue;
        }
        if (command == "borrowedbooks")
        { // wypisuje wypożyczone książki na kartę biblioteczną konkretnego czytelnika
            unsigned int user_id;
            cout << "Input your userID number: ";
            cin >> user_id;
            if (!library.getUsers().count(user_id))
            {
                cout << "This userID does not exist.\n";
                continue;
            }
            User &u = library.getUsers()[user_id];
            if (!u.doesHaveCard())
            {
                cout << "This user does not have a library card.\n";
                continue;
            }
            for (auto p : u.getCardPtr()->getBorrowedBooks())
                cout << p.second;
            continue;
        }
        if (command == "balance")
        { // obecne saldo karty bibliotecznej czytelnika - to je się obciąża karą za nie książki w terminie
            unsigned int user_id;
            cout << "Input your userID number: ";
            cin >> user_id;
            if (!library.getUsers().count(user_id))
            {
                cout << "This userID does not exist.\n";
                continue;
            }
            User &u = library.getUsers()[user_id];
            if (!u.doesHaveCard())
            {
                cout << "This user does not have a library card.\n";
                continue;
            }
            const vector<Fine> &fine_v = u.getCardPtr()->getFines();
            unsigned int total_fine_value = 0;
            for (Fine f : fine_v)
                total_fine_value += f.getFineValue();
            cout << "Fines in total: " << total_fine_value << "$"
                 << "\n";
            continue;
        }
        if (command == "lsfines")
        { // wylistowuje wszystkie naliczone kary na podaną kartę
            unsigned int user_id;
            cout << "Input your userID number: ";
            cin >> user_id;
            if (!library.getUsers().count(user_id))
            {
                cout << "This userID does not exist.\n";
                continue;
            }
            User &u = library.getUsers()[user_id];
            if (!u.doesHaveCard())
            {
                cout << "This user does not have a library card.\n";
                continue;
            }
            const vector<Fine> &fine_v = u.getCardPtr()->getFines();
            unsigned int total_fine_value = 0;
            for (Fine f : fine_v)
                cout << f.getFineValue() << "$ ";
            cout << "\n";
            continue;
        }
        if (command == "payfine")
        { // spłać którąś z kar naliczonych dla tego użytkownika
            unsigned int user_id;
            cout << "Input your userID number: ";
            cin >> user_id;
            if (!library.getUsers().count(user_id))
            {
                cout << "This userID does not exist.\n";
                continue;
            }
            User &u = library.getUsers()[user_id];
            if (!u.doesHaveCard())
            {
                cout << "This user does not have a library card.\n";
                continue;
            }
            unsigned int money;
            cout << "How much money would you like to pay?: ";
            cin >> money;
            u.getCardPtr()->payFine(money);
            continue;
        }
        if (command == "bookinfo")
        { // zwraca info o książce o podanym ID
            unsigned int book_id;
            cout << "Input bookID: ";
            cin >> book_id;
            if (!library.getBookCollection().count(book_id))
            {
                cout << "This book does not exist or has already been borrowed.\n";
                continue;
            }
            cout << library.bookInfo(book_id);
            continue;
        }
        cout << "Unrecognized command \n";
    }
    return 0;
}