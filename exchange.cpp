#include <set>

#include "order_book.h"

class Exchange {
public:
    Exchange() = default;
    ~Exchange() = default;
    // stores all stock symbols/identifiers
    // receive order
    // cancel order
    // get stock price
private:
    OrderBook bid_book_{}; // order book for bid side
    OrderBook ask_book_{}; // order book for buy side
};


using namespace boost::posix_time;

void printTime(ptime &&t) {
    std::cout << "clock right now: " << t << "\n";
}

int main() {
    ptime t{microsec_clock::local_time()};
    // printTime(std::move(t));
    std::cout << "Time: " << t << std::endl;

    Order o1{OrderType::kMarket, 0, false, 2, 430.0, t};
    Order o2{{OrderType::kMarket, 0, false, 2, 430.1, t};
    OrderBook ob{};
    ob.addOrder(o1);
    ob.addOrder(o2);


}