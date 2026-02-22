//
// Created by ngnamhy on 22/10/2025.
//

#include "umncode.h"

namespace ds {
    std::string UMNCode::to_string() {
        std::string result = "{" + std::to_string(this->utility) +
                             " " + std::to_string(this->pre) +
                             " " + std::to_string(this->post) + "}";
        return result;
    }
} // ds