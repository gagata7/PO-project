#pragma once
#include "fine.h"
#include <bits/stdc++.h>

using namespace std;

bool leap_year(unsigned int y);

bool good_date(unsigned int y, unsigned int m, unsigned int d);

struct Date
{
    unsigned int year, month, day;
    bool laterThan(Date other_date);
};

bool good_time(unsigned int h, unsigned int m);

unsigned int minutes_between(unsigned int h2, unsigned int m2, unsigned int h1, unsigned int m1);

struct TimeOfTheDay
{
    unsigned int hour, minute;
    bool laterThan(TimeOfTheDay other);
};