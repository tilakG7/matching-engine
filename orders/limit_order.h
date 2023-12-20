#pragma once

#include <iostream>

class LimitOrder : public BaseOrder {
public:
    LimitOrder() = delete;
    LimitOrder(uint64_t order_id, uint64_t security_id, OrderType order_type,
              bool is_bid, double limit_price) : 
              limit_price_{limit_price}, 
              BaseOrder(order_id, security_id, order_type, is_bid) {}
    double limit_price_{};
}; // class Limit Order