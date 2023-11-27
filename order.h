#pragma once
#include <cstdint>

#include "boost/date_time/posix_time/posix_time.hpp"

enum class OrderType : uint8_t {
    kMarket = 0,
    kLimit,
    kStop,
    kStopLimit,
    kNumOrders,
};

const char* kOrderTypeStrings[] = {
    "market",
    "limit",
    "stop",
    "stop limit",
};

using namespace boost::posix_time;
struct Order {
    OrderType type{};
    uint32_t symbol_id{};       // id of stock this order is for
    bool is_bid{};              // side - bid or ask
    uint32_t num{};             // quantity of shares to purchase
    double limit_price{};       // only for certain order types
    double stop_price{};        // only for stop orders
    bool must_exceed_stop{};    // only for stop orders
    ptime time_received{};      // time the order was received
};

