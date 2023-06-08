#include <bits/stdc++.h>
#include "fine.h"
#include "additional.h"
using namespace std;

bool leap_year(unsigned int y)
{ // sprawdza, czy rok y jest przestępny
    if ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0)
        return true;
    return false;
}

bool good_date(unsigned int y, unsigned int m, unsigned int d)
{
    if (m == 0 || m > 12)
        return false;
    if (leap_year(y) && m == 2 && d > 29)
        return false;
    if (!leap_year(y) && m == 2 && d > 28)
        return false;
    if (m <= 7)
    {
        if (m % 2 != 0 && d > 31)
            return false;
        if (m % 2 == 0 && d > 30)
            return false;
    }
    else
    {
        if (m % 2 != 0 && d > 30)
            return false;
        if (m % 2 == 0 && d > 31)
            return false;
    }
    return true;
}

    bool Date::laterThan(Date other_date)
    {
        if (year != other_date.year)
            return year > other_date.year;
        if (month != other_date.month)
            return month > other_date.month;
        return day > other_date.day;
    }

bool good_time(unsigned int h, unsigned int m)
{
    if (h > 24 || m > 60)
        return false;
    return true;
}

unsigned int minutes_between(unsigned int h2, unsigned int m2, unsigned int h1, unsigned int m1)
{ // liczy ile minut upłynęło między tymi dwoma godzinami (h2,m2) jest później
    int totalMinutes1 = h1 * 60 + m1;
    int totalMinutes2 = h2 * 60 + m2;
    int difference = totalMinutes2 - totalMinutes1;
    if (difference < 0)
    {
        difference += 24 * 60;
    }
    return difference;
}

    bool TimeOfTheDay::laterThan(TimeOfTheDay other)
    {
        if (hour != other.hour)
            return hour > other.hour;
        return minute > other.minute;
    }
