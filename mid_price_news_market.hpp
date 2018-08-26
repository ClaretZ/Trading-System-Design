#ifndef MIDNEWSPRICENEWS_H
#define MIDNEWSPRICENEWS_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

class MidPriceNewsMarket {
public:
    std::string ts = "0000";
    std::string symbol = "NaN";
    int volume = 0;
    double price = 0;
    MidPriceNewsMarket(std::string Ts, std::string Symbol, int Volume, double Price):ts(Ts),symbol(Symbol),volume(Volume), price(Price){}
    MidPriceNewsMarket() {}
    ~MidPriceNewsMarket(){}
};


#endif
