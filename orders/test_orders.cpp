#include <iostream>
#include "base_order.h"
#include "market_order.h"
#include "limit_order.h"
#include "stop_limit_order.h"
#include "stop_order.h"

using namespace std;
int main(int argc, char *argv[]) {
    // print program name
    if(argc > 0) {
        cout << "Program name: " << argv[0] << "\n"; 
    }

    // print remaining arguments
    for(int i{1}; i < argc; i++) {
        cout << "Arg #" << i << ": " << argv[i] << "\n";
    }
    cout << "\n\n";

    MarketOrder my_tesla_order(
        0,              // order ID
        11611510897llu, // security ID for tesla
        339,            // quantity of shares
        true     
    );

    LimitOrder my_tesla_order_2(
        1,              // order ID
        11611510897llu, // security ID for tesla
        954,            // quantity of shares
        false,          // not a bid
        250             // limit price
    );

    StopLimitOrder my_tesla_order_3(
        2,              // order ID
        11611510897llu, // security ID for tesla
        1234,           // quantity of shares
        false,          // not a bid
        500,            // limit price 
        505,            // stop price
        false           // must NOT exceed
    );

    StopOrder my_tesla_order_4(
        3,              // order ID
        11611510897llu, // security ID for tesla
        432,            // quantity of shares
        true,           // bid
        555,            // stop price
        true            // must NOT exceed
    );

    my_tesla_order.print();
    my_tesla_order_2.print();
    my_tesla_order_3.print();
    my_tesla_order_4.print();
}