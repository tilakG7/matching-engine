#pragma once

#include "limit_order.h"

#include <iostream>

class StopLimitOrder : public LimitOrder {
public:
    StopLimitOrder() = delete;
    StopLimitOrder(uint64_t order_id, uint64_t security_id, bool is_bid, 
                   double limit_price, double stop_price, bool exceed) : 
              stop_price_{stop_price}, exceed_{exceed}, 
              LimitOrder(order_id, security_id, is_bid, limit_price) {
        order_type_ = OrderType::kStopLimit;
    }
    double stop_price_{};
    bool exceed_{};

    virtual void print() override final {
        std::cout << "Stop price: " << stop_price_ << "\n";
        std::cout << "Must exceed SP: " << (exceed_ ? "true" : "false") << "\n";
        LimitOrder::print();
    }
}; // class StopLimitOrder

