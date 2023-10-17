#include <cmath>
#include <set>

#include "order.h"

// Returns true if 2 floating point types are equals
// TODO: increase precision?
bool fp_equals(double a, double b) {
    return (std::abs(a - b)) < 0.01; // true if differce is less than a cent
}

// Returns true if order a should be placed before order b
// This is a binary predicate function for determining ordering of elements in 
// a container
// @returns - true if a should be placed before b, else false
bool compareOrder(const Order& a, const Order& b) {
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

// Only orders that are active make it into the orderbook
class OrderBook {
public:
    void addOrder(Order my_order) {
        set.emplace(my_order);
    }
private:
    std::set<Order, compareOrder> orders_; // stores all orders
};
