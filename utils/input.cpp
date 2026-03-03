//
// Created by ngnamhy on 18/10/2025.
//

#include "input.h"

#include <fstream>
#include <sstream>

#include "logger.h"
#include "stat.h"
#include "variable.h"
#include "../ds/transaction.h"

namespace utils {
    std::string Config::algorithm;
    std::string Config::dataset;
    std::string Config::input_path;
    double Config::minsup_percent;
    double Config::minwus;
    boost::log::trivial::severity_level Config::severity;

    ds::Database Input::read() {
        // ---------- read .pro ----------
        std::string pro_file = Config::input_path + "/" + Config::dataset + ".pro";
        std::ifstream pro_stream(pro_file);
        if (!pro_stream.is_open()) {
            throw std::runtime_error("Cannot open file: " + pro_file);
        }

        // index = item_id (0-based)
        std::vector<double> item_weights;
        double w;
        while (pro_stream >> w) {
            item_weights.push_back(w);
        }
        for (int i = 0; i < item_weights.size(); ++i) {
            utils::Variable::weight[i] = item_weights[i];
        }
        pro_stream.close();

        // ---------- read .hui ----------
        std::string hui_file = Config::input_path + "/" + Config::dataset + ".hui";
        std::ifstream hui_stream(hui_file);
        if (!hui_stream.is_open()) {
            throw std::runtime_error("Cannot open file: " + hui_file);
        }

        int num_transactions = 0;
        int num_items = 0;
        hui_stream >> num_transactions >> num_items;
        utils::Stat::n_item = num_items;

        // transaction_id -> items
        std::unordered_map<int, std::vector<ds::Item>> tx_items;

        int tx_id, item_id, quantity;
        while (hui_stream >> tx_id >> item_id >> quantity) {
            if (item_id < 0 || item_id >= (int)item_weights.size()) {
                throw std::runtime_error("Invalid item_id in hui file");
            }

            ds::Item item;
            item.id = item_id;
            item.quantity = quantity;
            item.weight = item_weights[item_id];

            tx_items[tx_id].push_back(item);
        }
        hui_stream.close();

        // ---------- build Database ----------
        std::vector<ds::Transaction> transactions;
        transactions.reserve(num_transactions);

        for (int i = 1; i <= num_transactions; ++i) {
            auto it = tx_items.find(i);
            if (it != tx_items.end()) {
                transactions.emplace_back(i, it->second);
            } else {
                transactions.emplace_back(i, std::vector<ds::Item>{});
            }
        }

        ds::Database db = ds::Database(transactions);
        // LOG(debug) << db.to_string();
        return db;
    }

} // utils