//
// Created by ngnamhy on 25/10/2025.
//

#ifndef MAXIMAL_BMFI_H
#define MAXIMAL_BMFI_H
#include <map>
#include <boost/dynamic_bitset/dynamic_bitset.hpp>

#include "algo.h"
#include "ds/itemset.h"

namespace algo {
    class BMFWUP : public algo::Algo {
    public:
        void work(ds::Database db, double minwus) override;

        std::string name() const override;
        
        size_t estimate_memory_usage() const override;

    private:
        void search(std::vector<ds::Itemset> k);

        std::vector<ds::Itemset> all_combination(ds::Itemset &itemset, ds::Itemset &current);

        void backtrack(std::vector<ds::Itemset> &list, std::vector<int> &itemset, ds::Itemset current, int index);

        bool has_superset(ds::Itemset k);


        std::unordered_map<int, boost::dynamic_bitset<>> bitmax;
    };;
} // algo

#endif //MAXIMAL_BMFI_H
