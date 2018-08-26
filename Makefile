all: Application.cpp Application.h main.cpp
	g++ Application.cpp main.cpp  -I /usr/local/quickfix/include -I /usr/local/quickfix/include/quickfix -L /usr/local/quickfix/lib -lquickfix -w -lboost_system -o marketdata

unit_test: 
	g++ Application.cpp unit_test.cpp -o unit_test -w -I /usr/local/quickfix/include -I /usr/local/quickfix/include/quickfix -L /usr/local/quickfix/lib -lquickfix -lboost_unit_test_framework

clean:
	rm -f *.o a.out marketdata
