#include "BaseOrder.h"

class LimitOrder : public BaseOrder{
public:
    LimitOrder() = delete;
    LimitOrder(uint64_t order_id, uint64_t security_id, OrderType order_type,
              bool is_bid, double limit_price) 
              : BaseOrder(order_id, security_id, order_type, is_bid), 
              limit_price_{limit_price} {}
    ~LimitOrder() = default;
private: 
    double limit_price_{};
};

class StopLimitOrder : public LimitOrder 
{
public: 
    StopLimitOrder() = delete;
    StopLimitOrder(uint64_t order_id, uint64_t security_id, OrderType order_type,
                    bool is_bid, double limit_price, double stop_price)
                : LimitOrder(order_id, security_id, order_type, is_bid, limit_price),
                stop_price_{stop_price} {}
private:
    double stop_price_{};
};



