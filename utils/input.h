//
// Created by ngnamhy on 18/10/2025.
//

#ifndef MAXIMAL_INPUT_H
#define MAXIMAL_INPUT_H
#include <iosfwd>

#include "config.h"
#include "../ds/database.h"

namespace utils {
    class Input {
    public:
        ds::Database read();
    };

} // utils

#endif //MAXIMAL_INPUT_H