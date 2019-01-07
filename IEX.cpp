/**
*@file IEX.cpp
*@brief This file contains a lot of the API used to gather the data from the IEX online source
*
*This file contains the functions used to gather the data from online
*and the data is usually returned in JSON which are then used by other functions in our program.
*Another important function used in this file is the to check whether the ticker inputted is valid
*
*@author Austin Brandon
*@author Michael Moriyama
*@author Mohammad Niazi
*@author Stephen Kim
*@author Hantao Jiang
*@bug No known bugs.
*/

/*--Include--*/
#include "IEX.h"

/** \brief Method used to parse JSON file
 *  \param IEXdata accesses data using API; symbolVec lists tickers to parse JSON files for. i.e. "AAPL", "GOOGLE"
 *  \return none
 */

void IEX::parseSymbolData(const Json::Value &IEXdata, std::vector<std::string> &symbolVec)
{
    int i = 0;

    while(i < IEXdata.size()) {
        symbolVec.push_back(IEXdata[i]["symbol"].asString());
        i++;
    }
}

/** \brief Method for populating a list of tickers
 *  \param none
 *  \return list of tickers
 */

std::vector<std::string> IEX::getSymbolList()
{
    Json::Value jsonData;
    std::string url(IEX_ENDPOINT);
    std::vector<std::string> symbolList;
    url += "/ref-data/symbols";
    IEX::sendGetRequest(jsonData, url);
    parseSymbolData(jsonData, symbolList);
    return symbolList;

}

/** \brief Method for checking if ticker is valid
 *  \param symbol (e.g. "AAPL")
 *  \return true if valid ticker; false if invalid ticker
 */

bool IEX::isValidSymbol(const std::string &symbol)
{
    std::vector<std::string> symbolList = getSymbolList();
    std::string symbolCopy = symbol;
    boost::to_upper(symbolCopy);
    if (std::find(symbolList.begin(), symbolList.end(), symbolCopy) != symbolList.end())
        return true;

    return false;
}

/** \brief Method used by sendGetRequest to get result from curl
 *  \param
 *  \return size
 */

std::size_t callback(const char* in, std::size_t size, std::size_t num, std::string* out)
{
    const std::size_t totalBytes(size * num);
    out->append(in, totalBytes);
    return totalBytes;
}


/** \brief Method used to send the GET requests
 *  \param url contains url being accessed
 *  \return list of tickers
 *
 * This method used LIBCURL to send the GET requests
 */

void IEX::sendGetRequest(Json::Value &jsonData, const std::string url)
{
    CURL* curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    long int httpCode(0); ///>Having this as a normal int will cause a segmentation fault for some requests being too large
    std::unique_ptr<std::string> httpData(new std::string());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());
    curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
    curl_easy_cleanup(curl);

    Json::Reader jsonReader;
    jsonReader.parse(*httpData, jsonData);
}

 /** \brief Method for getting info on batch of tickers
 *  \param symbol (e.g. "AAPL")
 *  \return list of tickers with their respective info
 *
 * market
 {
 "AAPL" : {
 "quote": {...},
 "news": [...],
 "chart": [...]
 },
 "FB" : {
 "quote": {...},
 "news": [...],
 "chart": [...]
 },
 }
  */

Json::Value IEX::stocks::batch(const std::string &symbol)
{
    Json::Value jsonData;

    if(!isValidSymbol(symbol)){
        std::cout << "Invalid Symbol! I am returning an uninitialized JSON object!";
        return jsonData;
    }

    std::string url(IEX_ENDPOINT);
    url+="/stock/"+symbol+"/batch";
    IEX::sendGetRequest(jsonData, url);
    return jsonData;
}

 /** \brief Method for getting trading book info
 *  \param symbol (e.g. "AAPL")
 *  \return requested info
 *
 * GET /stock/{symbol}/book
 {
 "quote": {...},
 "bids": [...],
 "asks": [...],
 "trades": [...],
 "systemEvent": {...},
 }
  */

Json::Value IEX::stocks::book(const std::string &symbol)
{
    Json::Value jsonData;

    if(!isValidSymbol(symbol)){
        std::cout << "Invalid Symbol! I am returning an uninitialized JSON object!";
        return jsonData;
    }

    std::string url(IEX_ENDPOINT);
    url+="/stock/"+symbol+"/book";
    IEX::sendGetRequest(jsonData, url);
    return jsonData;
}

 /** \brief Method for getting basic company info
 *  \param symbol (e.g. "AAPL")
 *  \return requested info
 *
 *GET /stock/{symbol}/company
 {
 "symbol": "AAPL",
 "companyName": "Apple Inc.",
 "exchange": "Nasdaq Global Select",
 "industry": "Computer Hardware",
 "website": "http://www.apple.com",
 "description": "Apple Inc is an American multinational technology company. It designs, manufactures, and markets mobile communication and media devices, personal computers, and portable digital music players.",
 "CEO": "Timothy D. Cook",
 "issueType": "cs",
 "sector": "Technology",
 }
  */

Json::Value IEX::stocks::company(const std::string &symbol)
{
    Json::Value jsonData;

    if(!isValidSymbol(symbol)){
        std::cout << "Invalid Symbol! I am returning an uninitialized JSON object!";
        return jsonData;
    }

    std::string url(IEX_ENDPOINT);
    url += "/stock/"+symbol+"/company";
    IEX::sendGetRequest(jsonData, url);
    return jsonData;
}

 /** \brief Method for getting delayed quote
  *  \param symbol (e.g. "AAPL")
  *  \return requested info (the quote)
  */

Json::Value IEX::stocks::delayedQuote(const std::string &symbol)
{
    Json::Value jsonData;

    if(!isValidSymbol(symbol)){
        std::cout << "Invalid Symbol! I am returning an uninitialized JSON object!";
        return jsonData;
    }

    std::string url(IEX_ENDPOINT);
    url += "/stock/"+symbol+"/delayed-quote";
    IEX::sendGetRequest(jsonData, url);
    return jsonData;
}

 /** \brief Method for getting important stats for a ticker
  *  \param symbol (e.g. "AAPL")
  *  \return requested info (the stats)
  */

Json::Value IEX::stocks::stats(const std::string &symbol)
{
    Json::Value jsonData;

    if(!isValidSymbol(symbol)){
        std::cout << "Invalid Symbol! I am returning an uninitialized JSON object!";
        return jsonData;
    }

    std::string url(IEX_ENDPOINT);
    url += "/stock/"+symbol+"/stats";
    IEX::sendGetRequest(jsonData, url);
    return jsonData;
}

 /** \brief Method for getting important stats for a ticker
  *  \param symbol (e.g. "AAPL")
  *  \return requested info (the stats)
  */

Json::Value IEX::stocks::statsCompanyName(const std::string &symbol)
{
    Json::Value jsonData;

    if(!isValidSymbol(symbol)){
        std::cout << "Invalid Symbol! I am returning an uninitialized JSON object!";
        return jsonData;
    }

    std::string url(IEX_ENDPOINT);
    url += "/stock/"+symbol+"/stats?filter=companyName"; //add to end for filtering: ?filter=symbol,volume,lastSalePrice
    IEX::sendGetRequest(jsonData, url);
    return jsonData;
}

 /** \brief Method for getting important stats for a ticker
  *  \param symbol (e.g. "AAPL")
  *  \return requested info (the stats)
  */

Json::Value IEX::stocks::statsTicker(const std::string &symbol)
{
    Json::Value jsonData;

    if(!isValidSymbol(symbol)){
        std::cout << "Invalid Symbol! I am returning an uninitialized JSON object!";
        return jsonData;
    }

    std::string url(IEX_ENDPOINT);
    url += "/stock/"+symbol+"/stats?filter=symbol"; //add to end for filtering: ?filter=symbol,volume,lastSalePrice
    IEX::sendGetRequest(jsonData, url);
    return jsonData;
}

 /** \brief Method for getting important stats for a ticker
  *  \param symbol (e.g. "AAPL")
  *  \return requested info (the stats)
  */

Json::Value IEX::stocks::statsSector(const std::string &symbol)
{
    Json::Value jsonData;

    if(!isValidSymbol(symbol)){
        std::cout << "Invalid Symbol! I am returning an uninitialized JSON object!";
        return jsonData;
    }

    std::string url(IEX_ENDPOINT);
    url += "/stock/"+symbol+"/company?filter=sector"; //add to end for filtering: ?filter=symbol,volume,lastSalePrice
    IEX::sendGetRequest(jsonData, url);
    return jsonData;
}

 /** \brief Method for getting important stats for a ticker
  *  \param symbol (e.g. "AAPL")
  *  \return requested info (the stats)
  */

Json::Value IEX::stocks::statsPrice(const std::string &symbol)
{
    Json::Value jsonData;

    if(!isValidSymbol(symbol)){
        std::cout << "Invalid Symbol! I am returning an uninitialized JSON object!";
        return jsonData;
    }

    std::string url(IEX_ENDPOINT);
    url += "/stock/"+symbol+"/stats?filter=lastSalePrice"; //add to end for filtering: ?filter=symbol,volume,lastSalePrice
    IEX::sendGetRequest(jsonData, url);
    return jsonData;
}

 /** \brief Method for getting important stats for a ticker
  *  \param symbol (e.g. "AAPL")
  *  \return requested info (the stats)
  */

Json::Value IEX::stocks::statsMrktCap(const std::string &symbol)
{
    Json::Value jsonData;

    if(!isValidSymbol(symbol)){
        std::cout << "Invalid Symbol! I am returning an uninitialized JSON object!";
        return jsonData;
    }

    std::string url(IEX_ENDPOINT);
    url += "/stock/"+symbol+"/stats?filter=marketcap"; //add to end for filtering: ?filter=symbol,volume,lastSalePrice
    IEX::sendGetRequest(jsonData, url);
    return jsonData;
}

 /** \brief Method for getting important stats for a ticker
  *  \param symbol (e.g. "AAPL")
  *  \return requested info (the stats)
  */

Json::Value IEX::stocks::statsVolume(const std::string &symbol)
{
    Json::Value jsonData;

    if(!isValidSymbol(symbol)){
        std::cout << "Invalid Symbol! I am returning an uninitialized JSON object!";
        return jsonData;
    }

    std::string url(IEX_ENDPOINT);
    url += "/stock/"+symbol+"/stats?filter=volume"; //add to end for filtering: ?filter=symbol,volume,lastSalePrice
    IEX::sendGetRequest(jsonData, url);
    return jsonData;
}

 /** \brief Method for getting important stats for a ticker
  *  \param symbol (e.g. "AAPL")
  *  \return requested info (the stats)
  */

Json::Value IEX::stocks::statsPE(const std::string &symbol)
{
    Json::Value jsonData;

    if(!isValidSymbol(symbol)){
        std::cout << "Invalid Symbol! I am returning an uninitialized JSON object!";
        return jsonData;
    }

    std::string url(IEX_ENDPOINT);
    url += "/stock/"+symbol+"/stats?filter=peRatioHigh"; //add to end for filtering: ?filter=symbol,volume,lastSalePrice
    IEX::sendGetRequest(jsonData, url);
    return jsonData;
}

 /** \brief Method for getting important stats for a ticker
  *  \param symbol (e.g. "AAPL")
  *  \return requested info (the stats)
  */

Json::Value IEX::stocks::statsDivY(const std::string &symbol)
{
    Json::Value jsonData;

    if(!isValidSymbol(symbol)){
        std::cout << "Invalid Symbol! I am returning an uninitialized JSON object!";
        return jsonData;
    }

    std::string url(IEX_ENDPOINT);
    url += "/stock/"+symbol+"/stats?filter=dividendRate"; //add to end for filtering: ?filter=symbol,volume,lastSalePrice
    IEX::sendGetRequest(jsonData, url);
    return jsonData;
}

 /** \brief Method for getting important stats for a ticker
  *  \param symbol (e.g. "AAPL")
  *  \return requested info (the stats)
  */

Json::Value IEX::stocks::stats52High(const std::string &symbol)
{
    Json::Value jsonData;

    if(!isValidSymbol(symbol)){
        std::cout << "Invalid Symbol! I am returning an uninitialized JSON object!";
        return jsonData;
    }

    std::string url(IEX_ENDPOINT);
    url += "/stock/"+symbol+"/stats?filter=week52high"; //add to end for filtering: ?filter=symbol,volume,lastSalePrice
    IEX::sendGetRequest(jsonData, url);
    return jsonData;
}

 /** \brief Method for getting important stats for a ticker
  *  \param symbol (e.g. "AAPL")
  *  \return requested info (the stats)
  */

Json::Value IEX::stocks::stats52Low(const std::string &symbol)
{
    Json::Value jsonData;

    if(!isValidSymbol(symbol)){
        std::cout << "Invalid Symbol! I am returning an uninitialized JSON object!";
        return jsonData;
    }

    std::string url(IEX_ENDPOINT);
    url += "/stock/"+symbol+"/stats?filter=week52low"; //add to end for filtering: ?filter=symbol,volume,lastSalePrice
    IEX::sendGetRequest(jsonData, url);
    return jsonData;
}

 /** \brief Method for getting largest trades
  *  \param symbol (e.g. "AAPL")
  *  \return requested info (the trade info)
  */

Json::Value IEX::stocks::largestTrades(const std::string &symbol)
{
    Json::Value jsonData;

    if(!isValidSymbol(symbol)){
        std::cout << "Invalid Symbol! I am returning an uninitialized JSON object!";
        return jsonData;
    }

    std::string url(IEX_ENDPOINT);
    url += "/stock/"+symbol+"/largest-trades";
    IEX::sendGetRequest(jsonData, url);
    return jsonData;
}

 /** \brief Method for getting a list of common company stats
  *  \param symbol (e.g. "AAPL")
  *  \return requested info (the list)
  */

Json::Value IEX::stocks::list(const std::string &listType)
{
    Json::Value jsonData;
    if(listType == "mostactive" || listType == "gainers" || listType == "losers" || listType == "iexvolume" || listType == "iexmarketpercent") {
        std::string url(IEX_ENDPOINT);
        url+="/stock/market/list/"+listType;
        IEX::sendGetRequest(jsonData, url);
    }
    else{
        std::cout << std::endl << "Incorrect 'listType' input in function list(). I am returning an uninitialized JSON object!" << std::endl;
        return jsonData;
    }
    return jsonData;
}

/** \brief Method for getting news about a specific ticker
 *  \param symbol (e.g. "AAPL")
 *  \return requested info (the news)
 */

Json::Value IEX::stocks::news(const std::string &symbol, int last)
{
    Json::Value jsonData;

    if(!isValidSymbol(symbol)){
        std::cout << "Invalid Symbol! I am returning an uninitialized JSON object!";
        return jsonData;
    }

    std::string url(IEX_ENDPOINT);
    last == 0 ? url += "/stock/"+symbol+"/news" : url += "/stock/"+symbol+"/news/last/"+std::to_string(last);
    IEX::sendGetRequest(jsonData, url);
    return jsonData;
}

/** \brief Method for getting price
 *  \param symbol (e.g. "AAPL")
 *  \return requested info (the price)
 */

Json::Value IEX::stocks::price(const std::string &symbol)
{
    Json::Value jsonData;

    if(!isValidSymbol(symbol)){
        std::cout << "Invalid Symbol! I am returning an uninitialized JSON object!";
        return jsonData;
    }

    std::string url(IEX_ENDPOINT);
    url += "/stock/"+symbol+"/price";
    IEX::sendGetRequest(jsonData, url);
    return jsonData;
}

/** \brief Method for getting pricing info
 *  \param symbol (e.g. "AAPL")
 *  \return requested info (the quote)
 */

Json::Value IEX::stocks::quote(const std::string &symbol, bool displayPercent)
{
    Json::Value jsonData;

    if(!isValidSymbol(symbol)){
        std::cout << "Invalid Symbol! I am returning an uninitialized JSON object!";
        return jsonData;
    }

    std::string url(IEX_ENDPOINT);
    displayPercent ? url += "/stock/"+symbol+"/quote?displayPercent=true" : url += "/stock/"+symbol+"/quote";
    IEX::sendGetRequest(jsonData, url);
    return jsonData;
}

 /** \brief Method for getting info about a specific exchange
 *  \param symbol, name of exchange or stock (e.g. "AAPL")
 *  \return requested info
 */

Json::Value IEX::stocks::VolumeByVenue(const std::string &symbol)
{
    Json::Value jsonData;

    if(!isValidSymbol(symbol)){
        std::cout << "Invalid Symbol! I am returning an uninitialized JSON object!";
        return jsonData;
    }

    std::string url(IEX_ENDPOINT);
    url += "/stock/"+symbol+"/volume-by-venue";
    IEX::sendGetRequest(jsonData, url);
    return jsonData;
}


/// grabs symbols for seleted tickers

Json::Value IEX::refData::symbols()
{
    Json::Value jsonData;
    std::string url(IEX_ENDPOINT);
    url += "/ref-data/symbols";
    IEX::sendGetRequest(jsonData, url);
    return jsonData;
}

/** \brief Method for getting corporate actions
 *  \param date, the date for which you want to grab actions for
 *  \return requested info (any corporate actions for corresponding date)
 */

Json::Value IEX::refData::corporateActions(std::string date)
{
    Json::Value jsonData;
    std::string url(IEX_ENDPOINT);

    if(!date.empty()) {
        if(date.size() == 8) {
            url += "/ref-data/daily-list/corporate-actions/"+date;
            IEX::sendGetRequest(jsonData, url);
        }
        else{
            std::cout << std::endl << "Incorrect 'date' input in function chartDate. Exiting." << std::endl;
            exit(1);
        }
    }

    else{
        url += "/ref-data/daily-list/corporate-actions";
        IEX::sendGetRequest(jsonData, url);
    }
    assert(jsonData.isArray()); //Crash if not an array
    return jsonData;
}

/** \brief Method for getting dividends data
 *  \param date
 *  \return requested info (dividends data)
 */

Json::Value IEX::refData::dividends(std::string date)
{
    Json::Value jsonData;
    std::string url(IEX_ENDPOINT);
    if(!date.empty()) {
        if(date.size() == 8) {
            url += "/ref-data/daily-list/dividends/"+date;
            IEX::sendGetRequest(jsonData, url);
        }
        else{
            std::cout << std::endl << "Incorrect 'date' input in function chartDate. Exiting." << std::endl;
            exit(1);
        }
    }

    else{
        url += "/ref-data/daily-list/dividends";
        IEX::sendGetRequest(jsonData, url);
    }
    return jsonData;
}

/** \brief Method for getting any coming dividend activity
 *  \param date
 *  \return requested info
 */

Json::Value IEX::refData::nextDayExDate(std::string date)
{
    Json::Value jsonData;
    std::string url(IEX_ENDPOINT);
    if(!date.empty()) {
        if(date.size() == 8) {
            url += "/ref-data/daily-list/next-day-ex-date/"+date;
            IEX::sendGetRequest(jsonData, url);
        }
        else{
            std::cout << std::endl << "Incorrect 'date' input in function chartDate. Exiting." << std::endl;
            exit(1);
        }
    }

    else{
        url += "/ref-data/daily-list/next-day-ex-date";
        IEX::sendGetRequest(jsonData, url);
    }
    return jsonData;
}

/** \brief Method for getting a list of all IEX listed securites
 *  \param date, the date format you want your data in
 *  \return requested info
 */
Json::Value IEX::refData::symbolDirectory(std::string date)
{
    Json::Value jsonData;
    std::string url(IEX_ENDPOINT);
    if(!date.empty()) {
        if(date.size() == 8) {
            url += "/ref-data/daily-list/symbol-directory/"+date;
            IEX::sendGetRequest(jsonData, url);
        }
        else{
            std::cout << std::endl << "Incorrect 'date' input in function chartDate. Exiting." << std::endl;
            exit(1);
        }
    }

    else{
        url += "/ref-data/daily-list/symbol-directory";
        IEX::sendGetRequest(jsonData, url);
    }
    return jsonData;
}

/// Method for getting intraday trading volume
Json::Value IEX::stats::intraday()
{
    Json::Value jsonData;
    std::string url(IEX_ENDPOINT);
    url += "/stats/intraday";
    IEX::sendGetRequest(jsonData, url);
    return jsonData;
}

/// Method for getting recent market volume
Json::Value IEX::stats::recent()
{
    Json::Value jsonData;
    std::string url(IEX_ENDPOINT);
    url += "/stats/recent";
    IEX::sendGetRequest(jsonData, url);
    return jsonData;
}

/// Method for getting records of volume
Json::Value IEX::stats::records()
{
    Json::Value jsonData;
    std::string url(IEX_ENDPOINT);
    url += "/stats/records";
    IEX::sendGetRequest(jsonData, url);
    return jsonData;
}

/** \brief Method for getting historical pricing data
 *  \param date, the date format you want your data to be displayed in
 *  \return requested info (the historicals)
 */

Json::Value IEX::stats::historical(std::string date)
{
    Json::Value jsonData;
    std::string url(IEX_ENDPOINT);
    if(!date.empty()) {
        if(date.size() == 8) {
            url += "/stats/historical/"+date;
            IEX::sendGetRequest(jsonData, url);
        }
        else{
            std::cout << std::endl << "Incorrect 'date' input in function chartDate. Exiting." << std::endl;
            exit(1);
        }
    }

    else{
        url += "/stats/historical/";
        IEX::sendGetRequest(jsonData, url);
    }
    return jsonData;
}

/** \brief Method for getting historical daily volume and pricing
 *  \param date, the date format you wnt your data in
 *  \return requested info
 */

Json::Value IEX::stats::historicalDaily(std::string date)
{
    Json::Value jsonData;
    std::string url(IEX_ENDPOINT);
    std::locale loc; ///< For isdigit
    if(!date.empty()) {
        if(date.size() == 8) {
            url += "/stats/historical/daily?date="+date;
            IEX::sendGetRequest(jsonData, url);
        }
        else if(isdigit(date[0],loc)) {
            url += "/stats/historical/daily?last="+date;
            IEX::sendGetRequest(jsonData, url);
        }
        else{
            std::cout << std::endl << "Incorrect 'date' input in function chartDate. Exiting." << std::endl;
            exit(1);
        }
    }

    else{
        url += "/stats/historical/daily";
        IEX::sendGetRequest(jsonData, url);
    }
    return jsonData;
}

/// Method for getting data on specific markets rather than singular tickers
Json::Value IEX::markets::market()
{
    Json::Value jsonData;
    std::string url(IEX_ENDPOINT);
    url += "/market";
    IEX::sendGetRequest(jsonData, url);
    return jsonData;
}
