#ifndef BOOKANALYZERPROCCESSOR_H_
#define BOOKANALYZERPROCCESSOR_H_

#include <string>
#include <unordered_map>
#include <map>
#include <functional>
#include "Order.h"

namespace bookanalyzer {

const std::string buyCode = "B";
const std::string sellCode = "S";
const std::string addOrder = "A";
const std::string reduceOrder = "R";

class BookAnalyzerProcessor {

 public:
    BookAnalyzerProcessor(): orders_locator_(), bids_(), asks_() {}
    void processOrder(const std::string &input);
    void setTargetSize(Order::LimitSize targetSize);


 private:
    void updateBooks(Order &order);
    void addToBooks(Order &order);
    void reduceFromBooks(Order &order);
    Order::PriceValue executeOrder(Order &order);
    void printOutput(Order &order, Order::PriceValue &totalPrice);

    struct orderLocator {
        bookanalyzer::OrderSide side;
        std::multimap<Order::PriceValue, Order::LimitSize>::iterator order_it;
    };

    // An ordered map for bids in descending order,
    std::multimap<Order::PriceValue, Order::LimitSize, std::greater<>> bids_;

    // An ordered map for asks in ascending order
    std::multimap<Order::PriceValue, Order::LimitSize> asks_;

    // A hash map for locating order in the bids or asks map by it's order-id
    std::unordered_map<std::string, orderLocator> orders_locator_;

    Order::LimitSize targetLimitSize = 0;
    Order::PriceValue buy_previous_trade_total = 0;
    Order::PriceValue sell_previous_trade_total = 0;

};

}

#endif  // BOOKANALYZERPROCCESSOR_H_
