//
// Created by ngnamhy on 19/10/2025.
//

#ifndef MAXIMAL_PPCTREE_H
#define MAXIMAL_PPCTREE_H
#include <memory>

#include "database.h"
#include "umnnode.h"


namespace ds {
    class UMNTree {
    public:
        std::shared_ptr<UMNNode> root;
        UMNTree(Database db, double minwus);
    private:
        void insert(std::vector<ds::Item> & transaction, double twu);
        void scan(std::shared_ptr<UMNNode> p, int &curpre, int &curpost);
    };
}


#endif //MAXIMAL_PPCTREE_H