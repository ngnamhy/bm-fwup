//
// Created by ngnamhy on 22/10/2025.
//

#include "nlist.h"

namespace ds {
    std::string NList::to_string() {
        std::string result = "[";
        for (auto k : *this) {
            result += k->to_string();
        }
        return result + "]";
    }

    NList NList::merge(const NList &a, const NList &b) {
        NList res;
        int i = 0, j = 0;
        while (i < a.size() && j < b.size()) {
            if (a[i]->pre < b[j]->pre) {
                if (a[i]->post > b[j]->post) {
                    if (!res.empty() && res.back()->pre == a[i]->pre) {
                        res.back()->utility += b[j]->utility;
                        res.utility += b[j]->utility;
                    }
                    else {
                        res.add(std::make_shared<UMNCode>(a[i]->pre, a[i]->post, b[j]->utility));
                    }
                    j++;
                }
                else {
                    i++;
                }
            }
            else {
                j++;
            }
        }
        return res;
    }
} // ds