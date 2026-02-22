//
// Created by ngnamhy on 18/10/2025.
//

#include "database.h"

namespace ds {
    std::string Database::to_string() {
        std::ostringstream oss;
        oss << "Database {\n";

        for (const auto& tx : transactions) {
            oss << "  " << tx.to_string() << "\n";
        }

        oss << "}";
        return oss.str();
    }
} // ds