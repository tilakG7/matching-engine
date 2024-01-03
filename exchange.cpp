#include <iostream>
#include <set>
#include <string>

#include "common/template_helper.h"
#include "orders/base_order.h"


class Exchange {
public:
    Exchange() = default;
    ~Exchange() = default;

    // checks whether a security exists (given my string name)
    // returns true/false to indicate existance
    bool securityExists(std::string tckr) {
        return (security_map_.find(tckr) != security_map_.end());
    }
private:
    // maps id of financial security to struct containing more info
    const std::unordered_map<std::string, uint32_t>  security_map_{
        {"TSLA", 1},
        {"AMZN", 2},
        {"AAPL", 3},
        {"AMD" , 4},
        {"NVDA", 5},
        {"GOOG", 6},
        {"ASML", 7},
        {"META", 8}
    };
    // std::unordered_map<uint32_t, std::pair<StopOrderBook<true>, StopOrderBook<false>> inactive_orders_{};
};


using namespace std;
using namespace boost::posix_time;

void printTime(ptime &&t) {
    std::cout << "clock right now: " << t << "\n";
}

// Goal for today:
// 1. get the program to loop until valid security is entered
// 2. get the program to loop until valid buy/sell is placed
// 3. get the program to loop until valid share quantity is placed
// 4. which order type


int main() {
    Exchange e{};

    std::string ticker{};
    do {
        std::cout << "Enter security symbol: ";
        std::cin >> ticker;

        if(e.securityExists(ticker)) {
            cout << "Verified.\n" << ticker << ".\nSecurity exists.\n";
        }
        else {
            cout << "Error: invalid security. Try again.\n";
        }
    } while(!e.securityExists(ticker));


    
    bool is_bid{};
    std::string buy_or_sell{};
    do {  
        std::cout << "Buy or sell? (B/S): ";
        std::cin >> buy_or_sell;
        if (buy_or_sell == "B") {
            is_bid = true;
            cout << "Buying.\n";
        } else if (buy_or_sell == "S") {
            is_bid = false;
            cout << "Selling.\n";
        } else {
            cout << "Invalid input. Try again.\n";
        }
    } while((buy_or_sell != "B") && (buy_or_sell != "S"));

    bool is_valid_quantity{};
    uint64_t quantity{};
    do {
        cout << "Enter quantity to " << (is_bid ? "sell" : "buy") << ": ";
        if(!(cin >> quantity)) {
            std::cin.clear();  // Clear error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Discard invalid input
            std::cout << "Error, try again. Enter valid input.\n";
        }
        else {
            cout << quantity << endl;
            is_valid_quantity = true;
        }
    }while(!is_valid_quantity);
    
    int order_type{};
    bool is_valid_order_type{};
    do {
        cout << "What kind of order is this? Choose: \n"
            << "(0) Market\n"
            << "(1) Limit\n"
            << "(2) Stop limit\n"
            << "(3) Stop order\n"
            << "Enter a digit (0-3): ";
        cin >> order_type;
        if(!cin.good() || (order_type > 3) || (order_type < 0)) {
            std::cin.clear();  // Clear error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Discard invalid input
            std::cout << "Error, try again. Enter valid input.\n";
        }
        else {
            is_valid_order_type = true;
        }
    } while(!is_valid_order_type);


    OrderType ot{static_cast<OrderType>(order_type)};
    cout << "You want to place a " << kOrderTypeToStr[to_underlying(ot)] << " order.\n";

    if(ot == OrderType::kLimit || ot == OrderType::kStopLimit) {
        cout << "Enter limit price: ";
        double limit_price{};
        cin >> limit_price;
        cout << "The limit price you entered is: " << limit_price << std::endl;
    }
    
    if(ot == OrderType::kStop || ot == OrderType::kStopLimit) {
        cout << "Enter stop price: ";
        double stop_price{};
        cin >> stop_price;
        cout << "The stop price you entered is: " << stop_price << std::endl;
    }







    // Ask the user which security they would like to buy

    // ptime t{microsec_clock::local_time()};
    // // printTime(std::move(t));
    // std::cout << "Time: " << t << std::endl;

    // Order o1{OrderType::kMarket, 0, false, 2, 430.0, 0.0, false, t};
    // Order o2{OrderType::kMarket, 0, false, 2, 430.1, 0.0, false, t};
    // OrderBook ob{};
    // ob.addOrder(o1);
    // ob.addOrder(o2);
    // ob.print();

    // StopOrderBook<true> s;
    // s.addOrder(o1);
}


// What all does the exchange need to do?
// 1. Everytime a new order comes in, try to match it
// 2. Store orders that are not yet active: maybe store in order of which 
// orders will become active

// How to store orders that are not yet active?
// What determines when those orders become active?
    // the price of the financial instrument going above or below a stop price
    // above or below is determined by the stop price
    // ex: currently, stock is at 220
    // stop order 1 says once stock hits 260, sell (lock in profits)
    // stop order 2 says once stock hits 200, sell (protect losses)
// So how can I store inactive orders efficiently?
    // a mapping of each financial instrument to an "inactive order book"
// stop/stop-limit orders become active at a target price
// target price or better (I'm assuming)
// each financial instrument should have a sort of queue. 