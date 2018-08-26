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
****************************************************************************/

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;


void Application::onLogon( const FIX::SessionID& sessionID )
{
 std::cout << std::endl << "Logon - " << sessionID << std::endl;
}

void Application::onLogout( const FIX::SessionID& sessionID )
{
  std::cout << std::endl << "Logout - " << sessionID << std::endl;
}

void Application::fromApp( const FIX::Message& message, const FIX::SessionID& sessionID )
throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType )
{
    if (StartSignal == true){
        crack( message, sessionID );
        std::cout << std::endl << "IN: " << message << std::endl;}
}

void Application::toApp( FIX::Message& message, const FIX::SessionID& sessionID )
throw( FIX::DoNotSend )
{
  try
  {
    FIX::PossDupFlag possDupFlag;
    message.getHeader().getField( possDupFlag );
    if ( possDupFlag ) throw FIX::DoNotSend();
  }
  catch ( FIX::FieldNotFound& ) {}

  std::cout << std::endl
  << "OUT: " << message << std::endl;
}


void Application::run()
{
    while (1)
    {
        try
        {
            char action = queryAction();
             if ( action == '0' )
                break;
             else if ( action == '1' )
             {
                 start();
                 queryMarketDataRequest();
                 mystrategy.start();
             }
             else if ( action == '2' )
                 stop();
             else if ( action == '3' )
                 start();
             else if ( action == '4' )
             mystrategy.start();
             else if ( action == '5' )
             mystrategy.stop();
             else if ( action == '6' )
             Cancel_All_Orders_on_Market();
             else if ( action == '7' )
             mystrategy.show_position();
             else if ( action == '8' )
             mystrategy.get_CumPnL();
        }
        catch ( std::exception & e )
        {
            std::cout << "Message Not Sent: " << e.what();
        }
    }
}

char Application::queryAction()
{
    char value;
    std::cout << std::endl
    << "0) Quit" << std::endl
    << "1) Market data test & begin strategy" << std::endl
    << "2) Stop showing the detailed data " << std::endl
    << "3) Start showing the detailed data" << std::endl
    << "4) Start strategy" << std::endl
    << "5) Stop strategy" << std::endl
    << "6) Cancel All orders on market" << std::endl
    << "7) Show total position on market" << std::endl
    << "8) Get Cumulative P&L" << std::endl
    << "Action: ";
    std::cin >> value;
    switch ( value )
    {
        case'0': case '1': case '2': case '3': case '4':case '5': case '6':case '7': case '8':break;
        default: throw std::exception();
    }
    return value;
}


void Application::queryMarketDataRequest()
{
    if (StartSignal==true){
  std::cout << "\nMarketDataRequest\n";
  FIX::Message md;
  md = queryMarketDataRequest42();
  FIX::Session::sendToTarget( md );}
}

//FIX42::NewOrderSingle Application::queryNewOrderSingle42()


void Application::queryNewOrderSingle42(std::string symbol, char side, int quantity, double price)
{
    std::cout << "\nNewOrderSingle\n";
    FIX42::NewOrderSingle newOrderSingle(FIX::ClOrdID(m_generator.genOrderID()),
                                         FIX::HandlInst('1'),
                                         FIX::Symbol(symbol),
                                         FIX::Side(side),
                                         FIX::TransactTime(),
                                         FIX::OrdType(FIX::OrdType_LIMIT));
    
    newOrderSingle.setField( FIX::OrderQty( quantity ));
    newOrderSingle.setField( FIX::Price( price ));
    newOrderSingle.setField( FIX::Account(std::to_string(123)));
    newOrderSingle.getHeader().setField(FIX::TargetCompID("SEB91"));
    newOrderSingle.getHeader().setField(FIX::SenderCompID( "KYZHOU" ));
    FIX::Session::sendToTarget( newOrderSingle, FIX::SenderCompID("KYZHOU"), FIX::TargetCompID("SEB91"));
}

 
 
void Application::queryNewOrderSingle42_initial()
{
    std::cout << "\nNewOrderSingle\n";
    FIX42::NewOrderSingle newOrderSingle(FIX::ClOrdID(m_generator.genOrderID()),
                                         FIX::HandlInst('1'),
                                         FIX::Symbol("AAL"),
                                         FIX::Side('1'),
                                         FIX::TransactTime(),
                                         FIX::OrdType(FIX::OrdType_LIMIT));
    
    newOrderSingle.setField( FIX::OrderQty(233));
    newOrderSingle.setField( FIX::Price( 11.11 ));
    newOrderSingle.setField( FIX::Account(std::to_string(123)));
    newOrderSingle.getHeader().setField(FIX::TargetCompID("SEB91"));
    newOrderSingle.getHeader().setField(FIX::SenderCompID( "KYZHOU" ));
    FIX::Session::sendToTarget( newOrderSingle, FIX::SenderCompID("KYZHOU"), FIX::TargetCompID("SEB91"));
}



void Application::queryOrderCancelRequest42(std::string origclordid, std::string symbol, char side, int quantity, double price)
{
    std::cout << "\nOrderCancelRequest\n";
    FIX42::OrderCancelRequest orderCancelRequest( FIX::OrigClOrdID(origclordid),
                                                 FIX::ClOrdID(m_generator.genOrderID()),
                                                 FIX::Symbol(symbol),
                                                 FIX::Side(side),
                                                 FIX::TransactTime() );
    
    orderCancelRequest.setField( FIX::OrderQty ( quantity ) );
    orderCancelRequest.setField( FIX::Price( price ) );
    orderCancelRequest.setField( FIX::Account(std::to_string(1)));
    orderCancelRequest.getHeader().setField(FIX::TargetCompID("SEB91"));
    orderCancelRequest.getHeader().setField(FIX::SenderCompID( "KYZHOU" ));
    FIX::Session::sendToTarget( orderCancelRequest, FIX::SenderCompID("KYZHOU"), FIX::TargetCompID("SEB91"));
}

FIX42::OrderCancelReplaceRequest Application::queryCancelReplaceRequest42(){
    FIX::ClOrdID clid = FIX::ClOrdID(m_generator.genOrderID());
    FIX::Symbol sym = FIX::Symbol("LUV");
    FIX::Side the_side = FIX::Side('1');//1 buy, 2 sell
    FIX::OrdType ordertype = FIX::OrdType_LIMIT;
    
    
    FIX42::OrderCancelReplaceRequest cancelReplaceRequest(FIX::OrigClOrdID("123"),clid, FIX::HandlInst( '1' ),sym, the_side, FIX::TransactTime(), ordertype);
    
    cancelReplaceRequest.setField( FIX::Price( 233 ) );
    cancelReplaceRequest.setField( FIX::OrderQty( 2333 ));
    
    cancelReplaceRequest.getHeader().setField(FIX::TargetCompID("KYZHOU"));
    cancelReplaceRequest.getHeader().setField(FIX::SenderCompID( "SEB91" ));
    return cancelReplaceRequest;
}


FIX42::MarketDataRequest Application::queryMarketDataRequest42(){
    
    //Create MarketDataRequest object and what SubscriptionRequestType desired
    FIX42::MarketDataRequest marketDataRequest(
                                               FIX::MDReqID("1"),
                                               FIX::SubscriptionRequestType(FIX::SubscriptionRequestType_SNAPSHOT_PLUS_UPDATES),
                                               FIX::MarketDepth(5));
    
    //Setup whawt MarketData Update Type desired
    marketDataRequest.set( FIX::MDUpdateType( FIX::MDUpdateType_INCREMENTAL_REFRESH ) );
    
    FIX42::MarketDataRequest::NoRelatedSym noRelatedSym;
    
    // AAL	American Airlines Group Inc.
    // ALK	Alaska Air Group, Inc.
    // AVH	Avianca Holdings S.A.
    // CEA	China Eastern Airlines Corporation Ltd.
    // ZNH	China Southern Airlines Company Limited
    // VLRS	Controladora Vuela Compania de Aviacion, S.A.B. de C.V.
    // CPA	Copa Holdings, S.A.
    // DAL	Delta Air Lines, Inc.
    // GOL	Gol Linhas Aereas Inteligentes S.A.
    // LFL	LATAM Airlines Group S.A.
    // LUV	Southwest Airlines Company
    // UAL	United Continental Holdings, Inc.
    
    noRelatedSym.set( FIX::Symbol( "AAL" ) );
    marketDataRequest.addGroup( noRelatedSym );
    noRelatedSym.set( FIX::Symbol( "CEA" ) );
    marketDataRequest.addGroup( noRelatedSym );
    noRelatedSym.set( FIX::Symbol( "DAL" ) );
    marketDataRequest.addGroup( noRelatedSym );
    noRelatedSym.set( FIX::Symbol( "ALK" ) );
    marketDataRequest.addGroup( noRelatedSym );
    noRelatedSym.set( FIX::Symbol( "AVH" ) );
    marketDataRequest.addGroup( noRelatedSym );
    noRelatedSym.set( FIX::Symbol( "ZNH" ) );
    marketDataRequest.addGroup( noRelatedSym );
    noRelatedSym.set( FIX::Symbol( "VLRS" ) );
    marketDataRequest.addGroup( noRelatedSym );
    noRelatedSym.set( FIX::Symbol( "CPA" ) );
    marketDataRequest.addGroup( noRelatedSym );
    noRelatedSym.set( FIX::Symbol( "GOL" ) );
    marketDataRequest.addGroup( noRelatedSym );
    noRelatedSym.set( FIX::Symbol( "LFL" ) );
    marketDataRequest.addGroup( noRelatedSym );
    noRelatedSym.set( FIX::Symbol( "LUV" ) );
    marketDataRequest.addGroup( noRelatedSym );
    noRelatedSym.set( FIX::Symbol( "UAL" ) );
    marketDataRequest.addGroup( noRelatedSym );

    
    FIX42::MarketDataRequest::NoMDEntryTypes noMDEntryTypes;
    
    //Setup a group to for desired MarketDataRequest Entry Types: BID and OFFER
    noMDEntryTypes.set( FIX::MDEntryType( FIX::MDEntryType_BID ) );
    marketDataRequest.addGroup( noMDEntryTypes );
    noMDEntryTypes.set( FIX::MDEntryType( FIX::MDEntryType_OFFER ) );
    marketDataRequest.addGroup( noMDEntryTypes );
    
    //Setup your Sender and TargetCompID, You can hardcode them.
    //BONUS TODO: How can you call them from the config instead of them being hardcoded?
    marketDataRequest.getHeader().setField(FIX::SenderCompID( "KYZHOU" ));
    marketDataRequest.getHeader().setField(FIX::TargetCompID( "SEB91" ));
    std::cout << "Sending: " <<  marketDataRequest.toString() << std::endl;
    return marketDataRequest;
}


void Application::onMessage( const FIX42::ExecutionReport& message, const FIX::SessionID& sessionID )
{
    std::cout<<"Got A Execution Report Message: "<< message.toString()<< " "<<std::endl;
    // Got A Execution Report Message: 8=FIX.4.2   9=152  35=8  34=1332 49=SEB91  52=20170608-04:04:21.489  56=KYZHOU  6=233  11=383  14=2333  17=7265  20=0  31=233  32=2333  37=14530  38=2333  39=2  54=1  55=LUV  150=21  51=0  10=240
    
    Order order;
    
    // 35=8
    // Defines message type. ALWAYS THIRD FIELD IN MESSAGE. (Always unencrypted)
    const std::string& msgTypeValue = message.getHeader().getField( FIX::FIELD::MsgType );
    std::cout<<"Got MsgTypeValue: "<<msgTypeValue<<endl;
    
    // 34=1332
    // Integer message sequence number.
    const std::string& msgSeqNum = message.getHeader().getField( FIX::FIELD::MsgSeqNum );
    std::cout<<"Got MsgSeqNum: "<<msgSeqNum<<endl;
    
    // 49=SEB91
    // Assigned value used to identify firm sending message.
    //const std::string& senderCompID = message.getHeader().getField( FIX::FIELD::SenderCompID );
    //std::cout<<"Got SenderCompID: "<<senderCompID<<endl;
    
    // 52=20170608-04:04:21.489
    // Time of message transmission (always expressed in UTC (Universal Time Coordinated, also known as 'GMT')
    const std::string& sendingTime = message.getHeader().getField( FIX::FIELD::SendingTime);
    std::cout<<"Got SendingTime: "<<sendingTime<<endl;
    order.set_order_time(sendingTime);
    
    
    // 56=KYZHOU
    // Assigned value used to identify receiving firm.
    //const std::string& targetCompID = message.getHeader().getField( FIX::FIELD::TargetCompID );
    //std::cout<<"Got TargetCompID: "<<targetCompID<<endl;
    
    
    // 6=233
    // FIX 4.2 : AvgPx <6> field: Calculated average price of all fills on this order.
    FIX::AvgPx avgPx;
    double price = message.get(avgPx);
    std::cout<<"Got AvgPx: "<<message.get(avgPx)<<std::endl;
    order.set_price(price);
    
    // 11=383
    // FIX 4.2 : ClOrdID <11> field
    FIX::ClOrdID clOrdID;
    std::string clordid = message.get(clOrdID);
    std::cout<<"Got AvgPx: "<<message.get(clOrdID)<<std::endl;
    order.set_clordid(clordid);
    
    // 14=2333
    // FIX 4.2 : CumQty <14> field: Total number of shares filled.
    FIX::CumQty cumQty;
    double cumqty = message.get(cumQty);
    std::cout<<"Got CumQty: "<<message.get(cumQty)<<std::endl;
    order.set_quantity(cumqty);
    
    // 17=7265
    // FIX 4.2 : ExecID <17> field : Unique identifier of execution message as assigned by broker
    FIX::ExecID execID;
    std::cout<<"Got ExecID: "<<message.get(execID)<<std::endl;
    
    // 20=0
    // FIX 4.2 : ExecTransType <20> field
    // Identifies transaction type
    // Valid values:
    // 0 = New
    // 1 = Cancel
    // 2 = Correct
    // 3 = Status
    FIX::ExecTransType execTransType;
    std::cout<<"Got ExecTransType: "<<message.get(execTransType)<<std::endl;

    // 31=233
    // FIX 4.2 : LastPx <31> field: Price <44> of this (last) fill.
    FIX::LastPx lastPx;
    double lastpx = message.get(lastPx);
    cout<<"Got LastPx: "<<message.get(lastPx)<<std::endl;
    order.set_lastpx(lastpx);
    
    // 32=2333
    // FIX 4.2 : LastShares <32> field: Quantity of shares bought/sold on this (last) fill.
    FIX::LastShares lastShares;
    int lastshares = message.get(lastShares);
    cout<<"Got LastShares: "<<message.get(lastShares)<<std::endl;
    order.set_lastshares(lastshares);
    
    //37=14530
    // Unique identifier for Order as assigned by broker. Uniqueness must be guaranteed within a single trading
    // day. Firms which accept multi-day orders should consider embedding a date within the OrderID <37> field to
    // assure uniqueness across days.
    FIX::OrderID orderID;
    string order_id = message.get(orderID);
    int order_id_int;
    istringstream (order_id) >> order_id_int;
    cout<<"Got OrderID: "<<message.get(orderID)<<std::endl;
    order.set_order_id(order_id_int);
    
    //38=2333
    // FIX 4.2 : OrderQty <38> field: Number of shares ordered
    FIX::OrderQty orderQty;
    double orderqty = message.get(orderQty);
    cout<<"Got OrderQty: "<<message.get(orderQty)<<std::endl;
    
    // 39=2
    // FIX 4.2 : OrdStatus <39> field
    // 0 = New
    // 1 = Partially filled
    // 2 = Filled
    // 3 = Done for day
    // 4 = Canceled
    // 5 = Replaced
    // 6 = Pending Cancel (e.g. result of Order Cancel Request <F>)
    // 7 = Stopped
    // 8 = Rejected
    // 9 = Suspended
    // A = Pending New
    // B = Calculated
    // C = Expired
    // D = Accepted for bidding
    // E = Pending Replace (e.g. result of Order Cancel/Replace Request <G>)
    FIX::OrdStatus ordStatus;
    char ordstatus = message.get(ordStatus);
    cout<<"Got OrdStatus: "<<message.get(ordStatus)<<std::endl;
    order.set_state(ordstatus);
    
    //54=1
    //FIX 4.2 : Side <54> field
    //1 = Buy
    //2 = Sell
    FIX::Side side;
    char side_ = message.get(side);
    cout<<"Got Side: "<<message.get(side)<<std::endl;
    order.set_side(side_);
    
    //55=LUV
    // FIX 4.2 : Symbol <55> field
    FIX::Symbol symbol;
    std::string symbol_ = message.get(symbol);
    cout<<"Got Symbol: "<<message.get(symbol)<<std::endl;
    order.set_symbol(symbol_);
    
    //150=2
    //FIX 4.2 : ExecType <150> field
    FIX::ExecType execType;
    cout<<"Got ExecType: "<<message.get(execType)<<std::endl;
    
    //151=0
    //FIX 4.2 : LeavesQty <151> field
    FIX::LeavesQty leavesQty;
    double leavesqty = message.get(leavesQty);
    cout<<"Got LeavesQty: "<<message.get(leavesQty)<<std::endl;
    
    //10=249
    cout<<"Order Sent Back: "<<endl;
    order.show_order();
    
    // Get PnL
    mystrategy.onMarketUpdate(order);
    
}

void Application::onMessage( const FIX42::OrderCancelReject& message, const FIX::SessionID& sessionID ){
    std::cout<<"Got A Order Cancel Reject Message: "<< message.toString()<< " "<<std::endl;
    
    Order order;
    
    //37=14530
    // Unique identifier for Order as assigned by broker. Uniqueness must be guaranteed within a single trading
    // day. Firms which accept multi-day orders should consider embedding a date within the OrderID <37> field to
    // assure uniqueness across days.
    FIX::OrderID orderID;
    string order_id = message.get(orderID);
    int order_id_int;
    istringstream (order_id) >> order_id_int;
    cout<<"Got OrderID: "<<message.get(orderID)<<std::endl;
    order.set_order_id(order_id_int);
    
    // 39=2
    // FIX 4.2 : OrdStatus <39> field
    // 0 = New
    // 1 = Partially filled
    // 2 = Filled
    // 3 = Done for day
    // 4 = Canceled
    // 5 = Replaced
    // 6 = Pending Cancel (e.g. result of Order Cancel Request <F>)
    // 7 = Stopped
    // 8 = Rejected
    // 9 = Suspended
    // A = Pending New
    // B = Calculated
    // C = Expired
    // D = Accepted for bidding
    // E = Pending Replace (e.g. result of Order Cancel/Replace Request <G>)
    FIX::OrdStatus ordStatus;
    char ordstatus = message.get(ordStatus);
    cout<<"Got OrdStatus: "<<message.get(ordStatus)<<std::endl;
    order.set_state(ordstatus);
    
    cout<<"Got a Cancelled Order sent back: "<<endl;
    order.show_order();
    
    mystrategy.onMarketUpdate(order);
}


// void Application::onMessage( const FIX42::MarketDataSnapshotFullRefresh& message, const FIX::SessionID& sessionID ) {}
// void Application::onMessage( const FIX42::MarketDataIncrementalRefresh& message, const FIX::SessionID& sessionID ) {}



void Application::onMessage( const FIX42::ExecutionReport& message){
    std::cout<<"Got A Execution Report Message: "<< message.toString()<< " "<<std::endl;
    // Got A Execution Report Message: 8=FIX.4.2   9=152  35=8  34=1332 49=SEB91  52=20170608-04:04:21.489  56=KYZHOU  6=233  11=383  14=2333  17=7265  20=0  31=233  32=2333  37=14530  38=2333  39=2  54=1  55=LUV  150=21  51=0  10=249
    
    Order order;
    
    // 35=8
    // Defines message type. ALWAYS THIRD FIELD IN MESSAGE. (Always unencrypted)
    const std::string& msgTypeValue = message.getHeader().getField( FIX::FIELD::MsgType );
    std::cout<<"Got MsgTypeValue: "<<msgTypeValue<<endl;
    
    // 34=1332
    // Integer message sequence number.
    const std::string& msgSeqNum = message.getHeader().getField( FIX::FIELD::MsgSeqNum );
    std::cout<<"Got MsgSeqNum: "<<msgSeqNum<<endl;
    
    // 49=SEB91
    // Assigned value used to identify firm sending message.
    //const std::string& senderCompID = message.getHeader().getField( FIX::FIELD::SenderCompID );
    //std::cout<<"Got SenderCompID: "<<senderCompID<<endl;
    
    // 52=20170608-04:04:21.489
    // Time of message transmission (always expressed in UTC (Universal Time Coordinated, also known as 'GMT')
    const std::string& sendingTime = message.getHeader().getField( FIX::FIELD::SendingTime);
    std::cout<<"Got SendingTime: "<<sendingTime<<endl;
    order.set_order_time(sendingTime);
    
    
    // 56=KYZHOU
    // Assigned value used to identify receiving firm.
    //const std::string& targetCompID = message.getHeader().getField( FIX::FIELD::TargetCompID );
    //std::cout<<"Got TargetCompID: "<<targetCompID<<endl;
    
    
    // 6=233
    // FIX 4.2 : AvgPx <6> field: Calculated average price of all fills on this order.
    FIX::AvgPx avgPx;
    double price = message.get(avgPx);
    std::cout<<"Got AvgPx: "<<message.get(avgPx)<<std::endl;
    order.set_price(price);
    
    // 11=383
    // FIX 4.2 : ClOrdID <11> field
    FIX::ClOrdID clOrdID;
    std::string clordid = message.get(clOrdID);
    std::cout<<"Got ClOrdID: "<<message.get(clOrdID)<<std::endl;
    order.set_clordid(clordid);
    
    // 14=2333
    // FIX 4.2 : CumQty <14> field: Total number of shares filled.
    FIX::CumQty cumQty;
    double cumqty = message.get(cumQty);
    std::cout<<"Got CumQty: "<<message.get(cumQty)<<std::endl;
    order.set_quantity(cumqty);
    
    // 17=7265
    // FIX 4.2 : ExecID <17> field : Unique identifier of execution message as assigned by broker
    FIX::ExecID execID;
    std::cout<<"Got ExecID: "<<message.get(execID)<<std::endl;
    
    // 20=0
    // FIX 4.2 : ExecTransType <20> field
    // Identifies transaction type
    // Valid values:
    // 0 = New
    // 1 = Cancel
    // 2 = Correct
    // 3 = Status
    FIX::ExecTransType execTransType;
    std::cout<<"Got ExecTransType: "<<message.get(execTransType)<<std::endl;
    
    // 31=233
    // FIX 4.2 : LastPx <31> field: Price <44> of this (last) fill.
    FIX::LastPx lastPx;
    double lastpx = message.get(lastPx);
    cout<<"Got LastPx: "<<message.get(lastPx)<<std::endl;
    order.set_lastpx(lastpx);
    
    // 32=2333
    // FIX 4.2 : LastShares <32> field: Quantity of shares bought/sold on this (last) fill.
    FIX::LastShares lastShares;
    int lastshares = message.get(lastShares);
    cout<<"Got LastShares: "<<message.get(lastShares)<<std::endl;
    order.set_lastshares(lastshares);
    
    //37=14530
    // Unique identifier for Order as assigned by broker. Uniqueness must be guaranteed within a single trading
    // day. Firms which accept multi-day orders should consider embedding a date within the OrderID <37> field to
    // assure uniqueness across days.
    FIX::OrderID orderID;
    string order_id = message.get(orderID);
    int order_id_int;
    istringstream (order_id) >> order_id_int;
    cout<<"Got OrderID: "<<message.get(orderID)<<std::endl;
    order.set_order_id(order_id_int);
    
    //38=2333
    // FIX 4.2 : OrderQty <38> field: Number of shares ordered
    FIX::OrderQty orderQty;
    double orderqty = message.get(orderQty);
    cout<<"Got OrderQty: "<<message.get(orderQty)<<std::endl;
    
    // 39=2
    // FIX 4.2 : OrdStatus <39> field
    // 0 = New
    // 1 = Partially filled
    // 2 = Filled
    // 3 = Done for day
    // 4 = Canceled
    // 5 = Replaced
    // 6 = Pending Cancel (e.g. result of Order Cancel Request <F>)
    // 7 = Stopped
    // 8 = Rejected
    // 9 = Suspended
    // A = Pending New
    // B = Calculated
    // C = Expired
    // D = Accepted for bidding
    // E = Pending Replace (e.g. result of Order Cancel/Replace Request <G>)
    FIX::OrdStatus ordStatus;
    char ordstatus = message.get(ordStatus);
    cout<<"Got OrdStatus: "<<message.get(ordStatus)<<std::endl;
    order.set_state(ordstatus);
    
    //54=1
    //FIX 4.2 : Side <54> field
    //1 = Buy
    //2 = Sell
    FIX::Side side;
    char side_ = message.get(side);
    cout<<"Got Side: "<<message.get(side)<<std::endl;
    order.set_side(side_);
    
    //55=LUV
    // FIX 4.2 : Symbol <55> field
    FIX::Symbol symbol;
    std::string symbol_ = message.get(symbol);
    cout<<"Got Symbol: "<<message.get(symbol)<<std::endl;
    order.set_symbol(symbol_);
    
    //150=2
    //FIX 4.2 : ExecType <150> field
    FIX::ExecType execType;
    cout<<"Got ExecType: "<<message.get(execType)<<std::endl;
    
    //151=0
    //FIX 4.2 : LeavesQty <151> field
    FIX::LeavesQty leavesQty;
    double leavesqty = message.get(leavesQty);
    cout<<"Got LeavesQty: "<<message.get(leavesQty)<<std::endl;
    
    //10=249
    cout<<"Order Sent Back: "<<endl;
    order.show_order();
    
    // Get PnL
    mystrategy.onMarketUpdate(order);
    
}


//NOTE: Incremental and Snapshot messages have different tags, you need to parse the appropriately
// Got A Incremental Refresh: 8=FIX.4.29=9935=X34=28049=SEB9152=20170502-02:10:55.78056=KYZHOU268=1279=1269=1270=169.16271=1337=15010=078

void Application::onMessage
(   const FIX42::MarketDataIncrementalRefresh& message, const FIX::SessionID& sessionID)
{
    std::cout<<"Got A Incremental Refresh: "<< message.toString()<< " "<<std::endl;
    
    //TODO: Inspect the message object and take the appropiate action
    Order order;
    
    // 35=X
    //const std::string& msgTypeValue = message.getHeader().getField( FIX::FIELD::MsgType );
    //std::cout<<"Got msgTypeValue: "<<msgTypeValue<<endl;//X
    
    // 268=1
    FIX::NoMDEntries noMDEntries;
    //std::cout<<"Got noMDEntries: "<<message.get(noMDEntries)<<std::endl;
    int number;
    number = message.get(noMDEntries);
    
    FIX42::MarketDataIncrementalRefresh::NoMDEntries group;
    for (unsigned int i = 1; i <= number; i++)
    {
        message.getGroup(i, group);
        
        // 8=FIX.4.2
        // Identifies beginning of new message and protocol version. ALWAYS FIRST FIELD IN MESSAGE. (Always unencrypted)
        //const std::string& beginString = message.getHeader().getField( FIX::FIELD::BeginString );
        //std::cout<<"Got BeginString: "<<beginString<<endl;
        
        // 9=99
        // Message length, in bytes, forward to the CheckSum <10> field. ALWAYS SECOND FIELD IN MESSAGE. (Always unencrypted)
        //const std::string& bodyLength = message.getHeader().getField( FIX::FIELD::BodyLength );
        //std::cout<<"Got BodyLength: "<<bodyLength<<endl;
        
        
        // 35=X
        // Defines message type. ALWAYS THIRD FIELD IN MESSAGE. (Always unencrypted)
        const std::string& msgTypeValue = message.getHeader().getField( FIX::FIELD::MsgType );
        std::cout<<"Got MsgTypeValue: "<<msgTypeValue<<endl;//X
        
        // 34=280
        // Integer message sequence number.
        const std::string& msgSeqNum = message.getHeader().getField( FIX::FIELD::MsgSeqNum );
        std::cout<<"Got MsgSeqNum: "<<msgSeqNum<<endl;
        
        // 49=SEB91
        // Assigned value used to identify firm sending message.
        //const std::string& senderCompID = message.getHeader().getField( FIX::FIELD::SenderCompID );
        //std::cout<<"Got SenderCompID: "<<senderCompID<<endl;
        
        
        // 52=20170502-02:10:55.780
        // Time of message transmission (always expressed in UTC (Universal Time Coordinated, also known as 'GMT')
        const std::string& sendingTime = message.getHeader().getField( FIX::FIELD::SendingTime);
        //string order_time = sendingTime;
        std::cout<<"Got SendingTime: "<<sendingTime<<endl;
        //order.order_time = sendingTime;
        order.set_order_time(sendingTime);
        midprice.ts = sendingTime;
        
        
        // 56=KYZHOU
        // Assigned value used to identify receiving firm.
        //const std::string& targetCompID = message.getHeader().getField( FIX::FIELD::TargetCompID );
        //std::cout<<"Got TargetCompID: "<<targetCompID<<endl;
        
        // 268=1
        // Number of entries in Market Data message.
        // FIX::NoMDEntries noMDEntries;
        std::cout<<"Got noMDEntries: "<<message.get(noMDEntries)<<std::endl;
        
        // 279=1
        // Type of Market Data update action.
        // Valid values:
        // 0 = New
        // 1 = Change
        // 2 = Delete
        FIX::MDUpdateAction mdUpdateAction;
        char message_type = (char)group.get(mdUpdateAction);
        cout<<"Got MDUpdateAction: "<<group.get(mdUpdateAction)<<std::endl;
        order.set_message_type(message_type);
        
        // 269=1
        // Type Market Data entry.
        // Valid values:
        // 0 = Bid
        // 1 = Offer
        // 2 = Trade
        // 3 = Index Value
        // 4 = Opening Price <44>
        // 5 = Closing Price <44>
        // 6 = Settlement Price <44>
        // 7 = Trading Session High Price <44>
        // 8 = Trading Session Low Price <44>
        // 9 = Trading Session VWAP Price <44>
        FIX::MDEntryType mdEntryType;
        int side = group.get(mdEntryType)- '0';
        cout<<"Got MDEntryType: "<<group.get(mdEntryType)<<std::endl;
        order.set_side(side);
        
        // 270=169.16
        // Price <44> of the Market Data Entry.
        FIX::MDEntryPx mdEntryPx;
        double price = group.get(mdEntryPx);
        cout<<"Got MDEntryPx: "<<group.get(mdEntryPx)<<std::endl;
        order.set_price(price);
        midprice.price = price;
        
        // 271=13
        // Number of shares represented by the Market Data Entry.
        FIX::MDEntrySize mdEntrySize;
        int quantity = group.get(mdEntrySize);
        cout<<"Got MDEntrySize: "<<group.get(mdEntrySize)<<std::endl;
        order.set_quantity(quantity);
        midprice.volume = quantity;
        
        // 37=150
        // Unique identifier for Order as assigned by broker. Uniqueness must be guaranteed within a single trading
        // day. Firms which accept multi-day orders should consider embedding a date within the OrderID <37> field to
        // assure uniqueness across days.
        FIX::OrderID orderID;
        string order_id = group.get(orderID);
        int order_id_int;
        istringstream (order_id) >> order_id_int;
        cout<<"Got OrderID: "<<group.get(orderID)<<std::endl;
        order.set_order_id(order_id_int);
        
        // Does not work
        // 10=078
        // ALWAYS LAST FIELD IN MESSAGE; i.e. serves, with the trailing <SOH>, as the end-of-message delimiter.
        // Always defined as three characters. (Always unencrypted)
        // FIX::CheckSum checkSum;
        // cout<<"Got CheckSum: "<<group.get(checkSum)<<std::endl;
        // const std::string& checkSum = message.getHeader().getField( FIX::FIELD::CheckSum);
        // std::cout<<"Got CheckSum: "<<checkSum<<endl;
        cout<<"Order: "<<endl;
        order.show_order();
        //orderbook->process_order(order);
        orderbook.process_order(order);
        
        midprice.symbol = orderbook.symbol;
        
        //queryNewOrderSingle42_initial();
        
        if (midprice.symbol == "AAL" || midprice.symbol =="CEA" || midprice.symbol =="DAL"){
        
        mystrategy.onBookUpdate(midprice);
        if (mystrategy.signal_side != '0'){
            queryNewOrderSingle42(midprice.symbol, mystrategy.signal_side, quantity, price);}
        }
    }
    
}



void Application::onMessage
( const FIX42::MarketDataSnapshotFullRefresh& message, const FIX::SessionID& sessionID ) {
    //TODO: Inspect the message object and take the appropiate action
    std::cout<<"Got A Snapshot: "<< message.toString() << " "<<std::endl;
    //Got A Snapshot:            8=FIX.4.29=128 35=W 34=2   49=SEB91 52=20170505-03:06:01.151
    //  56=KYZHOU 55=DDD 268=2       269=0 270=12.94  271=10 37=1289 269=1 270=12.96 271=5 37=1290 10=213
    //Got A Incremental Refresh: 8=FIX.4.29=98  35=X 34=334 49=SEB91 52=20170505-01:42:55.983
    //  56=KYZHOU        268=1 279=1 269=0 270=169.2  271=6  37=1181 10=042
    Order order_2;
    //55=MMM
    //Ticker symbol
    //const std::string& symbol = message.getHeader().getField( FIX::FIELD::Symbol );
    FIX::Symbol symbol;
    std::string ticker = message.get(symbol);
    std::cout<<"Got Symbol: "<<ticker<<endl;
    //order_2.symbol = ticker;
    order_2.set_symbol(ticker);
    midprice.symbol = ticker;
    
    // 268=1
    FIX::NoMDEntries noMDEntries;
    //std::cout<<"Got noMDEntries: "<<message.get(noMDEntries)<<std::endl;
    int number;
    number = message.get(noMDEntries);
    
    FIX42::MarketDataIncrementalRefresh::NoMDEntries group;
    for (unsigned int i = 1; i <= number; i++)
    {
        //Order order_2;
        
        message.getGroup(i, group);
        
        char message_type = '0';
        order_2.set_message_type(message_type);
        std::cout<<"Got Message Type: "<<message_type<<endl;
        
        // 35=X
        // Defines message type. ALWAYS THIRD FIELD IN MESSAGE. (Always unencrypted)
        const std::string& msgTypeValue = message.getHeader().getField( FIX::FIELD::MsgType );
        std::cout<<"Got MsgTypeValue: "<<msgTypeValue<<endl;//X
        
        // 34=280
        // Integer message sequence number.
        const std::string& msgSeqNum = message.getHeader().getField( FIX::FIELD::MsgSeqNum );
        std::cout<<"Got MsgSeqNum: "<<msgSeqNum<<endl;
        
        // 52=20170502-02:10:55.780
        // Time of message transmission (always expressed in UTC (Universal Time Coordinated, also known as 'GMT')
        const std::string& sendingTime = message.getHeader().getField( FIX::FIELD::SendingTime);
        std::cout<<"Got SendingTime: "<<sendingTime<<endl;
        //order_2.order_time = sendingTime;
        order_2.set_order_time(sendingTime);
        midprice.ts = sendingTime;
        
        // 268=1
        // Number of entries in Market Data message.
        // FIX::NoMDEntries noMDEntries;
        std::cout<<"Got noMDEntries: "<<message.get(noMDEntries)<<std::endl;
        
        
        // 269=1
        // Type Market Data entry.
        // Valid values:
        // 0 = Bid
        // 1 = Offer
        // 2 = Trade
        // 3 = Index Value
        // 4 = Opening Price <44>
        // 5 = Closing Price <44>
        // 6 = Settlement Price <44>
        // 7 = Trading Session High Price <44>
        // 8 = Trading Session Low Price <44>
        // 9 = Trading Session VWAP Price <44>
        FIX::MDEntryType mdEntryType;
        int side = group.get(mdEntryType)- '0';
        cout<<"Got MDEntryType: "<<group.get(mdEntryType)<<std::endl;
        order_2.set_side(side);
        
        // 270=169.16
        // Price <44> of the Market Data Entry.
        FIX::MDEntryPx mdEntryPx;
        double price = group.get(mdEntryPx);
        cout<<"Got MDEntryPx: "<<group.get(mdEntryPx)<<std::endl;
        order_2.set_price(price);
        midprice.price = price;
        
        // 271=13
        // Number of shares represented by the Market Data Entry.
        FIX::MDEntrySize mdEntrySize;
        int quantity = group.get(mdEntrySize);
        cout<<"Got MDEntrySize: "<<group.get(mdEntrySize)<<std::endl;
        order_2.set_quantity(quantity);
        midprice.volume = quantity;
        
        // 37=150
        // Unique identifier for Order as assigned by broker. Uniqueness must be guaranteed within a single trading
        // day. Firms which accept multi-day orders should consider embedding a date within the OrderID <37> field to
        // assure uniqueness across days.
        FIX::OrderID orderID;
        string order_id = group.get(orderID);
        int order_id_int;
        istringstream (order_id) >> order_id_int;
        cout<<"Got OrderID: "<<group.get(orderID)<<std::endl;
        order_2.set_order_id(order_id_int);
        
        // Does not work
        // 10=078
        // ALWAYS LAST FIELD IN MESSAGE; i.e. serves, with the trailing <SOH>, as the end-of-message delimiter.
        // Always defined as three characters. (Always unencrypted)
        // FIX::CheckSum checkSum;
        // cout<<"Got CheckSum: "<<group.get(checkSum)<<std::endl;
        // const std::string& checkSum = message.getHeader().getField( FIX::FIELD::CheckSum);
        // std::cout<<"Got CheckSum: "<<checkSum<<endl;
        //order.set_symbol(ticker);
        cout<<"Order: "<<endl;
        order_2.show_order();
        orderbook.process_order(order_2);
        //queryNewOrderSingle42_initial();

        if (midprice.symbol == "AAL" || midprice.symbol =="CEA" || midprice.symbol =="DAL"){
        mystrategy.onBookUpdate(midprice);
        if (mystrategy.signal_side != '0'){
            queryNewOrderSingle42(midprice.symbol, mystrategy.signal_side, quantity, price);}
        }
    }
}
 

void Application::Cancel_All_Orders_on_Market()
{
    std::cout << "\nCancel_All_Orders_on_Market\n";
    for (auto it=mystrategy.order_to_delete_map.rbegin();it!=mystrategy.order_to_delete_map.rend();it--)
    {
        std::string origclordid_temp = std::to_string(it->second.m_order_id);
        std::string symbol_temp = it->second.m_symbol;
        char side_temp = it->second.m_side;
        int quantity_temp = it->second.m_quantity;
        double price_temp =it->second.m_price;
        queryOrderCancelRequest42(origclordid_temp, symbol_temp, side_temp, quantity_temp, price_temp);
    }
    auto it=mystrategy.order_to_delete_map.begin();
    std::string origclordid_temp = std::to_string(it->second.m_order_id);
    std::string symbol_temp = it->second.m_symbol;
    char side_temp = it->second.m_side;
    int quantity_temp = it->second.m_quantity;
    double price_temp =it->second.m_price;
    queryOrderCancelRequest42(origclordid_temp, symbol_temp, side_temp, quantity_temp, price_temp);
    mystrategy.cancel_All_orders();

}

