//
// Created by ngnamhy on 25/10/2025.
//

#include "bmfwup.h"


#include <cmath>
#include <math.h>
#include <ranges>

#include "ds/umntree.h"
#include "utils/input.h"
#include "utils/logger.h"
#include "utils/stat.h"
#include "utils/variable.h"

void algo::BMFWUP::work(ds::Database db, double minwus_percent) {
    utils::Config::minwus = utils::Variable::sumtwu * minwus_percent / 100.0;
    auto &minwus = utils::Config::minwus;
    LOG(debug) << "MINWUS PERCENT: " << minwus_percent << '\n';

    LOG(debug) << "SUMTWU: " << utils::Variable::sumtwu << '\n';
    LOG(debug) << "MINWUS: " << minwus << '\n';
    ds::UMNTree(db, minwus);
    std::vector<ds::Itemset> f1;
    std::for_each(utils::Variable::f1.begin(), utils::Variable::f1.end(),
                  [&](auto item) {
                      auto itemset = std::make_shared<ds::Itemset>(item);
                      f1.push_back(*itemset);
                      // LOG_CLASS("BMFWUP::work", debug) << utils::Variable::itemset_nlist[*itemset]->to_string();
                  }
    );
    search(f1);
}

std::string algo::BMFWUP::name() const {
    return "BMFWUP";
}

size_t algo::BMFWUP::estimate_memory_usage() const {
    size_t total = 0;
    
    // Size of fwups set (rough estimate: pointer per itemset + itemset data)
    for (const auto& itemset : fwups) {
        // Each itemset contains a vector of ints
        total += sizeof(ds::Itemset) + itemset.size() * sizeof(int) + 32; // overhead
    }
    
    // Size of bitmax
    for (const auto& [item, bitset] : bitmax) {
        // Each bitset uses approximately (num_blocks * sizeof(block_type)) bytes
        // dynamic_bitset block is typically size_t (8 bytes on 64-bit)
        size_t num_blocks = (bitset.size() + 63) / 64; // 64 bits per block
        total += sizeof(int) + num_blocks * 8 + 32; // key + bitset + map overhead
    }
    
    return total / 1024; // Convert to KB
}

void algo::BMFWUP::search(std::vector<ds::Itemset> k) {
    auto &mp = utils::Variable::itemset_nlist;
    for (int i = k.size() - 1; i >= 0; i--) {
        std::vector<ds::Itemset> next_k;

        auto &itemset_i = k[i];
        auto &nlist_i = mp[itemset_i];

        for (int j = i - 1; j >= 0; j--) {
            auto &itemset_j = k[j];
            auto &nlist_j = mp[itemset_j];

            auto itemset_ij = make_shared<ds::Itemset>(itemset_i);
            itemset_ij->add_item(itemset_j.last);

            auto nlist_ij = ds::NList::merge(*nlist_j, *nlist_i);

            if (nlist_ij.utility >= utils::Config::minwus) {
                mp[*itemset_ij] = make_shared<ds::NList>(nlist_ij);
                next_k.push_back(*itemset_ij);
            }
        }
        if (!next_k.empty()) {
            std::reverse(next_k.begin(), next_k.end());
            search(next_k);
        } else {
            if (!has_superset(itemset_i)) {
                fwups.insert(itemset_i);
                for (auto i: itemset_i) {
                    bitmax[i].resize(fwups.size());
                    bitmax[i][fwups.size() - 1] = true;
                }
            }
        }
    }
}

std::vector<ds::Itemset> algo::BMFWUP::all_combination(ds::Itemset &itemset, ds::Itemset &base) {
    std::vector<ds::Itemset> combination;
    std::vector<int> items;
    std::for_each(itemset.begin(), itemset.end(), [&](auto item) { items.push_back(item); });
    backtrack(combination, items, ds::Itemset(), 0);
    for (auto &i: combination) {
        for (int j: base)
            i.add_item(j);
    }
    return combination;
}

void algo::BMFWUP::backtrack(std::vector<ds::Itemset> &list, std::vector<int> &itemset, ds::Itemset current, int index) {
    if (index >= itemset.size()) {
        if (!current.empty()) {
            list.push_back(current);
        }
        return;
    }
    backtrack(list, itemset, current, index + 1);
    current.add_item(itemset[index]);
    backtrack(list, itemset, current, index + 1);
}

bool algo::BMFWUP::has_superset(ds::Itemset k) {
    utils::Stat::n_checkcount += 1;

    int curlen = fwups.size();
    boost::dynamic_bitset<> _and;
    for (auto i: k) {
        if (i == *k.begin()) {
            _and = bitmax[i];
            _and.resize(curlen);
        } else {
            auto &bs = bitmax[i];
            bs.resize(curlen);
            _and = _and & bs;
        }
        if (_and.none()) {
            return false;
        }
    }
    return true;
}

