//
// Created by ngnamhy on 22/10/2025.
//

#ifndef MAXIMAL_NLIST_H
#define MAXIMAL_NLIST_H
#include <vector>

#include "umncode.h"

namespace ds {
    class NList : public std::vector<std::shared_ptr<UMNCode>> {
    public:
        double utility;
        void add(std::shared_ptr<UMNCode> code) {
            utility += code->utility;
            this->push_back(code);
        }
        std::string to_string();
        static NList merge(const NList& a, const NList& b);
    };
} // ds

#endif //MAXIMAL_NLIST_H