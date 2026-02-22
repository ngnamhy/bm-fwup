//
// Created by ngnamhy on 25/10/2025.
//

#include "wunminer.h"


#include <cmath>
#include <math.h>
#include <ranges>

#include "ds/umntree.h"
#include "utils/input.h"
#include "utils/logger.h"
#include "utils/stat.h"
#include "utils/variable.h"

void algo::WUNMiner::work(ds::Database db, double minwus_percent) {
    utils::Config::minwus = utils::Variable::sumtwu * minwus_percent / 100.0;
    auto &minwus = utils::Config::minwus;
    // LOG(debug) << "MINWUS PERCENT: " << minwus_percent << '\n';
    //
    // LOG(debug) << "SUMTWU: " << utils::Variable::sumtwu << '\n';
    // LOG(debug) << "MINWUS: " << minwus << '\n';
    ds::UMNTree(db, minwus);
    std::vector<ds::Itemset> f1;
    std::for_each(utils::Variable::f1.begin(), utils::Variable::f1.end(),
                  [&](auto item) {
                      auto itemset = std::make_shared<ds::Itemset>(item);
                      f1.push_back(*itemset);
                      // LOG_CLASS("WUNMiner::work", debug) << utils::Variable::itemset_nlist[*itemset]->to_string();
                  }
    );
    search(f1);
}

std::string algo::WUNMiner::name() const {
    return "WUNMiner";
}

size_t algo::WUNMiner::estimate_memory_usage() const {
    size_t total = 0;
    
    // Size of fwups set
    for (const auto& itemset : fwups) {
        // Each itemset contains a vector of ints + set node overhead
        total += sizeof(ds::Itemset) + itemset.size() * sizeof(int) + 32;
    }
    
    return total / 1024; // Convert to KB
}

void algo::WUNMiner::search(std::vector<ds::Itemset> k) {
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
            }
        }
    }
}

std::vector<ds::Itemset> algo::WUNMiner::all_combination(ds::Itemset &itemset, ds::Itemset &base) {
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

void algo::WUNMiner::backtrack(std::vector<ds::Itemset> &list, std::vector<int> &itemset, ds::Itemset current, int index) {
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

bool algo::WUNMiner::has_superset(ds::Itemset k) {
    utils::Stat::n_checkcount += 1;

    for (auto p : fwups) {
        if (std::includes(p.begin(), p.end(), k.begin(), k.end())) {
            return true;
        }
    }
    return false;
}

