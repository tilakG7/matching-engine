#include <cstdint>

enum class OrderType : uint8_t {
    kMarket = 0,
    kLimit,
    kStop,
    kStopLimit
};

struct Order {
    OrderType type{};
    uint32_t symbol_id{}; // id of stock this order is for
    bool is_bid{};        // side - bid or ask
    uint32_t num{};       // quantity of shares to purchase
    double targetPrice{}; // only for certain order types
    // some type for time - boost?
};