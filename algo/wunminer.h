//
// Created by ngnamhy on 2/9/26.
//

#ifndef MAXIMAL_WUNMINER_H
#define MAXIMAL_WUNMINER_H
#include <map>
#include <boost/dynamic_bitset/dynamic_bitset.hpp>

#include "algo.h"
#include "ds/itemset.h"


namespace algo {
    class WUNMiner : public algo::Algo {
    public:
        void work(ds::Database db, double minwus) override;

        std::string name() const override;
        
        size_t estimate_memory_usage() const override;

    private:
        void search(std::vector<ds::Itemset> k);

        std::vector<ds::Itemset> all_combination(ds::Itemset &itemset, ds::Itemset &current);

        void backtrack(std::vector<ds::Itemset> &list, std::vector<int> &itemset, ds::Itemset current, int index);

        bool has_superset(ds::Itemset k);


    };;
} // algo


#endif //MAXIMAL_WUNMINER_H