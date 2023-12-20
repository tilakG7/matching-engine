#pragma once

#include <iostream>

class StopLimitOrder : public LimitOrder {
public:
    StopLimitOrder() = delete;
    StopLimitOrder(uint64_t order_id, uint64_t security_id, OrderType order_type,
              bool is_bid, double limit_price, double stop_price, bool exceed) : 
              stop_price_{stop_price}, exceed_{exceed}, 
              Limit(order_id, security_id, order_type, is_bid, limit_order) {}
    double stop_price{};
    bool exceed{};
}; // class StopLimitOrder