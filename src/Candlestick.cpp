#include "Candlestick.h"
#include <cmath>
#include <vector>
#include <iostream>
// Task 1
Candlestick::Candlestick(std::string _timeframe, 
                        double _open, 
                        double _high, 
                        double _low, 
                        double _close, 
                        std::string _product, 
                        CandlestickType _candlestickType)
: timeframe(_timeframe),
open(_open),
high(_high),
low(_low),
close(_close),
product(_product),
candlestickType(_candlestickType) {}

CandlestickType Candlestick::stringToCandlestickType(std::string s)
{
    if (s == "ask")
    {
        return CandlestickType::ask;
    }
    if (s == "bid")
    {
        return CandlestickType::bid;
    }
    return CandlestickType::unknown;
}

std::string Candlestick::getTimeframe()
{
    return timeframe;
}
double Candlestick::getOpen()
{
    return open;
}
double Candlestick::getHigh()
{
    return high;
}
double Candlestick::getLow()
{
    return low;
}
double Candlestick::getClose()
{
    return close;
}
std::string Candlestick::getProduct()
{
    return product;
}
CandlestickType Candlestick::getCandlestickType()
{
    return candlestickType;
}
