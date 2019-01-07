/**
*@file watchlist.cpp
*@brief This file contains the class watchlist allows the user to manipulate their watchlist
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

/*--Includes--*/
#include "watchlist.h"
#include <string.h>
#include <stdbool.h>
#include "IEX.h"
#include <iostream>
#include <sstream>


///Number of stocks in the watchlist
int numStocks;
///Next empty space in watchlist

/**
*@brief This is the constructor for the watchlist class
*
*Simply stores the number of stocks as public
*
*@author Austin Brandon
*@author Michael Moriyama
*@author Mohammad Niazi
*@author Stephen Kim
*@author Hantao Jiang
*@param Void.
*@return Void.
*/
watchlist::watchlist()
{
    numStocks;
}

/**
*@brief This is a helper function that checks the watchlist to find the next empty space
*
*This is a helper function that would iterate through the watchlist array 
*it will then return the first position that does not have a stock already there
*
*@author Austin Brandon
*@author Michael Moriyama
*@author Mohammad Niazi
*@author Stephen Kim
*@author Hantao Jiang
*@param wlist which is pointer to char* which is the array of watchlist
*@return int, which is the position in the array
*/
int watchlist::getFirstEmptySpace(char **wList) {
    for(int i = 0; i < 9; i++) {
        if(wList[i] == NULL) {
            return i;
        }
    }
}

/**
*@brief This is a helper function that checks the watchlist to see if the stock is in the watchlist
*
*The function iterates through the watchlist and if the stock is there, it will set the retun to true
*the check is done using the strcmp as we are comparing strings
*
*@author Austin Brandon
*@author Michael Moriyama
*@author Mohammad Niazi
*@author Stephen Kim
*@author Hantao Jiang
*@param ticker as the ticker that is inputed by teh user
*@param wlist which is pointer to char* which is the array of watchlist
*@return bool which shows whether the stock is in the watchlist or not
*/
bool watchlist::inList(char* ticker, char** wList) {
    bool check = false;
    for(int i = 0; i < 9; i++) {
      if (wList[i] != NULL) {
        if(strcmp(ticker,wList[i]) == 0) {
          check = true;
          break;
        }
      }

    }
    return check;
}


/**
*@brief This function gets the number of stocks in the watchlist
*
*This function returns the number of stocks in the watchlist
*
*@author Austin Brandon
*@author Michael Moriyama
*@author Mohammad Niazi
*@author Stephen Kim
*@author Hantao Jiang
*@param Void.
*@return int, which is the number of stocks 
*/
int watchlist::getNumStocks(){
  return numStocks;
}

/**
*@brief This function will add a stock to the watchlist 
*
*This function will add a stock to the watchlist by first checking if the stock is already in the watchlist
*if the stock is not in the watchlist already, it will be added to the first empty position
*then the number of stocks would be increased to match the added stock
*
*@author Austin Brandon
*@author Michael Moriyama
*@author Mohammad Niazi
*@author Stephen Kim
*@author Hantao Jiang
*@param ticker as the ticker that is inputed by teh user
*@param wlist which is pointer to char* which is the array of watchlist
*@return Void.
*/
void watchlist::addToWatchlist(char* ticker, char** wList) {
  //Case where the stock is already in the watchlist
    int x;
    if(inList(ticker, wList)) {
        cout << "Cannot add! Stock already in watchlist" << endl;

    //Case where the stock is not already in the watchlist
  } else if(!inList(ticker,wList)) {
        //Add stock to watchlist in first empty position
        x = getFirstEmptySpace(wList);
        wList[x] = ticker;
        numStocks = numStocks+1;
        cout << ticker << " added to watchlist" << endl;
        //if (numStocks < 10) {
            //Find the next empty space
            //firstEmpty = getFirstEmptySpace(wList);
        //}

    }
}

/**
*@brief This function will remove a stock from the watchlist 
*
*This function will see if the stock is in the watchlist
*if the stock is in the watchlist, it will remove the stock by setting it to NULL
*then the number of stocks will be decreased to match the removed stock 
*
*@author Austin Brandon
*@author Michael Moriyama
*@author Mohammad Niazi
*@author Stephen Kim
*@author Hantao Jiang
*@param ticker as the ticker that is inputed by teh user
*@param wlist which is pointer to char* which is the array of watchlist
*@return Void.
*/
void watchlist::removeFromWatchlist(char* ticker, char** wList) {
    //Case where the stock is in the watchlist
    if(inList(ticker, wList)) {
        //Search through watchlist for the stock to be removed
        for(int i = 0; i < 9; i++) {
            if(wList[i] == ticker) {
                //Remove stock from watchlist
                wList[i] = NULL;
                break;
            }
        }
        //Decrement number of stocks in watchlist
        numStocks -= 1;
        //Find next empty space
        firstEmpty = getFirstEmptySpace(wList);

    //Case where the stock is not in the watchlist
    } else {
        cout << "Unable to remove! Stock not in watchlist" << endl;
    }
}

/**
*@brief This function displays the contents of the watchlist 
*
*This function checks first to see if there are stocks in the watchlist
*if there are stocks in the watchlist, this function would then output the stocks ouf the watchlist
*this is done using a for loop up to 10 as there can only be 10 stocks at most in the watchlist 
*the function checks if there is less than 10 stocks and would not output if there is less than 10
*
*@author Austin Brandon
*@author Michael Moriyama
*@author Mohammad Niazi
*@author Stephen Kim
*@author Hantao Jiang
*@param wList as a pointer to char* which is the array storing the watchlist objects 
*@return Void.
*/
void watchlist::displayWatchlistContents(char** wList) {
    //Case where there are stocks in the watchlist
    if(numStocks > 0) {
        cout << "Watchlist:" << endl;
        for(int i = 0; i < 10; i++) {
            if (wList[i] != NULL) {
            cout << wList[i] << endl;
            }
        }
    //Case where there are no stocks in the watchlist
    } else {
        cout << "Watchlist empty" << endl;
    }
}

/**
*@brief This function displays the data for a givne stock in the watchlist
*
*This function first checks if the given ticker is in the watchlist 
*if the ticker is in the watchlist, it would then output the stock information
*for that given ticker as the information would be stored in a string array
*then using a for loop, would be outputed 
*@author Austin Brandon
*@author Michael Moriyama
*@author Mohammad Niazi
*@author Stephen Kim
*@author Hantao Jiang
*@param ticker which is the ticker entered by the user
*@param wlist which is pointer to char* which is the array of watchlist
*@return Void.
*/
void watchlist::displayData(char* ticker, char** wList) {
    //Case where the stock is in the watchlist
    if(inList(ticker, wList)) {
        string infoHolder[10];

        Json::Value cTick = IEX::stocks::statsTicker(ticker);
        infoHolder[0] = cTick.toStyledString();

        Json::Value cName = IEX::stocks::statsCompanyName(ticker);
        infoHolder[1] = cName.toStyledString();

        Json::Value cSector = IEX::stocks::statsSector(ticker);
        infoHolder[2] = cSector.toStyledString();

        Json::Value cPrice = IEX::stocks::statsPrice(ticker);
        infoHolder[3] = cPrice.toStyledString();

        Json::Value cHigh = IEX::stocks::stats52High(ticker);
        infoHolder[4] = cHigh.toStyledString();

        Json::Value cLow = IEX::stocks::stats52Low(ticker);
        infoHolder[5] = cLow.toStyledString();

        Json::Value cMktCap = IEX::stocks::statsMrktCap(ticker);
        infoHolder[6] = cMktCap.toStyledString();

        Json::Value cVol = IEX::stocks::statsVolume(ticker);
        infoHolder[7] = cVol.toStyledString();

        Json::Value cDivY = IEX::stocks::statsDivY(ticker);
        infoHolder[8] = cDivY.toStyledString();

        Json::Value cPE = IEX::stocks::statsPE(ticker);
        infoHolder[9] = cPE.toStyledString();

        for (int i = 0; i < 10; i++)
        {
                cout << infoHolder[i];
        }
    //Case where the stock is not in the watchlist
    } else {
        cout << "Unable to display data! Stock is not in the watchlist!" << endl;
    }
}
