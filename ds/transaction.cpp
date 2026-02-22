//
// Created by ngnamhy on 18/10/2025.
//

#include "transaction.h"

namespace ds {
    std::string Transaction::to_string() const {
        std::ostringstream oss;
        oss << "T" << id << ": [";

        for (size_t i = 0; i < items.size(); ++i) {
            oss << items[i].to_string();
            if (i + 1 < items.size()) {
                oss << ", ";
            }
        }

        oss << "]";
        return oss.str();
    }
}
