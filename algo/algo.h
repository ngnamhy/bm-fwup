//
// Created by ngnamhy on 18/10/2025.
//

#ifndef MAXIMAL_ALGO_H
#define MAXIMAL_ALGO_H


#include "ds/database.h"
#include "ds/itemset.h"
#include <map>
#include <memory>

#include <set>

#include "ds/nlist.h"


namespace algo {
    class Algo {
    public:
        virtual void work(ds::Database db, double minsup_percent) = 0;
        virtual std::string name() const = 0;
        virtual ~Algo() = default;
        virtual size_t estimate_memory_usage() const { return 0; }
        std::set<ds::Itemset> fwups;

    };
}

#endif //MAXIMAL_ALGO_H
