#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <map>
#include <array>
#include "Exception.h"
#include "BookAnalyzerProcessor.h"
#include "Parser.h"

using namespace bookanalyzer;

void BookAnalyzerProcessor::processOrder(const std::string &input) {
  Order order;

  try {
    order = Parser::parse(input);
  } catch (const ParseException&) {
    std::cerr << "Malformed input data: " << input << std::endl;
    return;
  }

  // Update the book if add or remove
  try {
    updateBooks(order);
  } catch (const updateBooksException&) {
    std::cerr << "Error on adding new order to the book";
  }

  auto totalPrice = executeOrder(order);

  // Printing the output
  printOutput(order, totalPrice);
}

void BookAnalyzerProcessor::updateBooks(Order &order) {
  if (order.type == add)
    addToBooks(order);
  else if (order.type == reduce)
    reduceFromBooks(order);
  else
    throw updateBooksException();
  
  std::cout<< "order timestamp: " << order.timestamp << std::endl;
  std::cout<< "order type: " << order.type << std::endl;
  std::cout<< "order size: " << order.size << std::endl;
  std::cout<< "order side: " << order.side << std::endl;
}

void BookAnalyzerProcessor::addToBooks(Order &order) {
  orderLocator book_order;
  book_order.side = order.side;
  auto locator_it = orders_locator_.insert_or_assign(order.id, book_order).first;
  std::multimap<Order::PriceValue, Order::LimitSize>::iterator order_it;
  if (order.side == bookanalyzer::buy)
    order_it = bids_.insert(std::pair<Order::PriceValue, Order::LimitSize>(order.price, order.size));
  else
    order_it = asks_.insert(std::pair<Order::PriceValue, Order::LimitSize>(order.price, order.size));
  locator_it->second.order_it = order_it;
}

void BookAnalyzerProcessor::reduceFromBooks(Order &order) {
  auto locator_it = orders_locator_.find(order.id);
  if (locator_it != orders_locator_.end()) {
    auto book_order = &locator_it->second;
    order.side = book_order->side;
    auto book_it = book_order->order_it;
    auto item_size_ptr = &book_it->second;
    auto new_size = *item_size_ptr - order.size;

    if (new_size <= 0) {  // remove from orderbook
      orders_locator_.erase(locator_it);
      if (order.side == bookanalyzer::buy)
        bids_.erase(book_it);
      else
        asks_.erase(book_it);
    } else {  // update the orderbook
      *item_size_ptr = new_size;
    }
  }
}

Order::PriceValue BookAnalyzerProcessor::executeOrder(Order &order) {
  Order::LimitSize total_size = 0;
  Order::PriceValue possible_income_expense = 0;
  Order::PriceValue total = 0;
  switch(order.side) {
    case bookanalyzer::buy:
      for (auto buy_item : bids_) {
        Order::PriceValue price = buy_item.first;
        Order::LimitSize size = buy_item.second;
        if (targetLimitSize <= total_size + size) {
          possible_income_expense += price * (targetLimitSize - total_size);
          total = possible_income_expense;
          break;
        } else {
          total_size += size;
          possible_income_expense += price * size;
        }
      }
      break;
    case bookanalyzer::sell:
      for (auto buy_item : asks_) {
        Order::PriceValue price = buy_item.first;
        Order::LimitSize size = buy_item.second;
        if (targetLimitSize <= total_size + size) {
          possible_income_expense += price * (targetLimitSize - total_size);
          total = possible_income_expense;
          break;
        } else {
          total_size += size;
          possible_income_expense += price * size;
        }
      }
      break;
    default:
      throw executeOrderException();
  }
  return total;
}

void BookAnalyzerProcessor::printOutput(Order &order, Order::PriceValue &totalPrice) {
  double total_in_dollar = static_cast<double>(totalPrice) / 100;
  std::cout<< "printOutput" << std::endl;
  if (order.side == bookanalyzer::buy && buy_previous_trade_total != totalPrice) {
    buy_previous_trade_total = totalPrice;
    std::cout << order.timestamp << " " << "S" << " ";
    if (totalPrice != 0)
      std::cout << std::fixed << std::setprecision(2) << total_in_dollar << std::endl;
    else
      std::cout << "NA" << std::endl;
  } else if (order.side == bookanalyzer::sell && sell_previous_trade_total != totalPrice) {
    sell_previous_trade_total = totalPrice;
    std::cout << order.timestamp << " " << "B" << " ";
    if (totalPrice != 0)
      std::cout << std::fixed << std::setprecision(2) << total_in_dollar << std::endl;
    else
      std::cout << "NA" << std::endl;
  }
}

void BookAnalyzerProcessor::setTargetSize(Order::LimitSize targetSize) {
  targetLimitSize = targetSize;
}