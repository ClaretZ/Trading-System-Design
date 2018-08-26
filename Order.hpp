#ifndef ORDERMATCH_ORDER_H
#define ORDERMATCH_ORDER_H

#include <string>
#include <iomanip>
#include <ostream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

enum Type { MARKET , LIMIT};
// enum State { PENDING , REJECTED, ACK, FILLED, CANCELLED };


class Order
{
public:
    std::string m_symbol = "NaN";
    int m_quantity; //271
    double m_price; //270
    char m_side; //269 //1 = Buy//2 = Sell
    char m_message_type;//insert, modify, delete //279
    std::string m_order_time; //52
    int m_order_id; //37
    Type m_type = LIMIT; // MARKET, LIMIT
    char m_state = '0'; // 0 = New
    double m_lastpx = 0;
    int m_lastshares = 0;
    
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
    std::string m_clordid = "NaN";
    int m_leave_quantity = 0;
    
    Order(std::string sy, int qty, double pri, char s, char mtyp, std::string odtime, int id, Type type, char state, std::string clordid,int leaveqty):m_symbol(sy),m_quantity(qty), m_price(pri), m_side(s), m_message_type(mtyp), m_order_time(odtime), m_order_id(id), m_type(type), m_state(state), m_clordid(clordid), m_leave_quantity(leaveqty){}
    Order() {}
    ~Order(){}
    
    void change_symbol(string sss){m_symbol = sss;}
    // get functions
    std::string get_symbol(){ return m_symbol;}
    int get_quantity(){return m_quantity;}
    double get_price(){return m_price;}
    char get_side(){return m_side;}
    char get_message_type(){return m_message_type;}
    std::string get_order_time() {return m_order_time;}
    int get_order_id() {return m_order_id;}
    Type get_type() {return m_type;}
    char get_state(){return m_state;}
    std::string get_clordid(){ return m_clordid;}
    int get_leave_quantity(){return m_leave_quantity;}
    double get_lastpx(){return m_lastpx;}
    int get_lastshares(){return m_lastshares;}
    
    // set functions
    void set_symbol(std::string sy){m_symbol = sy;}
    void set_quantity(int qty){m_quantity = qty;}//271
    void set_price(double pri){m_price = pri;} //270
    void set_side(char s){m_side = s;} //269
    void set_message_type(char mtyp){m_message_type = mtyp;}//279
    void set_order_time(std::string odtime){m_order_time = odtime;} //52
    void set_order_id(int id){m_order_id = id; } //37
    void set_type(Type type) {m_type = type;}
    void set_state(char state){m_state = state;}
    void set_clordid(std::string clordid){ m_clordid = clordid;}
    void set_leave_quantity(int leaveqty ){ m_leave_quantity = leaveqty;}
    void set_lastpx(double lastpx){ m_lastpx = lastpx;}
    void set_lastshares(int lastshares){m_lastshares = lastshares;}
    
    
    // show order
    void show_order(){
        cout<<m_symbol<<setw(20)<<m_quantity<<setw(23)<<m_price<<setw(19)<<m_side<<setw(20)<<m_message_type<<setw(30)<<m_order_time<<setw(12)<<m_order_id<< setw(20)<<m_type<<setw(20)<<m_state<<endl;
        //setw(20)<<m_clordid<<<<setw(20)<<m_leave_quantity<<endl;
    }
};
#endif
