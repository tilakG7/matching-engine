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
template<typename T>
struct OrderCmp {
    bool operator()(const unique_ptr<T>& a, const unique_ptr<t>& b) const {
        double a_price{a->getInterestingPrice()};
        double b_price{b->getInterestingPrice()};
        if(a->is_bid_) {
            if (fp_equals(a_price, b_price)) {
                return (a->timestamp_ < b->timestamp_);
            }
            // higher bids get to come first
            return (a_price > b_price);
        } 
        // if we reach this point, we know 
        // we are dealing with orders "ask" orders
        if (fp_equals(a_price, b_price)) {
            return (a->timestamp_ < b->timestamp_);
        }
        // lower asks get to come first
        return (a_price < b_price);
    }
};

template<typename T>
class OrderBook {
public:
    OrderBook() = default;

    void addOrder(unique_ptr<T> order_ptr) {
        orders_.emplace(std::move(order_ptr));
    }

    std::size_t getSize() {
        return orders_.size();
    }

    std::unique_ptr<T>& peek() {
        return *orders_.begin();
    }



    // prints all the orders in an order book
    void print() {
        uint64_t counter{};
        for(const auto &ptr : orders_) {
            counter++;
            std::cout << "Printing order #" << counter << std::endl;
            ptr->print();
            std::cout << "----------------";
        }
    }

private:
    std::set<unique_ptr<T>, OrderCmp> orders_; // stores all orders
};
