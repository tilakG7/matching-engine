#include <set>

#include "order_book.h"
#include "stop_order_book.h"
struct InstrumentInfo{
    double price;     // price of instrument
    std::string name; // name for instrument
};

class Exchange {
public:
    Exchange() = default;
    ~Exchange() = default;
    // stores all stock symbols/identifiers
    // cancel order
    // get stock price

    void tick() {

    }
    // places an order
    bool order(){
        return false;
    }
private:
    OrderBook bid_book_{}; // order book for bid side
    OrderBook ask_book_{}; // order book for ask side
    // maps id of financial instrument to struct containing more info
    std::unordered_map<uint32_t, InstrumentInfo>  instrument_map_{};
    std::unordered_map<uint32_t, std::pair<StopOrderBook<true>, StopOrderBook<false>> inactive_orders_{};
};


using namespace boost::posix_time;

void printTime(ptime &&t) {
    std::cout << "clock right now: " << t << "\n";
}

int main() {
    ptime t{microsec_clock::local_time()};
    // printTime(std::move(t));
    std::cout << "Time: " << t << std::endl;

    Order o1{OrderType::kMarket, 0, false, 2, 430.0, 0.0, false, t};
    Order o2{OrderType::kMarket, 0, false, 2, 430.1, 0.0, false, t};
    OrderBook ob{};
    ob.addOrder(o1);
    ob.addOrder(o2);
    ob.print();

    StopOrderBook<true> s;
    s.addOrder(o1);
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