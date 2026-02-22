#pragma once
#include <string>
#include <sstream>

namespace ds {
  class Item {
    public:
      int id;
      int quantity;
      double weight;

    public: 
      std::string to_string() const {
        std::ostringstream oss;
        oss << "{id=" << id
            << ", qty=" << quantity
            << ", w=" << weight
            << "}";
        return oss.str();
      }
  };
}
