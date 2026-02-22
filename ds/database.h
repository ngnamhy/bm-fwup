//
// Created by ngnamhy on 18/10/2025.
//

#ifndef MAXIMAL_DATABASE_H
#define MAXIMAL_DATABASE_H
#include "transaction.h"
#include "utils/stat.h"
#include "utils/variable.h"
#include "utils/logger.h"

namespace ds {
    class Database {
    public:
        Database() = default;
        explicit Database(const std::vector<Transaction>& transactions)
            : transactions(transactions) {
            auto &wus = utils::Variable::wus;
            auto &twu = utils::Variable::twu;

            for (auto &t : this->transactions) {
                for (auto &i : t.items) {
                    twu[t.id] += utils::Variable::weight[i.id] * i.quantity;
                }
                // Avoid division by zero for empty transactions
                if (t.items.size() > 0) {
                    twu[t.id] /= static_cast<double>(t.items.size());
                    utils::Variable::sumtwu += twu[t.id];
                }
            }
            for (auto &t : this->transactions) {
                for (auto &i : t.items) {
                    wus[i.id] += twu[t.id];
                }
            }
            // for (int i = 1; i <= utils::Stat::n_item; i++) {
            //     LOG(info) << i << " " << wus[i] << '\n';
            // }
        }

        std::string to_string();

        std::vector<Transaction> transactions;

    };
} // ds

#endif //MAXIMAL_DATABASE_H