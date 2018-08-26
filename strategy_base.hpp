#include "mid_price_news_market.hpp"
#include "Order.hpp"

class StrategyBase
{
public:
    StrategyBase() {}
    /*
    virtual void onBookUpdate(MidPriceNewsMarket &price )
    {
        std::cout << price.ts << " "
        << price.volume << " "
        << price.price
        << std::endl;
    }
     */
    
    virtual void onMarketUpdate( Order a)
    {
        std::cout << "Market response in StrategyBase" << std::endl;
    }
};
