
#ifndef ORDER_H
#define ORDER_H

#include <string>
namespace bookanalyzer {

enum OrderSide { buy, sell };
enum OrderType { add, reduce };

// Order model
struct Order {

  typedef std::string TransactionId;         // Unique Order Id
  typedef unsigned int Timestamp;            // Timestamp (milliseconds since midnight)
  typedef unsigned long int PriceValue;      // Limit Price of this order (cents)
  typedef unsigned long int Size;       // size in shares of this order

  Timestamp timestamp;
  OrderType orderType;
  TransactionId transId;
  OrderSide orderSide;
  PriceValue price;
  Size size;
};

} // bookanalyzer namespace

#endif //ORDER_H