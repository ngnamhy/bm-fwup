//
// Created by ngnamhy on 25/10/2025.
//

#ifndef MAXIMAL_ITEMSET_H
#define MAXIMAL_ITEMSET_H
#include <set>
#include <string>

namespace ds {
    class Itemset : public std::set<int> {
    public:
        Itemset() {}            // constructor mặc định
        explicit Itemset(int item);         // constructor có 1 item
        void add_item(int item);

        std::string to_string();

        int last;
    };
} // ds

#endif //MAXIMAL_ITEMSET_H