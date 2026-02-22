//
// Created by ngnamhy on 25/10/2025.
//

#include "itemset.h"

namespace ds {
    Itemset::Itemset(int item) {
        this->insert(item);
        last = item;
    }

    void Itemset::add_item(int item) {
        last = item;
        this->insert(item);

    }

    std::string Itemset::to_string() {
        std::string result = "{";
        for (auto i : *this) {
            result += std::to_string(i);
            if (i != *(this->rbegin())) {
                result += ", ";
            }
        }
        return result + "}";
    }
} // ds
