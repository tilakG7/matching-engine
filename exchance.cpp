#include "order_book.h"

class Exchange {
public:
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
    ptime t{};
    printTime(std::move(t));
    std::cout << "Time after move: " << t << std::endl;
}