#pragma once

#include <set>

#include "order.h"

// Returns true if order a should be placed before order b
// @template exceeds
//           whether order must exceed stop price to become active
template<bool exceeds>
struct StopOrderCmp{
    bool operator()(const Order&a, const Order&b) const {
        if constexpr(exceeds) {
            // store the lower activation price first
            // if stop price is the same, that's ok
            return a.stop_price < b.stop_price; 
        }
        // store the higher acivation price first
        return a.stop_price > b.stop_price;
    }
};

/**
* Stores stop or stop limit orders that are not yet active
* This class allows for fast lookup of orders that need to become active once
* price reaches and goes above/below a target stop price.
* @template exceeds
*           Determines whether for the order to become active, it must exceed 
*           the stop price or fall below it
*/
template<bool exceeds>
class StopOrderBook {
public:
    bool addOrder(Order my_order) {
        // check if user tried to order a wrong kind of order 
        if(my_order.must_exceed_stop != exceeds) {
            return false;
        }
        orders_.emplace(my_order);
        return true;
    }
private:
    std::set<Order, StopOrderCmp<exceeds>> orders_;
};