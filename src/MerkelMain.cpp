#include "MerkelMain.h"
#include <iostream>
#include <vector>
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include <iomanip>

MerkelMain::MerkelMain()
{

}

void MerkelMain::init()
{
    int input;
    currentTime = orderBook.getEarliestTime();

    wallet.insertCurrency("BTC", 10);

    while(true)
    {
        printMenu();
        input = getUserOption();
        processUserOption(input);
    }
}


void MerkelMain::printMenu()
{
    // 1 print help
    std::cout << "1: Print help " << std::endl;
    // 2 print exchange stats
    std::cout << "2: Print exchange stats" << std::endl;
    // 3 make an offer
    std::cout << "3: Make an offer " << std::endl;
    // 4 make a bid 
    std::cout << "4: Make a bid " << std::endl;
    // 5 print wallet
    std::cout << "5: Print wallet " << std::endl;
    // 6 continue   
    std::cout << "6: Continue " << std::endl;

    std::cout << "7: Print Candlestick table " << std::endl;

    std::cout << "8: Print Candlestick Plot " << std::endl;

    std::cout << "9: Print Other Candlestick Plot " << std::endl;

    std::cout << "============== " << std::endl;

    std::cout << "Current time is: " << currentTime << std::endl;
}

void MerkelMain::printHelp()
{
    std::cout << "Help - your aim is to make money. Analyse the market and make bids and offers. " << std::endl;
}

void MerkelMain::printMarketStats()
{
    for (std::string const& p : orderBook.getKnownProducts())
    {
        std::cout << "Product: " << p << std::endl;
        std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask, 
                                                                p, currentTime);
        std::cout << "Asks seen: " << entries.size() << std::endl;
        std::cout << "Max ask: " << OrderBook::getHighPrice(entries) << std::endl;
        std::cout << "Min ask: " << OrderBook::getLowPrice(entries) << std::endl;



    }
    // std::cout << "OrderBook contains :  " << orders.size() << " entries" << std::endl;
    // unsigned int bids = 0;
    // unsigned int asks = 0;
    // for (OrderBookEntry& e : orders)
    // {
    //     if (e.orderType == OrderBookType::ask)
    //     {
    //         asks ++;
    //     }
    //     if (e.orderType == OrderBookType::bid)
    //     {
    //         bids ++;
    //     }  
    // }    
    // std::cout << "OrderBook asks:  " << asks << " bids:" << bids << std::endl;

}

void MerkelMain::enterAsk()
{
    std::cout << "Make an ask - enter the amount: product,price, amount, eg  ETH/BTC,200,0.5" << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "MerkelMain::enterAsk Bad input! " << input << std::endl;
    }
    else {
        try {
            OrderBookEntry obe = CSVReader::stringsToOBE(
                tokens[1],
                tokens[2], 
                currentTime, 
                tokens[0], 
                OrderBookType::ask 
            );
            obe.username = "simuser";
            if (wallet.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks good. " << std::endl;
                orderBook.insertOrder(obe);
            }
            else {
                std::cout << "Wallet has insufficient funds . " << std::endl;
            }
        }catch (const std::exception& e)
        {
            std::cout << " MerkelMain::enterAsk Bad input " << std::endl;
        }   
    }
}

void MerkelMain::enterBid()
{
    std::cout << "Make an bid - enter the amount: product,price, amount, eg  ETH/BTC,200,0.5" << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "MerkelMain::enterBid Bad input! " << input << std::endl;
    }
    else {
        try {
            OrderBookEntry obe = CSVReader::stringsToOBE(
                tokens[1],
                tokens[2], 
                currentTime, 
                tokens[0], 
                OrderBookType::bid 
            );
            obe.username = "simuser";

            if (wallet.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks good. " << std::endl;
                orderBook.insertOrder(obe);
            }
            else {
                std::cout << "Wallet has insufficient funds . " << std::endl;
            }
        }catch (const std::exception& e)
        {
            std::cout << " MerkelMain::enterBid Bad input " << std::endl;
        }   
    }
}

void MerkelMain::printWallet()
{
    std::cout << wallet.toString() << std::endl;
}
        
void MerkelMain::gotoNextTimeframe()
{
    std::cout << "Going to next time frame. " << std::endl;
    for (std::string p : orderBook.getKnownProducts())
    {
        std::cout << "matching " << p << std::endl;
        std::vector<OrderBookEntry> sales =  orderBook.matchAsksToBids(p, currentTime);
        std::cout << "Sales: " << sales.size() << std::endl;
        for (OrderBookEntry& sale : sales)
        {
            std::cout << "Sale price: " << sale.price << " amount " << sale.amount << std::endl; 
            if (sale.username == "simuser")
            {
                // update the wallet
                wallet.processSale(sale);
            }
        }
        
    }

    currentTime = orderBook.getNextTime(currentTime);
}
 
int MerkelMain::getUserOption()
{
    int userOption = 0;
    std::string line;
    std::cout << "Type in 1-6" << std::endl;
    std::getline(std::cin, line);
    try{
        userOption = std::stoi(line);
    }catch(const std::exception& e)
    {
        // 
    }
    std::cout << "You chose: " << userOption << std::endl;
    return userOption;
}
//all self wrote
//Task 1
void MerkelMain::viewCandlestickData() {
    //getting user input to choose the product and the bid/ask of the product
    //storing the inputs into a variable
    std::cout << "Enter the product for which you want to view candlestick data (eg.ETH/BTC):  ";
    std::string product;
    std::getline(std::cin, product);
    std::cout << "Do you want to view the bid or ask of the product (eg.bid):  ";
    std::string type;
    std::getline(std::cin, type);
    
    // Calculate and display candlestick data
    // parsing the inputs into the calculation function 
    std::vector<Candlestick> candlesticks = orderBook.calculateCandlesticks(product, type);
    std::cout << "Candlestick Data for " << product << ":" << std::endl;
    int counter = 0;
    std::cout << "Timeframe                    " << ""
                  << "Open       "<< ""  
                  << "High       "<< ""
                  << "Low       "<< ""
                  << "Close       "<< ""
                  << "Product   "<< ""
                  << "Type"<< std::endl;
    // looping through the candlestick data to print out all the called data
    for (auto& candlestick : candlesticks) { 
        std::cout << candlestick.getTimeframe() << "  "
                  << candlestick.getOpen() << "  "
                  << candlestick.getHigh() << "  "
                  << candlestick.getLow() << "  "
                  << candlestick.getClose() << "  "
                  << candlestick.getProduct() << "  " 
                  << type << std::endl;

        counter += 1;

    }
    // showing the number of candlestick data 
    std::cout << "Total Candlesticks: " << counter << std::endl;
}

void MerkelMain::printCandlestickPlot() {

    //getting user input to choose the product and the bid/ask of the product
    //storing the inputs into a variable
    std::cout << "Enter the product for which you want to view candlestick data (eg.ETH/BTC):  ";
    std::string product;
    std::getline(std::cin, product);
    std::cout << "Do you want to view the bid or ask of the product (eg.bid):  ";
    std::string type;
    std::getline(std::cin, type);
    // Calculate and display candlestick data
    // parsing the inputs into the calculation function 
    std::vector<Candlestick> candlesticks = orderBook.calculateCandlesticks(product, type);
    orderBook.printCandlestickPlot(candlesticks);
}

void MerkelMain::printAdditionalCandlestickPlot()
{
    //using CSVReader to read the SOL-USD Data and store the variables into the Candlestick vector
    std::vector<Candlestick> Candlesticks = CSVReader::readCandlestickCSV("SOL-USD.csv");
    // loop through the candlestick data to print out the data into a table format
    for(Candlestick candle: Candlesticks){
        std::cout << candle.getTimeframe() << "\t"
                  << candle.getOpen() << "\t"  
                  << candle.getHigh() << "\t"
                  << candle.getLow() << "\t"
                  << candle.getClose() << "\t"
                  << std::endl;
    };
    // printing the candlestick plot 
    orderBook.printCandlestickPlot(Candlesticks);
}
//all self wrote
void MerkelMain::processUserOption(int userOption)
{
    if (userOption == 0) // bad input
    {
        std::cout << "Invalid choice. Choose 1-6" << std::endl;
    }
    if (userOption == 1) 
    {
        printHelp();
    }
    if (userOption == 2) 
    {
        printMarketStats();
    }
    if (userOption == 3) 
    {
        enterAsk();
    }
    if (userOption == 4) 
    {
        enterBid();
    }
    if (userOption == 5) 
    {
        printWallet();
    }
    if (userOption == 6) 
    {
        gotoNextTimeframe();
    }
    if (userOption == 7)
    {
        viewCandlestickData();
    }
    if (userOption == 8)
    {
        printCandlestickPlot();
    }
    if (userOption == 9)
    {
        printAdditionalCandlestickPlot();
    }
}


