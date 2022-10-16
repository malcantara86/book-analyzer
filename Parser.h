#ifndef BOOKANALYZER_PARSER_H
#define BOOKANALYZER_PARSER_H

#include <string>
#include <sstream>
#include <array>
#include <iterator>
#include "Order.h"
#include "Exception.h"
#include "BookAnalyzerProcessor.h"

const std::string addOrder "A"
const std::string reduceOrder "R"

namespace bookanalyzer {

class BookParser {
 public:
  static bookanalyzer::Order parse(const std::string &input);
};

} // bookanalyzer namespace

#endif //BOOKANALYZER_PARSER_H

inline bookanalyzer::Order bookanalyzer::BookParser::parse(const std::string &input) {
  // Parse and split the input string into different tokens
  std::istringstream input_string_stream(input);
  std::array<std::string, 6> tokens;
  std::copy(std::istream_iterator<std::string>(input_string_stream),
            std::istream_iterator<std::string>(),
            tokens.begin());

  Order order;
  for(int i =0;i<6;i++)
    std::cout<<tokens[i]<<std::endl;

  // parse timestamp
  auto timestamp = tokens[0];
  order.timestamp = (Order::Timestamp) std::stoul(timestamp);

  // parse identifier
  auto order_identifier = tokens[1];
  if (order_identifier.compare(addOrder) == 0)
    order.orderType = add;
  else if (order_identifier.compare(reduceOrder) == 0)
    order.orderType = reduce;
  else
    throw bookanalyzer::ParseException();

  // parse order-id
  auto order_id = tokens[2];
  order.transId = order_id;

  if (order.orderType == add) { // Add Order

    // parse orderSide and map it to OrderSide enum
    auto sideLiteral = tokens[3];
    if (sideLiteral == buyCode)
      order.orderSide = buy;
    else if (sideLiteral == sellCode)
      order.orderSide = sell;
    else
      throw bookanalyzer::ParseException();

    // price (in cents)
    order.price = static_cast<Order::PriceValue>(100 * std::stof(tokens[4]));

    // size
    order.size = std::stoul(tokens[5]);

  } else if (order.orderType == reduce) { // Reduce Order
    // size
    order.size = std::stoul(tokens[3]);

  } else {
    throw bookanalyzer::ParseException();
  }
  return order;
}