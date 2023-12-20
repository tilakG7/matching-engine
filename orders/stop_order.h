#pragma once

#include <iostream>

class StopOrder : public MarketOrder {
public:
    StopOrder() = delete;
    StopOrder(uint64_t order_id, uint64_t security_id, OrderType order_type,
              bool is_bid, double stop_price, bool exceed) : 
              stop_price_{stop_price}, exceed_{exceed}, 
              MarketOrder(order_id, security_id, order_type, is_bid) {}
    double stop_price{};
    bool exceed{};
}; // class StopOrder