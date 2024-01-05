#pragma once

#include <cmath>
#include <set>

#include "orders/base_order.h"

// Returns true if 2 floating point types are equal
// @todo: increase precision of comparison?
bool fp_equals(double a, double b) {
    return (std::abs(a - b)) < 0.01; // true if differce is less than a cent
}

// Returns true if order A should be placed before order B
// This is a binary predicate function for determining ordering of elements in 
// a container
// @returns - true if a should be placed before b, else false
struct OrderCmp {
    bool operator()(const unqiue_ptr<BaseOrder>& a, const unique_ptr<BaseOrder>& b) const {
        if(a.is_bid) {
            if (fp_equals(a.limit_price, b.limit_price)) {
                return (a.time_received < b.time_received);
            }
            // higher bids get to come first
            return (a.limit_price > b.limit_price);
        } 
        // if we reach this point, we know 
        // we are dealing with orders "ask" orders
        if (fp_equals(a.limit_price, b.limit_price)) {
            return (a.time_received < b.time_received);
        }
        // lower asks get to come first
        return (a.limit_price < b.limit_price);
    }
};


// Only orders that are active make it into the orderbook
class OrderBook {
public:
    OrderBook() = default;

    void addOrder(unique_ptr<BaseOrder> order_ptr) {
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
            std::cout << "price: " << order.limit_price << std::endl;
            std::cout << "time: " << order.time_received << std::endl;
            std::cout << "----------------------------------" << std::endl;
        }
    }
private:
    std::set<unique_ptr<BaseOrder>, OrderCmp> orders_; // stores all orders
};
