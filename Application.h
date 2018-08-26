/* -*- C++ -*- */

/****************************************************************************
** Copyright (c) 2001-2014
**
** This file is part of the QuickFIX FIX Engine
**
** This file may be distributed under the terms of the quickfixengine.org
** license as defined by quickfixengine.org and appearing in the file
** LICENSE included in the packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.quickfixengine.org/LICENSE for licensing information.
**
** Contact ask@quickfixengine.org if any conditions of this licensing are
** not clear to you.
** Modified by: MDCSAENZ for Assignment 4
**
****************************************************************************/

#include "quickfix/Application.h"
#include "quickfix/MessageCracker.h"
#include "quickfix/Values.h"
#include "quickfix/Mutex.h"

#include "quickfix/fix42/MarketDataRequest.h"
#include "quickfix/fix42/MarketDataSnapshotFullRefresh.h"
#include "quickfix/fix42/MarketDataIncrementalRefresh.h"
#include "quickfix/Session.h"
#include "quickfix/fix42/NewOrderSingle.h"
#include "quickfix/fix42/ExecutionReport.h"
#include "quickfix/fix42/OrderCancelRequest.h"
#include "quickfix/fix42/OrderCancelReject.h"
#include "quickfix/fix42/OrderCancelReplaceRequest.h"
#include <queue>

#include <map>
#include <string>
#include <vector>
#include <algorithm>// std::find; std::sort
#include <iomanip> //std::setw
#include <iostream>
#include <fstream>
#include <sstream>

#include "IDGenerator.h"
#include "Order.hpp"
#include "mystrategy.hpp"

using namespace std;

# define BUY 0
# define SELL 1



class OrderBook
{
public:
    std::string symbol;
    int side;
    int size;
    map<int, Order> bid_map;// Key is the ID, bid order is the object.
    map<int, Order> ask_map;// Key is the ID, ask order is the object.
    //map<int, Order> order_map;
    
    OrderBook(std::string sym, int s):symbol(sym),side(s),size(0){}
    //OrderBook():size(0){}
    OrderBook(){}
    
    bool process_order(Order a)
    {
        //size++;  // size will be dealt with in add_order, delete_order
        check_type(a);
        show_book();
    }
    
    int get_size()
    {
        return size;
    }
    
    
    void check_type(Order order)
    {
        char message_tp = (char)order.m_message_type;
        if (message_tp == '0') {add_order(order);}
        else if (message_tp == '1') {modify_order(order);}
        else if (message_tp == '2') {delete_order(order);}
    }
    
    void add_order(Order order){
        size = size + 1;
        // Bid
        if (order.m_side == 0)
        {
            bid_map.insert(pair<int, Order>(order.m_order_id,order));
        }
        // Ask
        if (order.m_side == 1)
        {
            ask_map.insert(pair<int, Order>(order.m_order_id,order));
        }
        
    }
    
    void modify_order(Order order){
        //add_order(order);
        //return;
        //Go through the OrderBook
        map<int, Order>::iterator old_order=bid_map.find(order.m_order_id);
        // If old_vector is in bid_vector
        if(old_order!=bid_map.end()){
            // Found the item
            symbol = old_order->second.m_symbol;
            if (old_order->second.m_side == order.m_side){
                //old_order->second.set_symbol(order.symbol);
                old_order->second.set_quantity(order.m_quantity);
                old_order->second.set_price(order.m_price);
                old_order->second.set_message_type(order.m_message_type);
                old_order->second.set_order_time(order.m_order_time);
            }
            else if (old_order->second.m_side != order.m_side){
                delete_order(old_order->second);
                order.set_symbol(old_order->second.m_symbol);
                add_order(order);
            }
        }
        
        // If old_order is in ask_vector
        else {
            map<int, Order>::iterator old_order=ask_map.find(order.m_order_id);
            symbol = old_order->second.m_symbol;
            if(old_order!=ask_map.end()){
                // Found the item
                if (old_order->second.m_side == order.m_side){
                    //old_order->second.set_symbol(order.symbol);
                    old_order->second.set_quantity(order.m_quantity);
                    old_order->second.set_price(order.m_price);
                    old_order->second.set_message_type(order.m_message_type);
                    old_order->second.set_order_time(order.m_order_time);
                }
                else if (old_order->second.m_side != order.m_side){
                    delete_order(old_order->second);
                    order.set_symbol(old_order->second.m_symbol);
                    add_order(order);
                }
            }
            else{add_order(order);}
        }
    }
    
    void delete_order(Order order){
        add_order(order);
        //return;
        size = size - 1;
        // Bid
        if (order.m_side == 0){
            map<int, Order>::iterator order_to_delete=bid_map.find(order.m_order_id);
            bid_map.erase(order_to_delete);
        }
        // Ask
        if (order.m_side == 1){
            map<int, Order>::iterator order_to_delete=ask_map.find(order.m_order_id);
            ask_map.erase(order_to_delete);
        }
    }
    
    
    ////The three functions below are used in the OrderBook::show_book////
    static void display(vector<Order> order_vector)
    {
        cout<<"Ticker"<<setw(20)<<"quantity"<<setw(20)<<"price"<<setw(20)<<"side"<<setw(24)<<"message_type"<<setw(20)<<"order_time"<<setw(19)<<"order_id"<<setw(20)<<"type"<<setw(20)<<"state"<< endl;
        
        for(int i = 0; i < order_vector.size(); i++){
            order_vector[i].show_order();
        }
    }
    static bool SortBid(Order i, Order j) { return i.m_price > j.m_price;}
    
    static bool SortAsk(Order i, Order j) { return i.m_price < j.m_price;}
    ////////////////////////////////////////////////////////////////////////
    
    
    
    //view book
    void show_book(){
        
        vector<Order> bid_vector;
        vector<Order> ask_vector;
        //vector<pair<int,Order>> bid_vector(bid_map.begin(), bid_map.end());
        for( map<int, Order>::iterator it = bid_map.begin(); it != bid_map.end(); ++it ) {
            
            bid_vector.push_back( it->second );
        }
        sort(bid_vector.begin(), bid_vector.end(), SortBid); //Sort bid_vector decreasingly
        
        //vector<pair<int,Order>> ask_vector(ask_map.begin(), ask_map.end());
        for( map<int, Order>::iterator it_a = ask_map.begin(); it_a != ask_map.end(); ++it_a ) {
            ask_vector.push_back( it_a->second );}
        
        sort(ask_vector.begin(), ask_vector.end(), SortAsk); //Sort ask_vector increasingly
        
        cout<<endl;
        cout<<"******************************************************************************* Order Book ********************************************************************************"<<endl;
        cout<<"---------------------------------------------------------------------------------------------------------------------------------------------------------------------------"<<endl;
        cout<<"*********************************************************************************** BID ***********************************************************************************"<<endl;
        display(bid_vector);
        cout<<endl;
        cout<<"*********************************************************************************** ASK ***********************************************************************************"<<endl;
        display(ask_vector);
        cout<<"---------------------------------------------------------------------------------------------------------------------------------------------------------------------------"<<endl;
        cout<<endl;
    }
};





class Application :
      public FIX::Application,
      public FIX::MessageCracker
{
  public:
    void run();
    OrderBook orderbook;
    IDGenerator m_generator;
    MyStrategy mystrategy;
    MidPriceNewsMarket midprice;
    bool StartSignal = true;

  private:
    void onCreate( const FIX::SessionID& ) {}
    void onLogon( const FIX::SessionID& sessionID );
    void onLogout( const FIX::SessionID& sessionID );
    void toAdmin( FIX::Message&, const FIX::SessionID& ) {}
    void toApp( FIX::Message&, const FIX::SessionID& )
    throw( FIX::DoNotSend );
    void fromAdmin( const FIX::Message&, const FIX::SessionID& )
    throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::RejectLogon ) {}
    void fromApp( const FIX::Message& message, const FIX::SessionID& sessionID )
    throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType );

    char queryAction();

    FIX42::MarketDataRequest queryMarketDataRequest42();
    
    void queryNewOrderSingle42(std::string symbol, char side, int quantity, double price);
    void queryNewOrderSingle42_initial();
    
    void queryOrderCancelRequest42(std::string origclordid, std::string symbol, char side, int quantity, double price);
    FIX42::OrderCancelReplaceRequest queryCancelReplaceRequest42();
    void queryMarketDataRequest();

    void onMessage( const FIX42::MarketDataSnapshotFullRefresh& message, const FIX::SessionID& sessionID );
    void onMessage( const FIX42::MarketDataIncrementalRefresh& message, const FIX::SessionID& sessionID );
    void onMessage( const FIX42::ExecutionReport& message, const FIX::SessionID& sessionID);
    void onMessage( const FIX42::OrderCancelReject& message, const FIX::SessionID& sessionID );
    void onMessage( const FIX42::ExecutionReport& message);
    
    // Added
    void Cancel_All_Orders_on_Market();
    void start()
    {
        StartSignal= true;
    }
    
    void stop()
    {
        StartSignal= false;
    }

    
};
