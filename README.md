# matching-engine

What is a limit order?
guaranteed to buy/sell at a specific price or better!
How to match bid limit orders? (ask would be inverse)
1. Against ask market order if market price is lower than limit
2. Against other ask limit order if ask limit price < bid limit price
Should we match a limit order against a market order or another limit order?
* Depends on which would give the order a better match.  


Notes on storing orders:
Each security needs its own store of orders
for bid and ask sides

1. How to store market orders?
No price.
Thus, store in order of arrival.
Vector of unique pointers?

2. How to store limit orders?
Have limit price.
Must store relative to price time priority
Order is not sequential like market orders
Use red-black trees for O(log(n)) insertion and retreival.

3. Matching strategy:
Runs every time the security price is updated

3a. Market order:
    Check if matching order on bid/ask side 
    Perform partial/full match
    Store remaining orders in queue