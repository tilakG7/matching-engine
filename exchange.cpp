#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <queue>


#include "common/template_helper.h"
#include "common/order_book.h"

#include "orders/base_order.h"
#include "orders/limit_order.h"
#include "orders/market_order.h"
#include "orders/stop_limit_order.h"
#include "orders/stop_order.h"


class Exchange {
public:
    Exchange() {
        for(uint32_t security_id{1U}; security_id <= kNumSecurities; security_id++) {
            map_security_mo_[security_id] = MOBPair();
        }
    }
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

    void addOrder(unique_ptr<BaseOrder> order) {
        if(order->order_type_ == OrderType::kMarket) {
            matchMarketOrder(std::move(order));
        }
        else if (order->order_type_ == OrderType::kLimit) {
            matchLimitOrder(std::move(order));
        }
        // else if(order->order_type_ == OrderType::kStopLimit) {
        //     e.stop_limit_order_book_.addOrder(std::move(oder));
        //     e.stop_limit_order_book_.print();
        // }
        // else if(order->orde_type_ == OrderType::kStop) {
        //     e.stop_order_book_.addOrder(std::move(order));
        //     e.stop_order_book_.print();
        // }
        // else {
        //     std::cout << "Error: invalid order type." << std::endl;
        // }
    }

    vector<unique_ptr<BaseOrder>> bid_market_order_book_{};
    vector<unique_ptr<BaseOrder>> ask_market_order_book_{};
    OrderBook bid_limit_order_book_{};
    OrderBook ask_limit_order_book_{};
    OrderBook bid_stop_limit_order_book_{};
    OrderBook bid_stop_order_book_{};
private:
    void completeOrder(unique_ptr<BaseOrder> order) {
        std::cout << "Order with order ID: " << order->order_id_ << " completed"
                  << endl;
    }

    void completeOrderWithMarketOrder(std::unique_ptr<BaseOrder> order_to_complete, bool mo_is_bid) {
        if(mo_is_bid) {
            completeOrder(std::move(
                map_security_mo_[order_to_complete->security_id_].bid_side.front()));
            map_security_mo_[order_to_complete->security_id_].bid_side.pop();
        } else {
            completeOrder(std::move(
                map_security_mo_[order_to_complete->security_id_].ask_side.front()));
            map_security_mo_[order_to_complete->security_id_].ask_side.pop();

        }
        completeOrder(order_to_complete);
    }

    void completeOrderWithLimitOrder(std::unique_ptr<BaseOrder> order_to_complete, bool lo_is_bid) {
        if(lo_is_bid) {


        } else {

        }
        completeOrder(order_to_complete);
    }

    void matchLimitOrder(unique_ptr<BaseOrder> order) {
        bool market_match_possible{};
        bool limit_match_possible{};

        if(order->is_bid_) {
            market_match_possible = (price_map_[order->security_id_] <= order->limit_price_) && map_security_mo_.ask_side.size();
            limit_match_possible =  map_security_lo_.getSize() && map_security_lo_.peek()->limit_price <= order->limit_price;

            double market_price{};
            double limit_order_price{};
            if(market_match_possible && limit_match_possible) {
                market_price = price_map_[order->security_id_];
                limit_order = map_security_lo.peek()->limit_price;
                if(market_price < limit_price) {
                    completeOrderWithMarketOrder(std::move(order), !order->is_bid_);
                }
                else {
                    completeOrderWithLimitOrder(std::move(order), !order->is_bid_);
                }
            }
            else if(limit_order_price) {
                // match w limit order
            }
            else if(market_order_price) {
                // match w market order
            }
            else {
                // add order to limit order book
                map_security_lo_.bid_side.addOrder(std::move(order));
            }
        } else {
            
        }
    }

    void matchMarketOrder(unique_ptr<BaseOrder> order) {
        MOBPair &pair_ref{map_security_mo_[order->security_id_]};
        if(order->is_bid_) {
            // perform partial matches as possible
            // iterate throught the orders that are on the ask side 
            while(pair_ref.ask_side.size() && order)
            {
                // get the quantity of shares from most recent
                // ask order
                uint64_t ask_order_quantity = pair_ref.ask_side.front()->quantity_;
                if(ask_order_quantity == order->quantity_) {
                    // this case means both the bid and ask orders are complete
                    completeOrder(std::move(order));
                    completeOrder(std::move(pair_ref.ask_side.front()));
                    pair_ref.ask_side.pop();
                }
                else if(ask_order_quantity > order->quantity_) {
                    // this means that the ask order get's partially completed.
                    // whereas the bid order is completed!
                    pair_ref.ask_side.front()->quantity_ -= order->quantity_;
                    completeOrder(std::move(order));
                }
                else {
                    // this means the original order is partially completed
                    // the ask order is completed
                    order->quantity_ -= ask_order_quantity;
                    completeOrder(std::move(pair_ref.ask_side.front()));
                    pair_ref.ask_side.pop(); 
                }
            }
            // if some/all of the original order is still not fulfilled,
            // queue it
            if(order) {
                if(order->quantity_ > 0) {
                    pair_ref.bid_side.push(std::move(order));
                }
            }

        } else {
            // original order is an ask

            // try to match original order with bid side
            while(order->quantity_ && pair_ref.bid_side.size()) {
                uint64_t bid_order_quantity{pair_ref.bid_side.front()->quantity_};
                if(bid_order_quantity == order->quantity_) {
                    completeOrder(std::move(order));
                    completeOrder(std::move(pair_ref.bid_side.front()));
                    pair_ref.bid_side.pop();
                }
                else if(bid_order_quantity > order->quantity_) {
                    pair_ref.bid_side.front()->quantity_ -= order->quantity_;
                    completeOrder(std::move(order));
                }
                else {
                    order->quantity_ -= pair_ref.bid_side.front()->quantity_;
                    completeOrder(std::move(pair_ref.bid_side.front()));
                    pair_ref.bid_side.pop();
                }
            }
            if(order) {
                if(order->quantity_ > 0) {
                    // order has not been completed and there are no possible
                    // matches from the bid side at this moment
                    pair_ref.ask_side.push(std::move(order));
                }
            }
        }
    }
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
    static inline constexpr uint32_t kNumSecurities{8};

    // market order book pair
    struct MOBPair{
        MOBPair() = default; 
        std::queue<unique_ptr<BaseOrder>> bid_side;
        std::queue<unique_ptr<BaseOrder>> ask_side;
    };

    // limit order book pair
    struct LOBPair{
        OrderBook bid_side;
        OrderBook ask_side;
    }

    std::unordered_map<uint64_t, MOBPair> map_security_mo_{}; // maps security to market orders
    std::unordered_map<uint64_t, LOBPair> map_secuirty_lo_{};
};


using namespace std;
using namespace boost::posix_time;

void printTime(ptime &&t) {
    std::cout << "clock right now: " << t << "\n";
}

std::unique_ptr<BaseOrder> getUserInput(Exchange &e) {
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
        cout << "Enter quantity to " << (is_bid ? "buy" : "sell") << ": ";
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
        break;
    }

    return order;
}

std::unique_ptr<BaseOrder> getNextOrderFromFile(Exchange &e, std::stringstream &csv_line_ss) {
    std::unique_ptr<BaseOrder> order; // to be constructed and returned
    std::string next_item; // where the next item from the CSV line will be populated
    
    // Read the ticket sybmol
    std::string ticker;
    std::getline(csv_line_ss, ticker, ',');
    if(!e.securityExists(ticker)) {
        cout << "Error: invalid security. Try again.\n";
        cout << "\"" << ticker << "\" does not exist in the exchange.";
        return nullptr;
    }

    // Order is a bid or an ask
    bool is_bid{};
    std::getline(csv_line_ss, next_item, ',');
    if (next_item == "bid") {
        is_bid = true;
    } else if (next_item == "ask") {
        is_bid = false;
    } else {
        cout << "Error: \"" << next_item << "\" unclear whether order is bid or ask. Try again.\n";
        return nullptr;
    }
    
    // get quantity of shares to buy/sell
    std::getline(csv_line_ss, next_item, ',');
    int order_quantity{};
    order_quantity = std::stoi(next_item);
    if(order_quantity <= 0) {
        cout << "Quantity must not be negative or 0. Try again.\n" << endl;
        return nullptr;
    } 

    // get the type of order
    OrderType ot{};
    std::getline(csv_line_ss, next_item, ',');
    if(next_item == "market") {
        ot = OrderType::kMarket;
    }
    else if(next_item == "stop") {
        ot = OrderType::kStop;
    }
    else if(next_item == "limit") {
        ot = OrderType::kLimit;
    }
    else if(next_item == "stop limit") {
        ot = OrderType::kStopLimit;
    } else {
        cout << "Invalid order type";
        return nullptr;
    }


    double limit_price{};
    double stop_price{};

    // get the limit price, if applicable
    if(ot == OrderType::kLimit || ot == OrderType::kStopLimit) {
        std::getline(csv_line_ss, next_item, ',');

        try{
            limit_price = std::stod(next_item);
        } catch(...) {
            cout << "Invalid double entered for limit price\n";
            return nullptr;
        }
    }

    // get the stop price, if applicable
    if(ot == OrderType::kStop || ot == OrderType::kStopLimit) { 
        std::getline(csv_line_ss, next_item, ',');
        try{
            stop_price = std::stod(next_item);

        } catch(...) {
            cout << "Invalid double entered for stop price\n";
            return nullptr;
        }
    }

    // construct the order
    switch(ot) {
    case OrderType::kLimit:
        order.reset(new LimitOrder(1234, 
                                    e.getSecurityId(ticker), 
                                    order_quantity, 
                                    is_bid,
                                    limit_price));
    break;
    case OrderType::kMarket:
        order.reset(new MarketOrder(1234,
                                    e.getSecurityId(ticker), 
                                    order_quantity, 
                                    is_bid));
    break;
    case OrderType::kStop:
        order.reset(new StopOrder(1234,
                                    e.getSecurityId(ticker), 
                                    order_quantity,
                                    is_bid,
                                    stop_price,
                                    (stop_price > e.getCurrentSharePrice(e.getSecurityId(ticker))) ? true : false));
    break;
    case OrderType::kStopLimit:
        order.reset(new StopLimitOrder(1234,
                                    e.getSecurityId(ticker),
                                    order_quantity,
                                    is_bid,
                                    limit_price,
                                    stop_price,
                                    (stop_price > e.getCurrentSharePrice(e.getSecurityId(ticker))) ? true : false));
    break;
    default:
    break;
    }
    return order;
}
int main() {
    Exchange e{};

    // setup reading from file
    std::ifstream orders_file("orders.csv");
    if(!orders_file.is_open()) {
        std::cout << "Could not open file." << std::endl;
        return 1;
    }

    std::unique_ptr<BaseOrder> order;
    std::string csv_line;
    while(std::getline(orders_file, csv_line)) {
        std::stringstream csv_line_ss{csv_line};
        order = getNextOrderFromFile(e, csv_line_ss);
        if(!order) {
            cout << "Error. Exiting.\n";
            return 1;
        }
        order->print();
        e.addOrder(std::move(order));
    }

    // What is a limit order?
    // guaranteed to buy/sell at a specific price or better!
    // How to match bid limit orders? (ask would be inverse)
    // 1. Against ask market order if market price is lower than limit
    // 2. Against other ask limit order if ask limit price < bid limit price
    // Should we match a limit order against a market order or another limit order?
    // * Depends on which would give the order a better match.   
}

    // Notes on storing orders:
    // Each security needs its own store of orders
    // for bid and ask sides
    
    // 1. How to store market orders?
    // No price.
    // Thus, store in order of arrival.
    // Vector of unique pointers?

    // 2. How to store limit orders?
    // Have limit price.
    // Must store relative to price time priority
    // Order is not sequential like market orders
    // Use red-black trees for O(log(n)) insertion and retreival.

    // 3. Matching strategy:
    // Runs every time the security price is updated
    // 
    // 3a. Market order:
    //     Check if matching order on bid/ask side 
    //     Perform partial/full match
    //     Store remaining orders in queue
