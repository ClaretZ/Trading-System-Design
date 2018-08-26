# Trading System Design in C++

## How to Use

In terminal, type:
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

## Outcome Example
The strategy uses AAL, CEA, DAL.

The order book's apperence:
![screen shot 1](https://user-images.githubusercontent.com/29934837/44633492-d4a79e00-a951-11e8-8e14-e8d3d6665e86.png)


When a position is opened, the performance summary looks like:
![screen shot 2](https://user-images.githubusercontent.com/29934837/44633516-06b90000-a952-11e8-821f-d2c850c7686c.png)


When a position is closed, the performance summary looks like:
![screen shot 3](https://user-images.githubusercontent.com/29934837/44633535-1f291a80-a952-11e8-8fac-ae63da8e136a.png)

















