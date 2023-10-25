#pragma once

#include "order_book.h"

class Instrument{
public:


private:
    uint32_t id{};       // unique identifier for the instrument
    double price_{};     // latest price of the instrument
    std::string name_{}; // name of the financial instrument
    OrderBook bid_ob_{}; // order book for the bid side
    OrderBook ask_ob_{}; // order book for the ask side
};