#include <bits/stdc++.h>
#include "transaction.h"

using namespace std;

Date Transaction::getDate()
{
    return date;
}
unsigned int Transaction::getCardPtrId()
{
    return card_id;
}
unsigned int Transaction::getBookId()
{
    return book_id;
}


bool ReturnTransaction::isOvertime()
{
    return overtime;
}


Date BorrowTransaction::getReturnDeadline()
{
    return return_deadline;
}
void BorrowTransaction::extendDeadline(Date new_deadline)
{
    return_deadline = new_deadline;
}
ReturnTransaction BorrowTransaction::createReturnTransaction(Date return_date) // automatyczne generowanie transakcji zwrotu opartej na transakcji wypopożyczenia
{
    return ReturnTransaction(return_date, card_id, book_id, return_date.laterThan(return_deadline));
}
