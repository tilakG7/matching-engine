#pragma once

#include <iostream>

class MarketOrder : public BaseOrder {
public:
    MarketOrder() = delete;
    MarketOrder(uint64_t order_id, uint64_t security_id, bool is_bid) : 
              BaseOrder(order_id, security_id, OrderType::kMarket, is_bid) {}
}; // class MarketOrder