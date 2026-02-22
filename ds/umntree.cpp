//
// Created by ngnamhy on 19/10/2025.
//

#include "umntree.h"

#include <algorithm>
#include <numeric>

#include "utils/logger.h"
#include "utils/stat.h"
#include "utils/variable.h"


namespace ds {
    UMNTree::UMNTree(Database db, double minwus) {
        auto &wus = utils::Variable::wus;
        auto &twu = utils::Variable::twu;
        // sup.resize(utils::Stat::n_item);


        // for (int i = 0; i <= utils::Stat::n_item; i++) {
        //     wus[i] /= utils::Variable::sumtwu;
        // }

        auto &f1 = utils::Variable::f1;
        for (int i = 0; i <= utils::Stat::n_item; i++) {
            if (wus[i] >= minwus) {
                f1.push_back(i);
            }
        }

        sort(f1.begin(), f1.end(), [&](int i, int j) {
            return wus[i] > wus[j];
        });

        auto &ord_f1 = utils::Variable::ord_f1;
        ord_f1.resize(utils::Stat::n_item, utils::Stat::n_item);
        for (int i = 0; i < f1.size(); i++) {
            ord_f1[f1[i]] = i;
        }

        root = std::make_shared<UMNNode>(-1);

        for (auto &t : db.transactions) {
            std::vector<ds::Item> freq;
            for (auto &i : t.items) {
                if (wus[i.id] >= minwus) {
                    freq.push_back(i);
                }
            }
            std::sort(freq.begin(), freq.end(), [&](ds::Item i, ds::Item j) {
                return ord_f1[i.id] < ord_f1[j.id];
            });
            insert(freq, utils::Variable::twu[t.id]);
        }
        int curpre = 0, curpost = 0;
        scan(root, curpre, curpost);
    }

    void UMNTree::insert(std::vector<ds::Item> &transaction, double twu) {
        auto t = root;
        for (auto &i : transaction) {
            if (t->children.count(i.id)) {
                auto nt = t->children[i.id];
                nt->utility += twu;
            }
            else {
                t->children[i.id] = std::make_shared<UMNNode>(i.id);
                t->children[i.id]->utility += twu;
            }
            t = t->children[i.id];
        }
    }

    void UMNTree::scan(std::shared_ptr<UMNNode> p, int &curpre, int &curpost) {
        p->pre = curpre;
        curpre++;
        for (auto &t : p->children) {
            scan(t.second, curpre, curpost);
        }
        p->post = curpost;
        curpost++;
        if (p->id != -1) {
            auto &mp = utils::Variable::itemset_nlist;
            auto itemset = Itemset(p->id);
            if (!mp.count(itemset)) {
                mp[itemset] = std::make_shared<NList>();
            }
            mp[itemset]->add(std::make_shared<UMNCode>(p));
        }
    }

}
