//
// Created by ngnamhy on 19/10/2025.
//

#pragma once
#include <memory>
#include <unordered_map>
#include <vector>

namespace ds {
    class UMNNode {
    public:
        int id;
        double utility;
        std::unordered_map<int, std::shared_ptr<UMNNode>> children;
        int pre;
        int post;

        UMNNode(int id): id(id), utility(0), pre(0), post(0) {}
    };
} // ds
