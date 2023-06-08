#include <bits/stdc++.h>
#include "librarian.h"

using namespace std;

const map<DayOfTheWeek, pair<TimeOfTheDay, TimeOfTheDay>> &Librarian::getSchedule()
{
    return schedule;
}
void Librarian::changeWorkHours(DayOfTheWeek day_of_the_week, TimeOfTheDay start, TimeOfTheDay end)
{
    if (!end.laterThan(start))
        throw runtime_error("Schedule time end is not later than start.");
    schedule[day_of_the_week] = {start, end};
}
void Librarian::removeWorkHours(DayOfTheWeek day_of_the_week)
{
    schedule.erase(day_of_the_week);
}