#include <cmath>
#include <set>

#include "order.h"

// Returns true if 2 floating point types are equal
// TODO: increase precision of comparison?
bool fp_equals(double a, double b) {
    return (std::abs(a - b)) < 0.01; // true if differce is less than a cent
}

// Returns true if order a should be placed before order b
// This is a binary predicate function for determining ordering of elements in 
// a container
// @returns - true if a should be placed before b, else false
struct OrderComp {
    bool operator()(const Order& a, const Order& b) const {
        if(a.is_bid) {
            if (fp_equals(a.target_price, b.target_price)) {
                return (a.time_received < b.time_received);
            }
            // higher bids get to come first
            return (a.target_price > b.target_price);
        } 
        // only orders must be ask
        if (fp_equals(a.target_price, b.target_price)) {
            return (a.time_received < b.time_received);
        }
        // lower asks get to come first
        return (a.target_price < b.target_price);
    }
};


// Only orders that are active make it into the orderbook
class OrderBook {
public:
    OrderBook() = default;

    void addOrder(Order my_order) {
        orders_.emplace(my_order);
    }

    void print() {
        std::cout << "Sequentially printing orders: \n";
        for (auto& order : orders_) {
            std::cout << "type: " << kOrderTypeStrings[
                static_cast<std::underlying_type_t<OrderType>>(order.type)] << std::endl;
            std::cout << "id: " << order.symbol_id << std::endl;
            std::cout << (order.is_bid ? "bid" : "ask") << std::endl;
            std::cout << "quantity: " << order.num << std::endl;
            std::cout << "price: " << order.target_price << std::endl;
            std::cout << "time: " << order.time_received << std::endl;
            std::cout << "----------------------------------" << std::endl;
        }
    }
private:
    std::set<Order, OrderComp> orders_; // stores all orders
};
