#include <set>

bool compareOrder(const Order& a, const Order& b) {
    // if bid orders
        // if bid(a) == bid(b)
            // return (time(a) < time(b) ? a : b);
        // return (bid(a) > bid(b) ? a : b);
    // else
        // if sell(a) == sell(b)
            // return (time(a) < time(b) ? a : b);
        // return (sell(a) < sell(b) ? a : b)
    // if a and b price match, look at time
    // else, look at price only
}

// Only orders that are active make it into the orderbook
class OrderBook {
public:

private:
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