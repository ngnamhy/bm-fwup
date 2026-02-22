//
// Created by ngnamhy on 19/10/2025.
//

#ifndef MAXIMAL_VARIABLE_H
#define MAXIMAL_VARIABLE_H
#include <map>
#include <vector>

#include "ds/itemset.h"
#include "ds/nlist.h"

namespace utils {
    class Variable {
    public:
        static std::map<ds::Itemset, std::shared_ptr<ds::NList>> itemset_nlist;
        static std::unordered_map<int, double> wus;
        static std::unordered_map<int, double> twu;
        static std::unordered_map<int, double> weight;
        static double sumtwu;
        static std::vector<int> f1;
        static std::vector<int> ord_f1;
    };
} // utils

#endif //MAXIMAL_VARIABLE_H