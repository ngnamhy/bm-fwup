//
// Created by ngnamhy on 19/10/2025.
//

#include "variable.h"

namespace utils {
    std::unordered_map<int, double> Variable::wus;
    std::unordered_map<int, double> Variable::twu;
    std::unordered_map<int, double> Variable::weight;
    std::vector<int> Variable::f1;
    std::vector<int> Variable::ord_f1;
    std::map<ds::Itemset, std::shared_ptr<ds::NList>> Variable::itemset_nlist;
    double Variable::sumtwu;
} // utils