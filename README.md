- export env.sh
  export LD_LIBRARY_PATH=/usr/local/quickfix/lib
- then make
- then ./marketdata

If the words below shows out, please first put in "1", and then choose whatever you like.

0) Quit
1) Market data test & begin strategy
2) Stop showing the detailed data
3) Start showing the detailed data
4) Start strategy
5) Stop strategy
6) Cancel All orders on market
7) Show total position on market
8) Get Cumulative P&L
Action: 


The strategy uses AAL, CEA, DAL.

The order book's apperence:

******************************************************************************* Order Book ********************************************************************************
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
*********************************************************************************** BID ***********************************************************************************
Ticker            quantity               price                side            message_type          order_time           order_id                type               state
ALK                4999                  65.08                                     1         20170609-02:06:19.345          15                   1                   0
CPA                 497                   64.8                                     1         20170609-02:06:17.867          23                   1                   0
UAL                 496                  44.33                                     1         20170609-02:06:19.410          31                   1                   0
DAL                1997                  36.55                                     1         20170609-02:06:19.425          13                   1                   0
LUV                2999                  35.61                                     1         20170609-02:06:19.359          29                   1                   0
AAL                 997                  33.72                                     1         20170609-02:06:17.872           9                   1                   0
ZNH                4999                   32.3                                     1         20170609-02:06:19.421          19                   1                   0
CEA                1997                  27.01                                     1         20170609-02:06:17.871          11                   1                   0
VLRS                 997                  17.85                                     1         20170609-02:06:17.854          21                   1                   0
GOL                1997                  15.27                                     1         20170609-02:06:17.869          25                   1                   0
LFL                4997                   8.45                                     1         20170609-02:06:17.871          27                   1                   0
AVH                 497                   5.88                                     1         20170609-02:06:17.871          17                   1                   0

*********************************************************************************** ASK ***********************************************************************************
Ticker            quantity               price                side            message_type          order_time           order_id                type               state
AVH                1003                   5.82                                     1         20170609-02:06:17.871          18                   1                   0
LFL                3003                   8.47                                     1         20170609-02:06:17.871          28                   1                   0
GOL                1003                   15.3                                     1         20170609-02:06:17.869          26                   1                   0
VLRS                 503                  17.76                                     1         20170609-02:06:17.858          22                   1                   0
CEA                1003                  27.02                                     1         20170609-02:06:17.871          12                   1                   0
NaN                5001                  32.34                                     1         20170609-02:06:19.422          20                   1                   0
AAL                 503                  33.68                                     1         20170609-02:06:17.872          10                   1                   0
NaN                5001                  35.71                                     1         20170609-02:06:19.359          30                   1                   0
DAL                 503                  36.63                                     1         20170609-02:06:19.425          14                   1                   0
UAL                3003                  44.35                                     1         20170609-02:06:17.406          32                   1                   0
CPA                2003                  64.73                                     1         20170609-02:06:17.868          24                   1                   0
NaN                5001                  65.17                                     1         20170609-02:06:19.345          16                   1                   0
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------








When a position is opened, the performance summary looks like:

Got A Execution Report Message: 8=FIX.4.29=15235=834=175049=SEB9152=20170609-02:06:26.02956=KYZHOU6=27.0111=9414=499617=13320=031=27.0132=499637=26538=499639=054=155=CEA150=0151=010=236 
Got MsgTypeValue: 8
Got MsgSeqNum: 1750
Got SendingTime: 20170609-02:06:26.029
Got AvgPx: 27.01
Got AvgPx: 94
Got CumQty: 4996
Got ExecID: 133
Got ExecTransType: 0
Got LastPx: 27.01
Got LastShares: 4996
Got OrderID: 265
Got OrderQty: 4996
Got OrdStatus: 0
Got Side: 1
Got Symbol: CEA
Got ExecType: 0
Got LeavesQty: 0
Order Sent Back: 
CEA                4996                  27.01                  1                   1         20170609-02:06:26.029         265                   1                   0
Market response:
CEA                4996                  27.01                  1                   1         20170609-02:06:26.029         265                   1                   0
FILLED buy order:
Order id: 265
Price: 27.01
BUY Quantity: 4996
Total Quantity of CEA: 4996
Account: -1.79794e+06
Position of CEA on Market: 134942
Total Position on Market: 134942







When a position is closed, the performance summary looks like:

Got A Execution Report Message: 8=FIX.4.29=14335=834=174849=SEB9152=20170609-02:06:26.02956=KYZHOU6=2711=9314=99717=13220=031=2732=99737=26338=99739=054=255=CEA150=0151=010=057 
Got MsgTypeValue: 8
Got MsgSeqNum: 1748
Got SendingTime: 20170609-02:06:26.029
Got AvgPx: 27
Got AvgPx: 93
Got CumQty: 997
Got ExecID: 132
Got ExecTransType: 0
Got LastPx: 27
Got LastShares: 997
Got OrderID: 263
Got OrderQty: 997
Got OrdStatus: 0
Got Side: 2
Got Symbol: CEA
Got ExecType: 0
Got LeavesQty: 0
Order Sent Back: 
CEA                 997                     27                  2                   1         20170609-02:06:26.029         263                   1                   0
Market response:
CEA                 997                     27                  2                   1         20170609-02:06:26.029         263                   1                   0
FILLED sell order:
Order id: 263
Price: 27
SELL Quantity: 997
Cumulative PnL = -1.663e+06
The final cumulative P&L is -1.663e+06
Position of CEA on Market: 0
Total Position on Market: 134996





















