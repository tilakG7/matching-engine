#include <iostream>
#include "BaseOrder.h"

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

    BaseOrder my_tesla_order(
        0,              // order ID
        11611510897llu, // security ID for tesla
        BaseOrder::OrderType::kMarket,
        true     
    );

    my_tesla_order.print();
}