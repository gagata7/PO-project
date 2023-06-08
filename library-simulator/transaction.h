#pragma once
#include <bits/stdc++.h>
#include "additional.h"

using namespace std;

class Transaction // Transakcje są między użytkownikiem a biblioteką
{
protected: // to robi, że pola tej klasy są widoczne dla podklas Transaction
    const Date date;
    const unsigned int card_id, book_id;

public:
    Transaction(Date _date, unsigned int _card_id, unsigned int _book_id) : date(_date), card_id(_card_id), book_id(_book_id) {}
    Date getDate();
    unsigned int getCardPtrId();
    unsigned int getBookId();
};

class ReturnTransaction : public Transaction
{
    const bool overtime; // informacja o tym, czy ktoś za długo zwlekał z oddaniem książki
public:
    ReturnTransaction(Date _date, unsigned int _card_id, unsigned int _book_id, bool _overtime) : Transaction(_date, _card_id, _book_id), overtime(_overtime) {}
    bool isOvertime();
};

class BorrowTransaction : public Transaction
{
    Date return_deadline; // to jest informacja, do kiedy książka powinna zostać zwrócona
public:
    BorrowTransaction(Date _date, unsigned int _card_id, unsigned int _book_id, Date _return_deadline) : Transaction(_date, _card_id, _book_id),
                                                                                                         return_deadline(_return_deadline) {}

    Date getReturnDeadline();
    void extendDeadline(Date new_deadline);
    ReturnTransaction createReturnTransaction(Date return_date); // automatyczne generowanie transakcji zwrotu opartej na transakcji wypopożyczenia
};
