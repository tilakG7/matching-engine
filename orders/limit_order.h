#pragma once

#include <iostream>
#include "base_order.h"

class LimitOrder : public BaseOrder{
public:
    LimitOrder() = delete;
    LimitOrder(uint64_t order_id, uint64_t security_id, uint64_t quantity,
               bool is_bid, double limit_price) 
              : BaseOrder(order_id, security_id, quantity, OrderType::kLimit, 
                          is_bid), 
              limit_price_{limit_price} {}
    ~LimitOrder() = default;

    virtual void print() override {
        std::cout << "Limit Price: " << limit_price_ << "\n";
        BaseOrder::print();
    }

private: 
    double limit_price_{};
};