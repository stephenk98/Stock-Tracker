/**
*@file IEX.cpp
*@brief This file contains a lot of the API used to gather the data from the IEX online source
*
*This is the header file of the IEX.cpp file which is used mainly to gather data from the IEX online source
*
*@author Austin Brandon
*@author Michael Moriyama
*@author Mohammad Niazi
*@author Stephen Kim
*@author Hantao Jiang
*@bug No known bugs.
*/

#ifndef IEX_h
#define IEX_h

#include <jsoncpp/json/json.h>
#include <curl/curl.h>
#include <locale> //std::locale, std::isdigit
#include <string>
#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/algorithm/string.hpp>

#define IEX_ENDPOINT "https://api.iextrading.com/1.0"

namespace IEX {
    void sendGetRequest(Json::Value &data, const std::string url);
    bool isValidSymbol(const std::string &);
    std::vector<std::string> getSymbolList();
    void parseSymbolData(const Json::Value &, std::vector<std::string> &);
    namespace stocks {
        Json::Value batch(const std::string &);
        Json::Value book(const std::string &);
        Json::Value company(const std::string &);
        Json::Value delayedQuote(const std::string &);
        Json::Value stats(const std::string &);
        Json::Value largestTrades(const std::string &);
        Json::Value news(const std::string &, int last = 0);
        Json::Value price(const std::string &);
        Json::Value quote(const std::string &, bool displayPercent = false);
        Json::Value VolumeByVenue(const std::string &);
	Json::Value statsCompanyName(const std::string &);
	Json::Value statsTicker(const std::string &);
    	Json::Value statsSector(const std::string &);
	Json::Value statsPrice(const std::string &);
	Json::Value statsMrktCap(const std::string &);
	Json::Value statsVolume(const std::string &);
	Json::Value statsPE(const std::string &);
	Json::Value statsDivY(const std::string &);
	Json::Value stats52High(const std::string &);
	Json::Value stats52Low(const std::string &);
	Json::Value list(const std::string &listType);

    }

    namespace refData {
        Json::Value symbols();
        Json::Value corporateActions(std::string date = "");
        Json::Value dividends(std::string date = "");
        Json::Value nextDayExDate(std::string date = "");
        Json::Value symbolDirectory(std::string date = "");
    }

    namespace marketData {

    }

    namespace stats {
        Json::Value intraday();
        Json::Value recent();
        Json::Value records();
        Json::Value historical(std::string date = "");
        Json::Value historicalDaily(std::string date = "");
    }

    namespace markets {
        Json::Value market();
    }
}

#endif
