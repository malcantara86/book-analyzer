#ifndef ORDER_H
#define ORDER_H

#include <string>
namespace bookanalyzer {

enum OrderSide { buy, sell };
enum OrderType { add, reduce };

// Market Book Order
struct Order {

  typedef std::string TransactionId;                    // Unique Order Id
  typedef unsigned int Timestamp;            // Milliseconds from midnight
  typedef unsigned long int PriceValue;      // Price in cents
  typedef unsigned long int LimitSize;       // Size

  Timestamp timestamp;
  OrderType type;
  TransactionId id;
  OrderSide side;
  PriceValue price;
  LimitSize size;
};

} // bookanalyzer namespace

#endif //ORDER_H