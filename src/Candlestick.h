#pragma once

#include <string>
//Task 1
enum class CandlestickType { bid, ask, unknown };

class Candlestick{
    public:
        //constructors
        
        Candlestick(std::string _timeframe, 
            double _open, 
            double _high, 
            double _low, 
            double _close, 
            std::string _product, 
            CandlestickType _candlestickType);

        static CandlestickType stringToCandlestickType(std::string s);

        std::string getTimeframe();
        double getOpen();
        double getHigh();
        double getLow();
        double getClose();
        std::string getProduct();
        CandlestickType getCandlestickType();
        
            
    private:
        std::string timeframe;
        double open;
        double high;
        double low;
        double close;
        std::string product;
        CandlestickType candlestickType;
};