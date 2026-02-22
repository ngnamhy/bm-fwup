//
// Created by ngnamhy on 22/10/2025.
//

#ifndef MAXIMAL_PPCCODE_H
#define MAXIMAL_PPCCODE_H
#include "umnnode.h"

namespace ds {
    class UMNCode {
    public:
        int pre;
        int post;
        double utility;

        UMNCode(int pre, int post, double utility)
            : pre(pre),
              post(post),
              utility(utility) {
        }

        UMNCode(std::shared_ptr<UMNNode> node) {
            this->pre = node->pre;
            this->post = node->post;
            this->utility = node->utility;
        }

        std::string to_string();
    };
} // ds

#endif //MAXIMAL_PPCCODE_H