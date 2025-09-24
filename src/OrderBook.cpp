#include "OrderBook.h"
#include "CSVReader.h"
#include <map>
#include <algorithm>
#include <iostream>
#include <cmath>


/** construct, reading a csv data file */
OrderBook::OrderBook(std::string filename)
{
    orders = CSVReader::readCSV(filename);
}

/** return vector of all know products in the dataset*/
std::vector<std::string> OrderBook::getKnownProducts()
{
    std::vector<std::string> products;

    std::map<std::string,bool> prodMap;

    for (OrderBookEntry& e : orders)
    {
        prodMap[e.product] = true;
    }
    
    // now flatten the map to a vector of strings
    for (auto const& e : prodMap)
    {
        products.push_back(e.first);
    }

    return products;
}
/** return vector of Orders according to the sent filters*/
std::vector<OrderBookEntry> OrderBook::getOrders(OrderBookType type, 
                                        std::string product, 
                                        std::string timestamp)
{
    std::vector<OrderBookEntry> orders_sub;
    for (OrderBookEntry& e : orders)
    {
        if (e.orderType == type && 
            e.product == product && 
            e.timestamp == timestamp)
            {
                orders_sub.push_back(e);
            }
    }
    return orders_sub;
}
//Task 1
//rewriting the OrderBook get orders by reducing the parameters to just type and product
std::vector<OrderBookEntry> OrderBook::getOrders(OrderBookType type, 
                                        std::string product)
{
    std::vector<OrderBookEntry> orders_sub;
    for (OrderBookEntry& e : orders)
    {
        if (e.orderType == type && 
            e.product == product)
            {
                orders_sub.push_back(e);
            }
    }
    return orders_sub;
}

double OrderBook::getHighPrice(std::vector<OrderBookEntry>& orders)
{
    double max = orders[0].price;
    for (OrderBookEntry& e : orders)
    {
        if (e.price > max)max = e.price;
    }
    return max;
}


double OrderBook::getLowPrice(std::vector<OrderBookEntry>& orders)
{
    double min = orders[0].price;
    for (OrderBookEntry& e : orders)
    {
        if (e.price < min)min = e.price;
    }
    return min;
}

std::string OrderBook::getEarliestTime()
{
    return orders[0].timestamp;
}

std::string OrderBook::getNextTime(std::string timestamp)
{
    std::string next_timestamp = "";
    for (OrderBookEntry& e : orders)
    {
        if (e.timestamp > timestamp) 
        {
            next_timestamp = e.timestamp;
            break;
        }
    }
    if (next_timestamp == "")
    {
        next_timestamp = orders[0].timestamp;
    }
    return next_timestamp;
}

void OrderBook::insertOrder(OrderBookEntry& order)
{
    orders.push_back(order);
    std::sort(orders.begin(), orders.end(), OrderBookEntry::compareByTimestamp);
}

std::vector<OrderBookEntry> OrderBook::matchAsksToBids(std::string product, std::string timestamp)
{
// asks = orderbook.asks
    std::vector<OrderBookEntry> asks = getOrders(OrderBookType::ask, 
                                                 product, 
                                                 timestamp);
// bids = orderbook.bids
//Task 1
    std::vector<OrderBookEntry> bids = getOrders(OrderBookType::bid, 
                                                 product, 
                                                    timestamp);

    // sales = []
    std::vector<OrderBookEntry> sales; 

    // I put in a little check to ensure we have bids and asks
    // to process.
    if (asks.size() == 0 || bids.size() == 0)
    {
        std::cout << " OrderBook::matchAsksToBids no bids or asks" << std::endl;
        return sales;
    }

    // sort asks lowest first
    std::sort(asks.begin(), asks.end(), OrderBookEntry::compareByPriceAsc);
    // sort bids highest first
    std::sort(bids.begin(), bids.end(), OrderBookEntry::compareByPriceDesc);
    // for ask in asks:
    std::cout << "max ask " << asks[asks.size()-1].price << std::endl;
    std::cout << "min ask " << asks[0].price << std::endl;
    std::cout << "max bid " << bids[0].price << std::endl;
    std::cout << "min bid " << bids[bids.size()-1].price << std::endl;
    
    for (OrderBookEntry& ask : asks)
    {
    //     for bid in bids:
        for (OrderBookEntry& bid : bids)
        {
    //         if bid.price >= ask.price # we have a match
            if (bid.price >= ask.price)
            {
    //             sale = new order()
    //             sale.price = ask.price
            OrderBookEntry sale{ask.price, 0, timestamp, 
                product, 
                OrderBookType::asksale};

                if (bid.username == "simuser")
                {
                    sale.username = "simuser";
                    sale.orderType = OrderBookType::bidsale;
                }
                if (ask.username == "simuser")
                {
                    sale.username = "simuser";
                    sale.orderType =  OrderBookType::asksale;
                }
            
    //             # now work out how much was sold and 
    //             # create new bids and asks covering 
    //             # anything that was not sold
    //             if bid.amount == ask.amount: # bid completely clears ask
                if (bid.amount == ask.amount)
                {
    //                 sale.amount = ask.amount
                    sale.amount = ask.amount;
    //                 sales.append(sale)
                    sales.push_back(sale);
    //                 bid.amount = 0 # make sure the bid is not processed again
                    bid.amount = 0;
    //                 # can do no more with this ask
    //                 # go onto the next ask
    //                 break
                    break;
                }
    //           if bid.amount > ask.amount:  # ask is completely gone slice the bid
                if (bid.amount > ask.amount)
                {
    //                 sale.amount = ask.amount
                    sale.amount = ask.amount;
    //                 sales.append(sale)
                    sales.push_back(sale);
    //                 # we adjust the bid in place
    //                 # so it can be used to process the next ask
    //                 bid.amount = bid.amount - ask.amount
                    bid.amount =  bid.amount - ask.amount;
    //                 # ask is completely gone, so go to next ask                
    //                 break
                    break;
                }


    //             if bid.amount < ask.amount # bid is completely gone, slice the ask
                if (bid.amount < ask.amount && 
                   bid.amount > 0)
                {
    //                 sale.amount = bid.amount
                    sale.amount = bid.amount;
    //                 sales.append(sale)
                    sales.push_back(sale);
    //                 # update the ask
    //                 # and allow further bids to process the remaining amount
    //                 ask.amount = ask.amount - bid.amount
                    ask.amount = ask.amount - bid.amount;
    //                 bid.amount = 0 # make sure the bid is not processed again
                    bid.amount = 0;
    //                 # some ask remains so go to the next bid
    //                 continue
                    continue;
                }
            }
        }
    }
    return sales;             
}
//all self wrote
//Task 1
std::vector<Candlestick> OrderBook::calculateCandlesticks(std::string product, std::string type) {
    //clearing the candlestick vector before proceeding
    candlesticks.clear();
    // getting all the products based what the user has chosen to view
    std::vector<OrderBookEntry> productOrders = getOrders(OrderBookEntry::stringToOrderBookType(type), product);
    // declaring all the variables to assist in calculation
    std::string currentInterval = "";
    double open = 0.0, high = 0.0, low = 0.0, close = 0.0;
    double totalAmount = 0.0, totalValue = 0.0;
    int counter = 0;
    // looping through all the chosen products
    for (OrderBookEntry& order : productOrders) {
        // whenever a new candlestick is created
        if(counter == 0)
        {
            // Set current interval
            currentInterval = order.timestamp;
            // set first high and low price as the first product's price
            high = order.price;
            low = order.price;
            // setting the initial total value as the multiplication of amount and price
            totalValue = order.amount * order.price;
            // setting initial total amount as the order amount
            totalAmount = order.amount;
            // adding counter to count the number of products are being looped through
            counter += 1;
            // setting the open amount as the division of initial totalvalue and totalamount
            open = totalValue / totalAmount;
        }
        
        // Update high, low variable by checking with the current product's price
        high = std::max(high, order.price);
        low = std::min(low, order.price);

        // Update totalValue and totalAmount by adding on with every product looped through
        totalValue += order.amount * order.price;
        totalAmount += order.amount;
        counter += 1;
        // every 20000 products calculate the necessary values and store the collated data in a candlestick
        // this will ensure that there will be 6 candlesticks data for our plotting
        if(counter == 20000)
        {
            // get the close value of the overall timeframe within the 20000 sets of data
            // by dividing the totalvalue and totalamount of all 20000 datasets
            double close = totalValue / totalAmount;
            // storing the values into one singular candlestick
            candlesticks.emplace_back(currentInterval, open, high, low, close, product, Candlestick::stringToCandlestickType(type));
            // Set current interval
            currentInterval = "";
            // Reset values for the new interval
            open = 0;
            high = 0;
            low = 0;
            close = 0;
            totalValue = 0;
            totalAmount = 0;
            counter = 0;
        }
        
    }
    //once all the product are all looped through and empty
    if (!productOrders.empty()) {
        //calculate the close value of the remaining data 
        double close = totalValue / totalAmount;
        //store the remaining data into the candlestick
        candlesticks.emplace_back(currentInterval, open, high, low, close, product, Candlestick::stringToCandlestickType(type));
    }
    return candlesticks;
}

//Task 2
void OrderBook::printCandlestickPlot(std::vector<Candlestick> Candlesticks){
    std::cout << "Candlestick::printCandlestickPlot" << std::endl;
    
    // declaring the initial highest and lowest value to the first Candlestick's data respectively
    double highest = Candlesticks[0].getHigh();
    double lowest = Candlesticks[0].getLow();
    // looping through the candlestick data to find the highest and lowest value of all the Candlestick data
    for (Candlestick candle : Candlesticks){
        // replacing the highest/lowest value if the current value is higher or lower
        highest = std::max(highest, candle.getHigh());
        lowest = std::min(lowest, candle.getLow());
    }
    //the number of y values being printed
    int steps = 100;
    //determine the y range
    double yRange = highest - lowest;
    double stepSize = yRange / steps;

    //starting the plot from the first y value which is the highest value in the candlestick
    for(double y = highest; y > lowest; y-=stepSize)
    {
        //prints the y value
        std::cout << y << "\t";
        
        //limiting the size to 10
        int maxSize = 10;
        //if size of candlestick is lesser than 10 replace the maxsize
        if (maxSize > Candlesticks.size())
        {
            maxSize = Candlesticks.size();
        }
        // loop for the horizontal plotting
        for(int i = 0; i < maxSize; i++){
            //set variable candle as the current candlestick data
            Candlestick candle = Candlesticks[i];
            //if y value is higher or lower than candle data range, plot an empty space
            if (y > candle.getHigh() || y < candle.getLow())
            {
                std::cout << "     ";
            }
            // if y value is in the candlestick data range but is not in between the candle open close data range
            //print a * as the stalk of the candlestick plot
            else if ((y <= candle.getHigh() && y > candle.getOpen() && y > candle.getClose()) || 
            (y >= candle.getLow() && y< candle.getOpen() && y < candle.getClose()))
            {
                std::cout << "  *  ";
            }
            //if y value is in the candlestick data range and is in between the open close data range
            // print --- or +++ to represent the candlestick box 
            else
            {
                //print --- if the close value is smaller than the open value
                if (candle.getOpen() > candle.getClose())
                {
                    std::cout << " --- ";
                }
                //print +++ if the close value is larger than the open value
                else
                {
                    std::cout << " +++ ";
                }
            }
           
        }
        // go to the next line
         std::cout << std::endl;
    }

}