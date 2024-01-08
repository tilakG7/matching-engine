#pragma once

#include <array>
#include <iostream>
#include <utility> // for std::to_underlying

#include "common/template_helper.h"
#include "boost/date_time/posix_time/posix_time.hpp"

using namespace std;
using namespace boost::posix_time;

// Types of orders supported by our exchange
enum class OrderType : uint8_t {
    kMarket = 0,
    kStop,
    kLimit,
    kStopLimit,
    kNumOrderTypes,
};

const array<string, 
            to_underlying(OrderType::kNumOrderTypes)> kOrderTypeToStr{
    "market",
    "stop",
    "limit",
    "stop limit"
};
class BaseOrder {
public:
    

    BaseOrder() = delete; // cannot default construct this object
    BaseOrder(uint64_t order_id, uint64_t security_id, uint64_t quantity, 
              OrderType order_type, bool is_bid) 
              : order_id_{order_id}, security_id_{security_id}, 
                quantity_{quantity}, order_type_{order_type}, is_bid_{is_bid}, 
              timestamp_{microsec_clock::local_time()} {}
    ~BaseOrder() = default; // default destructor

    // prints relevant order details
    virtual void print() const noexcept{
        cout << "Order #" << order_id_ << "\n";
        cout << "Security ID: " << security_id_ << "\n";
        cout << "Quantity: " << quantity_ << "\n";
        cout << "Type: " << kOrderTypeToStr[to_underlying(order_type_)] 
             << " order" << "\n";
        cout << (is_bid_ ? "Bid" : "Ask") << "\n";
        cout << "Time: " << timestamp_ << "\n";
        cout << "---------------------------\n\n";
    }

    virtual double getInterestingPrice() const noexcept {
        return 0;
    }

    const bool is_bid_{};
    ptime timestamp_{};            // time the order was received       
    OrderType order_type_{};       
    const uint64_t security_id_{}; // identifies security that this order targets
    uint64_t quantity_{};          // number of security to buy or sell
    const uint64_t order_id_{};    // a unique identifier is given to each order
};

