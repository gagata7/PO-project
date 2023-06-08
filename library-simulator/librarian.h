#pragma once
#include <bits/stdc++.h>
#include "additional.h"
#include "user.h"
#include "additional.h"

using namespace std;

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
    "SUNDAY"
};

class Librarian : public User
{
    map<DayOfTheWeek, pair<TimeOfTheDay, TimeOfTheDay>> schedule; // godziny pracy w tygodniu dla danego pracownika
public:
    Librarian(string _name, string _surname, map<DayOfTheWeek, pair<TimeOfTheDay, TimeOfTheDay>> _schedule)
        : User(_name, _surname), schedule(_schedule){}

    const map<DayOfTheWeek, pair<TimeOfTheDay, TimeOfTheDay>> &getSchedule();
    void changeWorkHours(DayOfTheWeek day_of_the_week, TimeOfTheDay start, TimeOfTheDay end);
    void removeWorkHours(DayOfTheWeek day_of_the_week);
};