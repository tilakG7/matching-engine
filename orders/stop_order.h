#pragma once

#include "market_order.h"
#include <iostream>

class StopOrder : public MarketOrder {
public:
    StopOrder() = delete;
    StopOrder(uint64_t order_id, uint64_t security_id, uint64_t quantity,
              bool is_bid, double stop_price, bool exceed) : 
              stop_price_{stop_price}, exceed_{exceed}, 
              MarketOrder(order_id, security_id, quantity, is_bid) {
        order_type_ = OrderType::kStop;
    }

    virtual void print() override final {
        std::cout << "Stop price: " << stop_price_ << "\n";
        std::cout << "Must exceed SP: " << (exceed_ ? "true" : "false") << "\n";
        MarketOrder::print();
    }
    double stop_price_{};
    bool exceed_{};
}; // class StopOrder