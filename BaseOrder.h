
class BaseOrder {
public:
    enum class OrderType : uint8_t {
        kMarket = 0,
        kStop,
        kLimit,
        kStopLimit,
    };

    BaseOrder() = delete; // cannot default construct this object
    BaseOrder(uint64_t order_id, uint64_t security_id, OrderType order_type,
              bool is_bid) 
              : order_id_{order_id}, security_id_{security_id}, 
              order_type_{order_type}, is_bid_{is_bid} {}
    ~BaseOrder() = default; // default destructor

private:
    const uint64_t order_id_{};    // a unique identifier is given to each order
    const uint64_t security_id_{}; // identifies security that this order targets
    OrderType order_type_{}; 
    const bool is_bid_{};          
};

