#include <iostream>
#include <set>
#include <string>

#include "common/template_helper.h"
#include "orders/base_order.h"
#include "orders/limit_order.h"
#include "orders/market_order.h"
#include "orders/stop_limit_order.h"
#include "orders/stop_order.h"


class Exchange {
public:
    Exchange() = default;
    ~Exchange() = default;

    // checks whether a security exists (given my string name)
    // returns true/false to indicate existance
    bool securityExists(std::string tckr) const {
        return (security_map_.find(tckr) != security_map_.end());
    }

    // returns the corresponding id of the security's ticker symbol
    uint64_t getSecurityId(std::string tckr) const {
        // check for invalid security ID
        if(!securityExists(tckr)) {
            return 0;
        }
        return security_map_.at(tckr);
    }

    double getCurrentSharePrice(uint64_t security_id) const {
        if(price_map_.find(security_id) == price_map_.end()) {
            return -1.;
        }
        return price_map_.at(security_id);
    }
private:
    // maps id of financial security to struct containing more info
    const std::unordered_map<std::string, uint64_t>  security_map_{
        {"TSLA", 1},
        {"AMZN", 2},
        {"AAPL", 3},
        {"AMD" , 4},
        {"NVDA", 5},
        {"GOOG", 6},
        {"ASML", 7},
        {"META", 8}
    };

    const std::unordered_map<uint64_t, double> price_map_{
        {1, 238.45},
        {2, 148.47},
        {3, 184.25},
        {4, 135.32},
        {5, 475.69},
        {6, 140.36},
        {7, 703.37},
        {8, 344.47}
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
    unique_ptr<BaseOrder> order{};

    // get the security ticker symbol
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


    // is the order a bid or an ask?
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

    // check if the user has entered a valid quantity
    bool is_valid_quantity{};
    int quantity{};
    do {
        cout << "Enter quantity to " << (is_bid ? "sell" : "buy") << ": ";
        string str_quantity{};
        cin >> str_quantity;
        try {
            quantity = std::stoi(str_quantity);
            if(quantity <= 0) {
                cout << "Quantity must not be negative. Try again." << endl;
            } 
            else {
                is_valid_quantity = true;
            }
        } catch(const std::invalid_argument& e) {
            cout << "Error. Argument is invalid. Try again." << endl;
        } catch(const std::out_of_range& e) {
            cout << "Error. Number is out of range. Try again." << endl;
        }    
    }while(!is_valid_quantity);
    
    // get order type
    int order_type{};
    bool is_valid_order_type{};
    do {
        cout << "What kind of order is this? Choose: \n"
            << "(0) Market\n"
            << "(1) Stop\n"
            << "(2) Limit\n"
            << "(3) Stop limit\n"
            << "(4) Stop order\n"
            << "Enter a digit (0-4): ";
        cin >> order_type;
        if(!cin.good() || (order_type > 4) || (order_type < 0)) {
            std::cin.clear();  // Clear error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Discard invalid input
            std::cout << "Error, try again. Enter valid input.\n";
        }
        else {
            is_valid_order_type = true;
        }
    } while(!is_valid_order_type);

    OrderType ot{static_cast<OrderType>(order_type)};
    cout << "A " << kOrderTypeToStr[to_underlying(ot)] << " order will be placed.\n";

    double limit_price{};
    double stop_price{};

    if(ot == OrderType::kLimit || ot == OrderType::kStopLimit) {
        while(true) {
            cout << "Enter limit price: ";
            cin >> limit_price;
            if(cin.fail() || limit_price <= 0.) {
                cin.clear();
                cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
                cout << "Invalid input. Provide a positive double value.\n";
            } else {
                cout << "Limit price set to " << limit_price << endl;
                break;
            }
        }
    }
    
    if(ot == OrderType::kStop || ot == OrderType::kStopLimit) {
        while(true) {
            cout << "Enter stop price: ";
            cin >> stop_price;
            if(cin.fail() || stop_price <= 0.) {
                cin.clear();
                cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
                cout << "Invalid input. Provide a positive double value.\n";
            } else {
                cout << "Stop price set to " << stop_price << endl;
                break;
            }
        }
    }

    switch(ot) {
        case OrderType::kLimit:
            order.reset(new LimitOrder(1234, 
                                        e.getSecurityId(ticker), 
                                        quantity, 
                                        is_bid,
                                        limit_price));
        break;
        case OrderType::kMarket:
            order.reset(new MarketOrder(1234,
                                        e.getSecurityId(ticker), 
                                        quantity, 
                                        is_bid));
        break;
        case OrderType::kStop:
            order.reset(new StopOrder(1234,
            e.getSecurityId(ticker), 
            quantity,
            is_bid,
            stop_price,
            (stop_price > e.getCurrentSharePrice(e.getSecurityId(ticker))) ? true : false));
        break;
        case OrderType::kStopLimit:
            order.reset(new StopLimitOrder(1234,
            e.getSecurityId(ticker),
            quantity,
            is_bid,
            limit_price,
            stop_price,
            (stop_price > e.getCurrentSharePrice(e.getSecurityId(ticker))) ? true : false));
        break;
        default:
            cout << "Unknown order type. Exiting" << endl;
            return -1;
    }

    order->print();







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