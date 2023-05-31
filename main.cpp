#include <bits/stdc++.h>
using namespace std;

struct Date
{
    unsigned int year, month, day;
    bool laterThan(Date other_date)
    {
        if (year != other_date.year)
            return year > other_date.year;
        if (month != other_date.month)
            return month > other_date.month;
        return day > other_date.day;
    }
};

struct TimeOfTheDay
{
    unsigned int hour, minute;
    bool laterThan(TimeOfTheDay other)
    {
        if (hour != other.hour)
            return hour > other.hour;
        return minute > other.minute;
    }
};

class Fine // kara pieniężna przypisana do karty bibliotecznej czytelnika
{
    const unsigned int fine_value; // to powoduje, że nie da się zmienić wartości kary albo spłacić jej części
public:
    Fine(unsigned int _fine_value) : fine_value(_fine_value) {} // delegate constructor
    unsigned int const getFineValue()
    {
        // const wyraźnie wskazuje, że ta metoda nie zmienia obiektu
        return fine_value;
    }
};

class Transaction // obiekt transakcji między użytkownikiem a biblioteką
{
protected: // to robi, że pola tej klasy są widoczne dla podklas Transaction
    const Date date;
    const unsigned int card_id, book_id;

public:
    Transaction(Date _date, unsigned int _card_id, unsigned int _book_id) : date(_date), card_id(_card_id), book_id(_book_id) {}
    Date getDate()
    {
        return date;
    }
    unsigned int getCardPtrId()
    {
        return card_id;
    }
    unsigned int getBookId()
    {
        return book_id;
    }
};

class ReturnTransaction : public Transaction
{
    const bool overtime; // informacja o tym, czy ktoś za długo zwlekał z oddaniem książki
public:
    ReturnTransaction(Date _date, unsigned int _card_id, unsigned int _book_id, bool _overtime) : Transaction(_date, _card_id, _book_id), overtime(_overtime) {}
    bool isOvertime()
    {
        return overtime;
    }
};

class BorrowTransaction : public Transaction
{
    Date return_deadline; // to jest informacja, do kiedy książka powinna zostać zwrócona
public:
    BorrowTransaction(Date _date, unsigned int _card_id, unsigned int _book_id, Date _return_deadline) : Transaction(_date, _card_id, _book_id),
                                                                                                         return_deadline(_return_deadline) {}

    Date getReturnDeadline()
    {
        return return_deadline;
    }
    void extendDeadline(Date new_deadline)
    {
        return_deadline = new_deadline;
    }
    ReturnTransaction createReturnTransaction(Date return_date) // automatyczne generowanie transakcji zwrotu opartej na transakcji wypopożyczenia
    {
        return ReturnTransaction(return_date, card_id, book_id, return_date.laterThan(return_deadline));
    }
};

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

    const vector<Transaction> &getTransactions()
    {
        // to może być baaaardzo duży wektor, dlatego zwracam referencję do niego
        return transactions;
    }
    unsigned int getId()
    {
        return id;
    }
    void borrowBook(BorrowTransaction transaction)
    {
        if (is_borrowed)
            throw runtime_error("Book borrowed twice.");
        Transaction *transaction_ptr = dynamic_cast<Transaction *>(&transaction);
        transactions.push_back(*transaction_ptr);
        is_borrowed = true;
        lastBorrowTransaction.emplace_back(transaction);
    }

    void returnBook(ReturnTransaction transaction)
    {
        if (!is_borrowed)
            throw runtime_error("Book has not been borrowed.");
        Transaction *transaction_ptr = dynamic_cast<Transaction *>(&transaction);
        transactions.push_back(*transaction_ptr);
        is_borrowed = false;
        lastBorrowTransaction.clear();
    }

    string getTitle()
    {
        return title;
    }
    string getAuthor()
    {
        return author;
    }
    string getISBN()
    {
        return ISBN;
    }
    bool isBorrowed()
    {
        return is_borrowed;
    }
    BorrowTransaction getLastBorrowTransaction()
    {
        return lastBorrowTransaction.back();
    }
};
unsigned int Book::nextFreeBookId = 0;

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
    void borrowABook(Book book, Date today, Date deadline)
    {
        if (book.isBorrowed())
            throw runtime_error("Book has already been borrowed.");
        BorrowTransaction transaction = BorrowTransaction(today, id, book.getId(), deadline);
        book.borrowBook(transaction);
        transactions.push_back(transaction);
        borrowed_books.emplace(book.getId(), book); // emplace konstruuje obiekt typu Book w miejscu - bez kopiowania
    }
    pair<Book, bool> returnABook(unsigned int book_id, Date today)
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
    unsigned int getId() const
    {
        return id;
    }
    Date getStartDate()
    {
        return start_date;
    }
    const vector<Transaction> &getTransactions()
    {
        return transactions;
    }
    const map<unsigned int, Book> &getBorrowedBooks() const
    {
        return borrowed_books;
    }
    void applyFine(Fine f)
    {
        fines.push_back(f);
    }
    const vector<Fine> &getFines()
    {
        return fines;
    }
    string getName()
    {
        return name;
    }
    string getSurname()
    {
        return surname;
    }
};
unsigned int LibraryCard::nextFreeId = 0;

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
    string getName()
    {
        return name;
    }
    string getSurname()
    {
        return surname;
    }
    unsigned int getUserId()
    {
        return user_id;
    }
    LibraryCard *getCardPtr()
    {
        if (!has_card)
            throw runtime_error("This user's card is inactive - cannot get card info.");
        return card;
    }
    bool doesHaveCard()
    {
        return has_card;
    }
    void deleteCard()
    {
        has_card = false;
        delete card;
    }
    void addCard(Date today)
    {
        if (has_card)
            throw runtime_error("User already has a card.");
        has_card = true;
        card = new LibraryCard(
            name,
            surname,
            today);
    }
};
unsigned int User::nextFreeUserId = 0;

enum DayOfTheWeek
{
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
    SUNDAY
};

const string daysOfTheWeek[7] =
    {
        "MONDAY",
        "TUESDAY",
        "WEDNESDAY",
        "THURSDAY",
        "FRIDAY",
        "SATURDAY",
        "SUNDAY"};

class Librarian : public User
{
    map<DayOfTheWeek, pair<TimeOfTheDay, TimeOfTheDay>> schedule; // godziny pracy w tygodniu dla danego pracownika
public:
    Librarian(string _name, string _surname, map<DayOfTheWeek, pair<TimeOfTheDay, TimeOfTheDay>> _schedule)
        : User(_name, _surname), schedule(_schedule) {}

    const map<DayOfTheWeek, pair<TimeOfTheDay, TimeOfTheDay>> &getSchedule()
    {
        return schedule;
    }
    void changeWorkHours(DayOfTheWeek day_of_the_week, TimeOfTheDay start, TimeOfTheDay end)
    {
        if (!end.laterThan(start))
            throw runtime_error("Schedule time end is not later than start.");
        schedule[day_of_the_week] = {start, end};
    }
    void removeWorkHours(DayOfTheWeek day_of_the_week)
    {
        schedule.erase(day_of_the_week);
    }
};

class Library
{
    map<unsigned int, User> users;           // baza użytkowników
    map<unsigned int, Librarian> librarians; // bibliotekarze
    map<unsigned int, Book> book_collection; // księgozbiór
public:
    Library() {}

    void extendBookCollection(Book new_book)
    {
        if (book_collection.count(new_book.getId()))
            throw runtime_error("There is already book with this ID.");
        book_collection.emplace(new_book.getId(), new_book);
    }
    void removeBook(unsigned int book_id)
    {
        if (!book_collection.count(book_id))
            throw runtime_error("There's no book with this ID.");
        book_collection.erase(book_id);
    }
    void hireNewLibrarian(Librarian new_librarian)
    {
        if (librarians.count(new_librarian.getUserId()))
            throw runtime_error("There is already hired librarian with this ID.");
        librarians.emplace(new_librarian.getUserId(), new_librarian);
    }
    void fireLibrarian(unsigned int user_id)
    {
        if (!librarians.count(user_id))
            throw runtime_error("There's no librarian with this ID.");
        librarians.erase(user_id);
    }
    void addNewUser(User new_user)
    {
        if (users.count(new_user.getUserId()))
            throw runtime_error("There is already registered user with this ID.");
        users.emplace(new_user.getUserId(), new_user);
    }
    void deleteUser(unsigned int user_id)
    {
        if (!users.count(user_id))
            throw runtime_error("There is no registered user with this ID.");
        users[user_id].deleteCard();
        users.erase(user_id);
    }
    void createCard(unsigned int user_id, Date date)
    {
        if (!users.count(user_id))
            throw runtime_error("There is no registered user with this ID.");
        users[user_id].addCard(date);
    }
    map<unsigned int, User> &getUsers()
    {
        return users;
    }
    map<unsigned int, Librarian> &getLibrarians()
    {
        return librarians;
    }
    const map<unsigned int, Book> &getBookCollection()
    {
        return book_collection;
    }
    Book retrieveBook(unsigned int book_id)
    {
        if (!book_collection.count(book_id))
            throw runtime_error("There is no book with this ID.");
        Book book = book_collection[book_id];
        book_collection.erase(book_id);
        return book;
    }
    void putBackBook(Book book)
    {
        book_collection.emplace(book.getId(), book);
    }
};

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
            cout << "Available commands: exit, help, lsbooks, lsusers, addBook,\n deleteBook, addUser, deleteUser, hireLibrarian, fireLibrarian, \n createCard, borrowBook, returnBook, userInfo, borrowedBooks, balance\n";
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
        if (command == "addbook")
        { // dodanie książki do księgozbioru
            string title, author, isbn;
            cout << "Give a title, author and ISBN: ";
            cin >> title >> author >> isbn;
            library.extendBookCollection(Book(title, author, isbn));
            cout << "\n";
            continue;
        }
        if (command == "deletebook")
        {// usunięcie egzemplaża z księgozbioru biblioteki
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
            continue;
        }
        if (command == "adduser")
        {// dodanie nowego użytkownika
            string name, surname;
            cout << "Give a name and surname: ";
            cin >> name >> surname;
            library.addNewUser(User(name, surname));
            cout << "User was registered successfully.\n";
            cout << "\n";
            continue;
        }
        if (command == "deleteuser")
        {// usunięcie użytkownika z bazy czytelników
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
        {// zatrudnienie nowego bibliotekarza
            string name, surname;
            map<DayOfTheWeek, pair<TimeOfTheDay, TimeOfTheDay>> schedule;
            cout << "Give a name and surname: ";
            cin >> name >> surname;
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
                    cout << "Give hh mm of end: ";
                    cin >> endh >> endm;
                    schedule[DayOfTheWeek(i)] = {TimeOfTheDay({starth, startm}), TimeOfTheDay({endh, endm})};
                }
                continue;
            }
            library.hireNewLibrarian(Librarian(name, surname, schedule));
            cout << "New librarian was hired successfully.\n";
            cout << "\n";
            continue;
        }
        if (command == "firelibrarian")
        {// zwolnienie bibliotekarza
            unsigned int user_id;
            cout << "Give a UserID: ";
            cin >> user_id;
            if (!library.getLibrarians().count(user_id))
            {
                cout << "There's no librarian with this userID.\n";
                continue;
            }
            library.fireLibrarian(user_id);
            cout << "Librarian with id: " << user_id << " was fired successfully.\n";
            cout << "\n";
            continue;
        }
        if (command == "createcard")
        {// stworzenie nowej karty dla danego użytkownika
            unsigned int user_id;
            cout << "Give me userID: ";
            cin >> user_id;
            if (!library.getUsers().count(user_id))
            {
                cout << "Invalid userID.";
                continue;
            }
            unsigned int d, m, y;
            cout << "Give today's date in format YYYY MM DD: ";
            cin >> y >> m >> d;
            library.createCard(user_id, Date({y, m, d}));
            cout << "\n";
            continue;
        }
        if (command == "borrowbook")
        {// transakcja wypożyczenia książki
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
            unsigned int d2, m2, y2;
            cout << "Give deadline's date in format YYYY MM DD: ";
            cin >> y2 >> m2 >> d2;
            if (!Date({y2, m2, d2}).laterThan(Date({y, m, d})))
            {
                cout << "Wrong deadline date.\n";
                continue;
            }
            Book book = library.retrieveBook(book_id);
            library.getUsers()[user_id].getCardPtr()->borrowABook(book, Date({y, m, d}), Date({y2, m2, d2}));
            continue;
        }
        if (command == "returnbook")
        {// transakcja zwrotu egzemplarza do biblioteki
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
            continue;
        }
        if (command == "userinfo")
        {// wypisuje info o użytkowniku
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
        {// wypisuje wypożyczone książki na kartę biblioteczną konkretnego czytelnika
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
        {// obecne saldo karty bibliotecznej czytelnika - to je się obciąża karą za nie książki w terminie
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
            cout << "Fines in total: " << total_fine_value << "$" << "\n";
            continue;
        }
        cout << "Unrecognized command \n";
    }
    return 0;
}