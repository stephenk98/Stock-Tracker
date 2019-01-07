/**
 *@file DataGrabber.cpp
 *@brief This file drives the program as in includes the main
 *
 *This file contains the main function for the program
 *and the functions that gets the required data from our API.
 *Another important role of this file is to create the watchlist object
 *which is instantiated in this file to store the stock information for users. 
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

#include <iostream>
#include <jsoncpp/json/json.h>
#include <curl/curl.h>
#include "IEX.h"
#include <string>
#include "watchlist.h"
#include <iostream>
#include <string.h>


using namespace std;

/*
*creating the watchlist array to hold the watchlist objects
*/
char* wList[10];
/*
*creating the watchlist object
*/
watchlist* wObj = new watchlist();

/**
 *@brief This function outputs the indexes and their information 
 *
 *Very neat function that simply calls upon the IEX API to output all the indexes as well as their information
 *and there is no need for user input due to the nature of indexes, we want to display as many as possible
 *
 *
 *@author Austin Brandon
 *@author Michael Moriyama
 *@author Mohammad Niazi
 *@author Stephen Kim
 *@author Hantao Jiang
 *@param Void. 
 *@return Void.
*/
void getIndices()
{
	Json::Value dataHolder = IEX::markets::market();
        cout << dataHolder.toStyledString();
}

/**
*@brief This function gets the volume of a stock or index
*
*This function allows the user to choose whether to check the volume of a stock or index.
*After taking in the user input, the function allows the user to input a stock or index 
*of which the volume is then returned. 
*The menu options as well as the ticker are checked for valid input
*but the index does not have any input due to the nature of stock indexes 
*
*@author Austin Brandon
*@author Michael Moriyama
*@author Mohammad Niazi
*@author Stephen Kim
*@author Hantao Jiang
*@param Void.
*@return Void.
*/
void getVol()
{
	int option;
	cout << "Volume Menu: \n";
	cout << "1. Check Volume for Indices\n";
	cout << "2. Check Volume for Stocks\n";
	cout << "Please enter the option number: \n";
	cin >> option;
	while ((option != 1) && (option != 2))
	{
		cout << "You entered an incorrect option number, please try again: \n";
		cin >> option;
	}
	if (option == 1)
	{
		int option2;
		cout << "List of Index Volumes Displayed: \n";
		Json::Value dHolder = IEX::markets::market();
								cout << dHolder.toStyledString();
	}
	else if (option == 2)
	{
		string ticker;
                cout << "Pulling volume for specific companies\n";
		cout << "Enter the ticker: \n";
		cin >> ticker;
		bool validTicker = IEX::isValidSymbol(ticker);
		while (!validTicker)
		{
			cout << "You entered an incorrect ticker, please try again: ";
			cin >> ticker;
			validTicker = IEX::isValidSymbol(ticker);
		}
		Json::Value dHolder = IEX::stocks::VolumeByVenue(ticker);
                cout << dHolder.toStyledString();

	}
}

/**
*@brief This function takes in an user inputed ticker and then outputs the news information for that stock
*
*The user inputs a ticker which is then checked to ensure proper ticker
*which is then passed to the IEX API to pull news information for that stock.
*The number of news articles chosen to output is currently 5.
*Expansion of the program would allow users to dicate the number of news articles
*and may even offer the option to get more news articles after receiving 5
*
*@author Austin Brandon
*@author Michael Moriyama
*@author Mohammad Niazi
*@author Stephen Kim
*@author Hantao Jiang
*@param Void.
*@return Void.
*/
void getNews()
{
	string ticker;
	int numArticles = 5;
	cout << "To pull news for a company, please enter its ticker: \n";
	cin >> ticker;
	bool validTicker = IEX::isValidSymbol(ticker);
	while (!validTicker)
	{
		cout << "You entered an incorrect ticker, please try again: ";
		cin >> ticker;
		validTicker = IEX::isValidSymbol(ticker);
	}
	Json::Value dHolder = IEX::stocks::news(ticker, numArticles);
        cout << dHolder.toStyledString();
}

/**
*@brief This function outputs the stats and financial information of a company given a ticker
*
*This function gets the ticker from the user which is then validated.
*Once the ticker is validated, it is used by the IEX API to return the pertinent 
*stock information for that ticker
*the information is stored in a string array and then outputed
*
*@author Austin Brandon
*@author Michael Moriyama
*@author Mohammad Niazi
*@author Stephen Kim
*@author Hantao Jiang
*@param Void.
*@return Void.
*/
void getSearchResult()
{
	string ticker;
	string infoHolder[10];
	cout << "You can get common stats for companies here. Let's get started.\n";
	cout << "To pull data for a company, please enter its ticker: \n";
	cin >> ticker;
	bool validTicker = IEX::isValidSymbol(ticker);
	while (!validTicker)
	{
		cout << "You entered an incorrect ticker, please try again: ";
		cin >> ticker;
		validTicker = IEX::isValidSymbol(ticker);
	}
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
}

/**
*@brief This is the main function that includes
*    the menu choices that are selected by user which are then run
*
*This is the main function that drives the whole program
*which includes the menu that takes the user's input and
*and then runs the selection that the user provides.
*It also provides the necessary choices for the watchlist user story
*as it checks that the watchlist is not empty and other watchlist conditions
*
*@author Austin Brandon
*@author Michael Moriyama
*@author Mohammad Niazi
*@author Stephen Kim
*@author Hantao Jiang
*@param Void. Main function takes no parameters 
*@return Void. Main function 
*/
int main()
{
	bool flag = true;
	//char* wList[10] = {};
	while(flag) {


		int option;
		cout << "Welcome to the app!\n";
		cout << "To get started, please chose an option from the main menu below.\n";
		cout << "Main Menu: \n";
		cout << "1. Search to View Stats for a Ticker\n";
		cout << "2. News\n";
		cout << "3. Volume by Venue or Volume by Stock\n";
		cout << "4. Indices\n";
		cout << "5. Watchlist\n";
		cout << "6. Exit Program\n";
		cout << "Enter chosen option: \n";
		cin >> option;
		if ((option != 1) && (option != 2) && (option != 3) && (option != 4) && (option != 5) && (option != 6))
		{
			cout << "You entered an incorrect option, please try again: \n";
			cin >> option;
		}
		if (option == 1)
		{
			int opt2;
			getSearchResult();
                        cout << "Would you like to return to the main menu - 1 for yes, 2 for no (1/2): \n";
			cin >> opt2;
			if(opt2==1) {
				continue;
			}
			else {
				flag = false;
			}

		}
		else if (option == 2)
		{
			int opt2;
			getNews();
											cout << "Would you like to return to the main menu - 1 for yes, 2 for no (1/2): \n";
			cin >> opt2;
			if(opt2==1) {
				continue;
			}
			else {
				flag = false;
			}
		}
		else if (option == 3)
		{
			int opt2;
			getVol();
			cout << "Would you like to return to the main menu - 1 for yes, 2 for no (1/2): \n";
			cin >> opt2;
			if(opt2==1) {
				continue;
			}
			else {
				flag = false;
			}
		}
		else if (option == 4)
		{
			int opt2;
			getIndices();
			cout << "Would you like to return to the main menu - 1 for yes, 2 for no (1/2): \n";
			cin >> opt2;
			if(opt2==1) {
				continue;
			}
			else {
				flag = false;
			}
		}
	else if (option == 5)
		{
			int opt2;
			//watchlist* wObj = new watchlist();
			char* tick;
			int woption;
			cout << "Watchlist Menu: " << endl;
			cout << "1. Add stock to watchlist" << endl;
			cout << "2. Remove stock from watchlist" << endl;
			cout << "3. Display watchlist contents" << endl;
			cout << "4. Display watchlist stock data" << endl;
			cout << "Please enter the number of the desired option: " << endl;
			cin >> woption;

			switch(woption) {
					//Adding a stock
					case 1 :
							if(wObj->getNumStocks() < 10) {
									cout << "Please enter a stock to add to the watchlist: " << endl;
									cin >> tick;
								/*	tickCheck = tick;
									bool validTicker = IEX::isValidSymbol(tickCheck);
									while (!validTicker)
									{
										cout << "You entered an incorrect ticker, please try again: ";
										cin >> tickCheck;
										validTicker = IEX::isValidSymbol(tickCheck);
									}*/
									wObj->addToWatchlist(tick,wList);
									wObj->displayWatchlistContents(wList);
							} else {
									cout << "Watchlist is full! Unable to add to watchlist" << endl;
							}
							break;

					//Removing a stock
					case 2 :
							if(wObj->getNumStocks() == 0) {
									cout << "Watchlist is empty! Unable to remove from watchlist" << endl;
							} else {
									cout << "Enter a stock to remove from the watchlist: " << endl;
									cin >> tick;
									wObj->removeFromWatchlist(tick, wList);
									cout << tick << " removed from watchlist" << endl;
									wObj->displayWatchlistContents(wList);
							}
							break;
					//Displaying the watchlist contents
					case 3 :
							if(wObj->getNumStocks() == 0) {
									cout << "Watchlist is empty" << endl;
							} else {
								 wObj->displayWatchlistContents(wList);
							}
							break;
					//Displaying data of a stock in the watchlist
					case 4 :
							if(wObj->getNumStocks() == 0) {
									cout << "Watchlist is empty! No stock data to display" << endl;
							} else {
									cout << "Please enter the stock whose data you want to display: " << endl;
									cin >> tick;
							}
							break;
			}

			cout << "Would you like to return to the main menu - 1 for yes, 2 for no (1/2): \n";
			cin >> opt2;
			if(opt2==1) {
				continue;
			}
			else {
				flag = false;
			}
		}
		else if (option == 6)
		{
			exit(0);
		}
}
}
