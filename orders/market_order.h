#pragma once

#include <iostream>

class MarketOrder : public BaseOrder {
public:
    MarketOrder() = delete;
    MarketOrder(uint64_t order_id, uint64_t security_id, uint64_t quantity,
                bool is_bid) : 
              BaseOrder(order_id, security_id, quantity, OrderType::kMarket, 
                        is_bid) {}
}; // class MarketOrder