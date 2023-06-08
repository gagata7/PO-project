#pragma once
#include <bits/stdc++.h>
using namespace std;

class Fine // kara pieniężna przypisana do karty bibliotecznej czytelnika
{
    unsigned int fine_value;
public:
    Fine(unsigned int _fine_value) : fine_value(_fine_value) {} // delegate constructor
    unsigned int const getFineValue();
};
