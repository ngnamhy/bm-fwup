//
// Created by ngnamhy on 18/10/2025.
//

#ifndef MAXIMAL_TRANSACTION_H
#define MAXIMAL_TRANSACTION_H
#include <string>
#include <vector>

#include "item.h"

namespace ds {
    class Transaction {
    public:
        int id;
        std::vector<ds::Item> items;

        std::string to_string() const;

        bool operator==(std::vector<Transaction>::const_reference value) const = default;

        Transaction() {}

        Transaction(int id, const std::vector<Item>& items)
            : items(items),
              id(id) {
        }
    };
}


#endif //MAXIMAL_TRANSACTION_H
