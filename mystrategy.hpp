#include "strategy_base.hpp"
#include "Order.hpp"
#include <string>
#include <iostream>
#include <iomanip> //std::setw
#include <map>
#include <vector>
#include <utility>// std::pair

using namespace std;



class MyStrategy : public StrategyBase
{
    
public:
    MidPriceNewsMarket old_mid_price_AAL;
    MidPriceNewsMarket old_mid_price_CEA;
    MidPriceNewsMarket old_mid_price_DAL;
    
    double Position_AAL = 0;
    int Quantity_AAL = 0;
    
    double Position_CEA = 0;
    int Quantity_CEA = 0;
    
    double Position_DAL = 0;
    int Quantity_DAL = 0;
    
    // For all tickers
    
    map<int, Order> ACK_map;
    map<int, Order> FILLED_sell_map;
    map<int, Order> FILLED_buy_map;
    map<int, Order> CANCELLED_map;
    map<int, Order> REJECTED_map;
    map<int, Order> SENT_map;
    map<int, Order> PARTIALLY_FILLED_map;
    map<int, Order> order_to_delete_map;
    
    
    double CumPnL=0.00;
    double PnL=0;
    double Position = 0;
    
    int signal = 0;
    double Account = 0.00;
    bool isStarted;
    char signal_side;
    
    void start()
    {
        isStarted= true;
    }
    
    void stop()
    {
        isStarted= false;
    }

    
    void get_CumPnL()
    {
        cout<<"The final cumulative P&L is "<< CumPnL<<endl;
    }
    
        
    void send_buy_limit_order()
    {
        signal_side = '1';
    }
    
    
    void send_sell_limit_order()
    {
       signal_side = '2';
        
    }
    
    
    void cancel_All_orders()
    {
        cout<<"====BEGINNING of cancel_All_orders()===="<<endl;
        cout<<"Cancel ALL orders: "<<endl;
        for (auto it=ACK_map.rbegin();it!=ACK_map.rend();it--)
        {
            //cout<< it->second << endl;
            order_to_delete_map.insert(pair<int, Order>(it->second.m_order_id,it->second));
            // send_cancel_order(it->second);
        }
        order_to_delete_map.insert(pair<int, Order>(ACK_map.begin()->second.m_order_id,ACK_map.begin()->second));
        cout<<"NO PENDING ORDER IS ON THE MARKET"<<endl;
        cout<<"====END of cancel_All_orders()===="<<endl;
    }
    
    void show_outstanding_orders()
    {
        cout<<"====BEGINNING of show_outstanding_orders()===="<<endl;
        for (auto &it:ACK_map)
        {
            it.second.show_order();
        }
        cout<<"====END of show_outstanding_orders()===="<<endl;
    }
    
    void show_position()
    {
        Position = Position_AAL + Position_CEA + Position_DAL;
        cout<<"Total Position on Market: "<<Position<<endl;
    }
    

    
    // For different tickers;
    
    // AAL
    
    void onBookUpdate(MidPriceNewsMarket &price ){
        if (price.symbol == "AAL"){
        if (old_mid_price_AAL.volume ==0 && old_mid_price_AAL.price == 0){
            old_mid_price_AAL.ts = price.ts;
            old_mid_price_AAL.volume = price.volume;
            old_mid_price_AAL.price = price.price;
        }
        else if ( price.volume>old_mid_price_AAL.volume && price.price>old_mid_price_AAL.price){
            int DiffQuantity;
            int QuantityTmp = (int)(price.volume*0.005);
            DiffQuantity = QuantityTmp - Quantity_AAL; // New buy quantity
            send_buy_limit_order();
        }
        
        else if (price.volume<old_mid_price_AAL.volume && price.price<old_mid_price_AAL.price){
            send_sell_limit_order();
        }
        else{signal_side = '0';}
        
        old_mid_price_AAL.ts = price.ts;
        old_mid_price_AAL.volume = price.volume;
        old_mid_price_AAL.price = price.price;
        
        //if (!isStarted){
          //  return;
        //}
    }
       
        if (price.symbol == "CEA"){
            if (old_mid_price_CEA.volume ==0 && old_mid_price_CEA.price == 0){
                old_mid_price_CEA.ts = price.ts;
                old_mid_price_CEA.volume = price.volume;
                old_mid_price_CEA.price = price.price;
            }
            else if ( price.volume>old_mid_price_CEA.volume && price.price>old_mid_price_CEA.price){
                int DiffQuantity;
                int QuantityTmp = (int)(price.volume*0.005);
                DiffQuantity = QuantityTmp - Quantity_CEA; // New buy quantity
                send_buy_limit_order();
            }
            
            else if (price.volume<old_mid_price_CEA.volume && price.price<old_mid_price_CEA.price){
                send_sell_limit_order();
            }
            
            else {signal_side = '0';}
            
            old_mid_price_CEA.ts = price.ts;
            old_mid_price_CEA.volume = price.volume;
            old_mid_price_CEA.price = price.price;
            
            //if (!isStarted){
            //  return;
            //}
        }

        
        if (price.symbol == "DAL"){
            if (old_mid_price_DAL.volume ==0 && old_mid_price_DAL.price == 0){
                old_mid_price_DAL.ts = price.ts;
                old_mid_price_DAL.volume = price.volume;
                old_mid_price_DAL.price = price.price;
            }
            else if ( price.volume>old_mid_price_DAL.volume && price.price>old_mid_price_DAL.price){
                int DiffQuantity;
                int QuantityTmp = (int)(price.volume*0.005);
                DiffQuantity = QuantityTmp - Quantity_DAL; // New buy quantity
               send_buy_limit_order();
            }
            
            else if (price.volume<old_mid_price_DAL.volume && price.price<old_mid_price_DAL.price){
                send_sell_limit_order();
            }
            
            else {signal_side = '0';}
            
            old_mid_price_DAL.ts = price.ts;
            old_mid_price_DAL.volume = price.volume;
            old_mid_price_DAL.price = price.price;
            
            //if (!isStarted){
            //  return;
            //}
        }
        
    }
    
    
    void onMarketUpdate( Order a)
    {
        std::cout << "Market response:" << std::endl;
        a.show_order();
        ACK_map.insert(pair<int, Order>(a.m_order_id,a));
        
        // 1 = Partially filled
        if (a.m_state == '1'){
            if (a.m_symbol == "AAL")
            {
            map<int, Order>::iterator ACK_order_to_change=ACK_map.find(a.m_order_id);
            ACK_order_to_change->second.set_quantity(a.m_leave_quantity);
            PARTIALLY_FILLED_map.insert(pair<int, Order>(a.m_order_id,a));
            
            // Buy order
            if (a.m_side == '1') {
                cout<<"PARTIALLY FILLED buy order:"<<endl;
                cout<<"Order id: "<<a.m_order_id<<endl;
                cout<<"Price: "<<a.m_lastpx<<endl;
                cout<<"BUY Quantity: "<<a.m_lastshares<<endl;
                Quantity_AAL = Quantity_AAL + a.m_lastshares;
                Account = Account - a.m_lastshares*a.m_lastpx;
                Position_AAL = a.m_lastpx*Quantity_AAL;
                cout<<"Total Quantity of AAL: "<<Quantity_AAL<<endl;
                cout<<"Account: "<<Account<<endl;
                cout<<"Position of AAL on Market: "<<Position<<endl;
                show_position();
            }
            // Sell order
            if (a.m_side == '2') {
                cout<<"PARTIALLY FILLED sell order:"<<endl;
                cout<<"Order id: "<<a.m_order_id<<endl;
                cout<<"Price: "<<a.m_lastpx<<endl;
                cout<<"SELL Quantity: "<<a.m_lastshares<<endl;
                Account = Account + a.m_lastshares*a.m_lastpx;
                CumPnL = Account;
                cout<<"Cumulative PnL = "<<CumPnL<<endl;
                get_CumPnL();
                Quantity_AAL = 0; // Clear the position;
                Position_AAL = a.m_lastpx*Quantity_AAL;
                cout<<"Position of AAL on Market: "<<Position<<endl;
                show_position();
            }
        }
            
            
            if (a.m_symbol == "CEA")
            {
                map<int, Order>::iterator ACK_order_to_change=ACK_map.find(a.m_order_id);
                ACK_order_to_change->second.set_quantity(a.m_leave_quantity);
                PARTIALLY_FILLED_map.insert(pair<int, Order>(a.m_order_id,a));
                
                // Buy order
                if (a.m_side == '1') {
                    cout<<"PARTIALLY FILLED buy order:"<<endl;
                    cout<<"Order id: "<<a.m_order_id<<endl;
                    cout<<"Price: "<<a.m_lastpx<<endl;
                    cout<<"BUY Quantity: "<<a.m_lastshares<<endl;
                    Quantity_CEA = Quantity_CEA + a.m_lastshares;
                    Account = Account - a.m_lastshares*a.m_lastpx;
                    Position_CEA = a.m_lastpx*Quantity_CEA;
                    cout<<"Total Quantity of CEA: "<<Quantity_CEA<<endl;
                    cout<<"Account: "<<Account<<endl;
                    cout<<"Position of CEA on Market: "<<Position<<endl;
                    show_position();
                }
                // Sell order
                if (a.m_side == '2') {
                    cout<<"PARTIALLY FILLED sell order:"<<endl;
                    cout<<"Order id: "<<a.m_order_id<<endl;
                    cout<<"Price: "<<a.m_lastpx<<endl;
                    cout<<"SELL Quantity: "<<a.m_lastshares<<endl;
                    Account = Account + a.m_lastshares*a.m_lastpx;
                    CumPnL = Account;
                    cout<<"Cumulative PnL = "<<CumPnL<<endl;
                    get_CumPnL();
                    Quantity_CEA = 0; // Clear the position;
                    Position_CEA = a.m_lastpx*Quantity_CEA;
                    cout<<"Position of CEA on Market: "<<Position<<endl;
                    show_position();
                }
            }

            
            if (a.m_symbol == "DAL")
            {
                map<int, Order>::iterator ACK_order_to_change=ACK_map.find(a.m_order_id);
                ACK_order_to_change->second.set_quantity(a.m_leave_quantity);
                PARTIALLY_FILLED_map.insert(pair<int, Order>(a.m_order_id,a));
                
                // Buy order
                if (a.m_side == '1') {
                    cout<<"PARTIALLY FILLED buy order:"<<endl;
                    cout<<"Order id: "<<a.m_order_id<<endl;
                    cout<<"Price: "<<a.m_lastpx<<endl;
                    cout<<"BUY Quantity: "<<a.m_lastshares<<endl;
                    Quantity_DAL = Quantity_DAL + a.m_lastshares;
                    Account = Account - a.m_lastshares*a.m_lastpx;
                    Position_DAL = a.m_lastpx*Quantity_DAL;
                    cout<<"Total Quantity of DAL: "<<Quantity_DAL<<endl;
                    cout<<"Account: "<<Account<<endl;
                    cout<<"Position of DAL on Market: "<<Position<<endl;
                    show_position();
                }
                // Sell order
                if (a.m_side == '2') {
                    cout<<"PARTIALLY FILLED sell order:"<<endl;
                    cout<<"Order id: "<<a.m_order_id<<endl;
                    cout<<"Price: "<<a.m_lastpx<<endl;
                    cout<<"SELL Quantity: "<<a.m_lastshares<<endl;
                    Account = Account + a.m_lastshares*a.m_lastpx;
                    CumPnL = Account;
                    cout<<"Cumulative PnL = "<<CumPnL<<endl;
                    get_CumPnL();
                    Quantity_DAL = 0; // Clear the position;
                    Position_DAL = a.m_lastpx*Quantity_DAL;
                    cout<<"Position of DAL on Market: "<<Position<<endl;
                    show_position();
                }
            }
            
            
            
        }
        
        // 2 = Filled
        if (a.m_state == '2'){
            map<int, Order>::iterator ACK_order_to_delete=ACK_map.find(a.m_order_id);
            ACK_map.erase(ACK_order_to_delete);
            
            map<int, Order>::iterator PARTIALLY_FILLED_order_to_delete=PARTIALLY_FILLED_map.find(a.m_order_id);
            // Find the order in PARTIALLY_FILLED_map;
            if(PARTIALLY_FILLED_order_to_delete!=PARTIALLY_FILLED_map.end()){
                PARTIALLY_FILLED_map.erase(PARTIALLY_FILLED_order_to_delete);
                
                if (a.m_symbol == "AAL")
                {
                // Buy order
                if (a.m_side == '1') {
                    cout<<"PARTIALLY FILLED buy order:"<<endl;
                    cout<<"Order id: "<<a.m_order_id<<endl;
                    cout<<"Price: "<<a.m_lastpx<<endl;
                    cout<<"BUY Quantity: "<<a.m_lastshares<<endl;
                    Quantity_AAL = Quantity_AAL + a.m_lastshares;
                    Account = Account - a.m_lastshares*a.m_lastpx;
                    Position_AAL = a.m_lastpx*Quantity_AAL; //Total position on market
                    cout<<"Total Quantity of AAL: "<<Quantity_AAL<<endl;
                    cout<<"Account: "<<Account<<endl;
                    cout<<"Total Position of AAL on Market: "<<Position_AAL<<endl;
                    FILLED_buy_map.insert(pair<int, Order>(a.m_order_id,a));
                    show_position();
                }
                // Sell order
                if (a.m_side == '2') {
                    cout<<"PARTIALLY FILLED sell order:"<<endl;
                    cout<<"Order id: "<<a.m_order_id<<endl;
                    cout<<"Price: "<<a.m_lastpx<<endl;
                    cout<<"SELL Quantity: "<<a.m_lastshares<<endl;
                    Account = Account + a.m_lastshares*a.m_lastpx;
                    CumPnL = Account;
                    cout<<"Cumulative PnL = "<<CumPnL<<endl;
                    get_CumPnL();
                    Quantity_AAL = 0; // Clear the position;
                    Position_AAL = a.m_lastpx*Quantity_AAL;
                    cout<<"Total Position on Market: "<<Position_AAL<<endl;
                    FILLED_sell_map.insert(pair<int, Order>(a.m_order_id,a));
                    show_position();
                }
            
                }
                
                
                
                if (a.m_symbol == "CEA")
                {
                    // Buy order
                    if (a.m_side == '1') {
                        cout<<"PARTIALLY FILLED buy order:"<<endl;
                        cout<<"Order id: "<<a.m_order_id<<endl;
                        cout<<"Price: "<<a.m_lastpx<<endl;
                        cout<<"BUY Quantity: "<<a.m_lastshares<<endl;
                        Quantity_CEA = Quantity_CEA + a.m_lastshares;
                        Account = Account - a.m_lastshares*a.m_lastpx;
                        Position_CEA = a.m_lastpx*Quantity_CEA; //Total position on market
                        cout<<"Total Quantity of CEA: "<<Quantity_CEA<<endl;
                        cout<<"Account: "<<Account<<endl;
                        cout<<"Total Position of CEA on Market: "<<Position_CEA<<endl;
                        FILLED_buy_map.insert(pair<int, Order>(a.m_order_id,a));
                        show_position();
                    }
                    // Sell order
                    if (a.m_side == '2') {
                        cout<<"PARTIALLY FILLED sell order:"<<endl;
                        cout<<"Order id: "<<a.m_order_id<<endl;
                        cout<<"Price: "<<a.m_lastpx<<endl;
                        cout<<"SELL Quantity: "<<a.m_lastshares<<endl;
                        Account = Account + a.m_lastshares*a.m_lastpx;
                        CumPnL = Account;
                        cout<<"Cumulative PnL = "<<CumPnL<<endl;
                        get_CumPnL();
                        Quantity_CEA = 0; // Clear the position;
                        Position_CEA = a.m_lastpx*Quantity_CEA;
                        cout<<"Total Position on Market: "<<Position_CEA<<endl;
                        FILLED_sell_map.insert(pair<int, Order>(a.m_order_id,a));
                        show_position();
                    }
                    
                }

                if (a.m_symbol == "DAL")
                {
                    // Buy order
                    if (a.m_side == '1') {
                        cout<<"PARTIALLY FILLED buy order:"<<endl;
                        cout<<"Order id: "<<a.m_order_id<<endl;
                        cout<<"Price: "<<a.m_lastpx<<endl;
                        cout<<"BUY Quantity: "<<a.m_lastshares<<endl;
                        Quantity_DAL = Quantity_DAL + a.m_lastshares;
                        Account = Account - a.m_lastshares*a.m_lastpx;
                        Position_DAL = a.m_lastpx*Quantity_DAL; //Total position on market
                        cout<<"Total Quantity of DAL: "<<Quantity_DAL<<endl;
                        cout<<"Account: "<<Account<<endl;
                        cout<<"Total Position of DAL on Market: "<<Position_DAL<<endl;
                        FILLED_buy_map.insert(pair<int, Order>(a.m_order_id,a));
                        show_position();
                    }
                    // Sell order
                    if (a.m_side == '2') {
                        cout<<"PARTIALLY FILLED sell order:"<<endl;
                        cout<<"Order id: "<<a.m_order_id<<endl;
                        cout<<"Price: "<<a.m_lastpx<<endl;
                        cout<<"SELL Quantity: "<<a.m_lastshares<<endl;
                        Account = Account + a.m_lastshares*a.m_lastpx;
                        CumPnL = Account;
                        cout<<"Cumulative PnL = "<<CumPnL<<endl;
                        get_CumPnL();
                        Quantity_DAL = 0; // Clear the position;
                        Position_DAL = a.m_lastpx*Quantity_DAL;
                        cout<<"Total Position on Market: "<<Position_DAL<<endl;
                        FILLED_sell_map.insert(pair<int, Order>(a.m_order_id,a));
                        show_position();
                    }
                    
                }

                
                }
                
            
            }
            else {
                
                if (a.m_symbol == "AAL")
                {
            // Buy order
            if (a.m_side == '1') {
                FILLED_buy_map.insert(pair<int, Order>(a.m_order_id,a));
                cout<<"FILLED buy order:"<<endl;
                cout<<"Order id: "<<a.m_order_id<<endl;
                cout<<"Price: "<<a.m_price<<endl;
                cout<<"BUY Quantity: "<<a.m_quantity<<endl;
                Quantity_AAL = Quantity_AAL + a.m_quantity;// Total quatity in buy position
                Account = Account - a.m_quantity*a.m_price; // Total money in account
                Position_AAL = a.m_price*Quantity_AAL; //Total position on market
                cout<<"Total Quantity of AAL: "<<Quantity_AAL<<endl;
                cout<<"Account: "<<Account<<endl;
                cout<<"Position of AAL on Market: "<<Position_AAL<<endl;
                show_position();
            }
            
            // Sell order
            if (a.m_side == '2') {
                FILLED_sell_map.insert(pair<int, Order>(a.m_order_id,a));
                cout<<"FILLED sell order:"<<endl;
                cout<<"Order id: "<<a.m_order_id<<endl;
                cout<<"Price: "<<a.m_price<<endl;
                cout<<"SELL Quantity: "<<a.m_quantity<<endl;
                Account = Account + a.m_quantity*a.m_price;
                CumPnL = Account;
                cout<<"Cumulative PnL = "<<CumPnL<<endl;
                get_CumPnL();
                Quantity_AAL = 0; // Clear the position;
                Position_AAL = a.m_price*Quantity_AAL;
                cout<<"Position of AAL on Market: "<<Position<<endl;
                show_position();
            }
            }
                
                
                
                if (a.m_symbol == "CEA")
                {
                    // Buy order
                    if (a.m_side == '1') {
                        FILLED_buy_map.insert(pair<int, Order>(a.m_order_id,a));
                        cout<<"FILLED buy order:"<<endl;
                        cout<<"Order id: "<<a.m_order_id<<endl;
                        cout<<"Price: "<<a.m_price<<endl;
                        cout<<"BUY Quantity: "<<a.m_quantity<<endl;
                        Quantity_CEA = Quantity_CEA + a.m_quantity;// Total quatity in buy position
                        Account = Account - a.m_quantity*a.m_price; // Total money in account
                        Position_CEA = a.m_price*Quantity_CEA; //Total position on market
                        cout<<"Total Quantity of CEA: "<<Quantity_CEA<<endl;
                        cout<<"Account: "<<Account<<endl;
                        cout<<"Position of CEA on Market: "<<Position_CEA<<endl;
                        show_position();
                    }
                    
                    // Sell order
                    if (a.m_side == '2') {
                        FILLED_sell_map.insert(pair<int, Order>(a.m_order_id,a));
                        cout<<"FILLED sell order:"<<endl;
                        cout<<"Order id: "<<a.m_order_id<<endl;
                        cout<<"Price: "<<a.m_price<<endl;
                        cout<<"SELL Quantity: "<<a.m_quantity<<endl;
                        Account = Account + a.m_quantity*a.m_price;
                        CumPnL = Account;
                        cout<<"Cumulative PnL = "<<CumPnL<<endl;
                        get_CumPnL();
                        Quantity_CEA = 0; // Clear the position;
                        Position_CEA = a.m_price*Quantity_CEA;
                        cout<<"Position of CEA on Market: "<<Position<<endl;
                        show_position();
                    }
                }
                
                
                
                if (a.m_symbol == "DAL")
                {
                    // Buy order
                    if (a.m_side == '1') {
                        FILLED_buy_map.insert(pair<int, Order>(a.m_order_id,a));
                        cout<<"FILLED buy order:"<<endl;
                        cout<<"Order id: "<<a.m_order_id<<endl;
                        cout<<"Price: "<<a.m_price<<endl;
                        cout<<"BUY Quantity: "<<a.m_quantity<<endl;
                        Quantity_DAL = Quantity_DAL + a.m_quantity;// Total quatity in buy position
                        Account = Account - a.m_quantity*a.m_price; // Total money in account
                        Position_DAL = a.m_price*Quantity_DAL; //Total position on market
                        cout<<"Total Quantity of DAL: "<<Quantity_DAL<<endl;
                        cout<<"Account: "<<Account<<endl;
                        cout<<"Position of DAL on Market: "<<Position_DAL<<endl;
                        show_position();
                    }
                    
                    // Sell order
                    if (a.m_side == '2') {
                        FILLED_sell_map.insert(pair<int, Order>(a.m_order_id,a));
                        cout<<"FILLED sell order:"<<endl;
                        cout<<"Order id: "<<a.m_order_id<<endl;
                        cout<<"Price: "<<a.m_price<<endl;
                        cout<<"SELL Quantity: "<<a.m_quantity<<endl;
                        Account = Account + a.m_quantity*a.m_price;
                        CumPnL = Account;
                        cout<<"Cumulative PnL = "<<CumPnL<<endl;
                        get_CumPnL();
                        Quantity_DAL = 0; // Clear the position;
                        Position_DAL = a.m_price*Quantity_DAL;
                        cout<<"Position of DAL on Market: "<<Position<<endl;
                        show_position();
                    }
                }
              
        }
        
        // 8 = Rejected
        if (a.m_state == '8') {
            REJECTED_map.insert(pair<int, Order>(a.m_order_id,a));
        }
        
        // 4 = Canceled
        if (a.m_state == '4') {
            map<int, Order>::iterator ACK_order_to_delete=ACK_map.find(a.m_order_id);
            ACK_map.erase(ACK_order_to_delete);
            CANCELLED_map.insert(pair<int, Order>(a.m_order_id,a));
        }
        
                
                // 6 = Pending Cancel (e.g. result of Order Cancel Request <F>)
                // 9 = Suspended
                // A = Pending New
                // D = Accepted for bidding
                // E = Pending Replace (e.g. result of Order Cancel/Replace Request <G>)
        if (a.m_state == '6'||a.m_state == '9'||a.m_state == 'A'||a.m_state == 'D'||a.m_state == 'E') {
            //Go through ACK_map
            map<int, Order>::iterator ACK_old_order=ACK_map.find(a.m_order_id);
            // If old_vector is in ACK_map
            if(ACK_old_order==ACK_map.end()){
                ACK_map.insert(pair<int, Order>(a.m_order_id,a));
            }
        }
    }
    
};


