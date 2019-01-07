/**
*@file watchlist.cpp
*@brief This file contains the header for the class watchlist which allows the user to manipulate their watchlist
*
*This file contains many of the functions of the watchlist class as well as
*the necessary ability to create a watchlist object which is then used to
*be populated by stock so we can add and remove these stocks as well as display their information
*
*
*@author Austin Brandon
*@author Michael Moriyama
*@author Mohammad Niazi
*@author Stephen Kim
*@author Hantao Jiang
*@bug No known bugs.
*/

#ifndef WATCHLIST_H
#define WATCHLIST_H

#include <string.h>
#include <stdbool.h>
#include "IEX.h"
#include <iostream>

using namespace std;

class watchlist
{
private:
    char** wList;
    std::string tick;
    int numStocks, firstEmpty;
public:
    watchlist();
    int getNumStocks();
    void addToWatchlist(char*,char**);
    void removeFromWatchlist(char*, char**);
    void displayWatchlistContents(char**);
    void displayData(char*, char**);
    bool inList(char*, char**);
    int getFirstEmptySpace(char**);
};

#endif // WATCHLIST_H
