#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <unistd.h> // sysconf()
#include <boost/dynamic_bitset/dynamic_bitset.hpp>

#include "algo/algo.h"
#include "algo/bmfwup.h"

#include "algo/bmfwup.h"
#include "algo/wunminer.h"

#include "utils/config.h"
#include "utils/input.h"
#include "utils/logger.h"
#include "utils/stat.h"
#include <ctime>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

static size_t parseKbLine(const std::string& line) {
  // line dạng: "VmHWM:\t  12345 kB"
  std::string num;
  for (char c : line) {
    if (c >= '0' && c <= '9') num.push_back(c);
    else if (!num.empty()) break;
  }
  return num.empty() ? 0 : static_cast<size_t>(std::stoull(num));
}

// Current resident set size (RSS) in MB
size_t getCurrentRssMB() {
  std::ifstream f("/proc/self/status");
  std::string line;
  while (std::getline(f, line)) {
    if (line.rfind("VmRSS:", 0) == 0) {
      size_t kb = parseKbLine(line);
      return kb / 1024;
    }
  }
  return 0;
}

// Peak resident set size (RSS) in MB since process start
size_t getPeakRssMB() {
  std::ifstream f("/proc/self/status");
  std::string line;
  while (std::getline(f, line)) {
    if (line.rfind("VmHWM:", 0) == 0) {
      size_t kb = parseKbLine(line);
      return kb / 1024;
    }
  }
  return 0;
}


std::unique_ptr<algo::Algo> createMiner(const std::string &algorithm) {
  if (algorithm == "wunminer")
    return std::make_unique<algo::WUNMiner>();
  if (algorithm == "bmfwup")
    return std::make_unique<algo::BMFWUP>();
  throw std::runtime_error("Unknown algorithm: " + algorithm);

}

int main() {
  utils::Config::load();
  logger::init_logging();


  utils::Input input;
  ds::Database db = input.read();

  auto start = std::chrono::high_resolution_clock::now();

  auto miner = createMiner(utils::Config::algorithm);

  miner->work(db, utils::Config::minsup_percent);

  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  size_t estimated_algo_mem = miner->estimate_memory_usage();

  auto& f = miner->fwups;

  std::map<ds::Itemset, bool> mp;

  // for (auto i : f) {
  //   LOG(info) << i.to_string() << '\n';
  // }
  LOG(info) << "Algorithm: " << miner->name() << std::endl;
  LOG(info) << "Dataset: " << utils::Config::dataset << std::endl;
  LOG(info) << "Minwus: " << utils::Config::minwus << std::endl;
  LOG(info) << "Check counts: " << utils::Stat::n_checkcount << std::endl;
  LOG(info) << "Number of FMI: " << miner->fwups.size() << std::endl;
  LOG(info) << "Runtime: " << duration.count() << " ms" << std::endl;
  LOG(info) << "Estimated algo structures: " << estimated_algo_mem << " KB" << std::endl;


  json output = {
    {"algo", miner->name()},
    {"dataset", utils::Config::dataset},
    { "minwus", utils::Config::minwus },
    {"check_count", utils::Stat::n_checkcount},
    {"fwups size", miner->fwups.size()},
    {"runtime", duration.count()},
    {"estimated_algo_mem", estimated_algo_mem},
  };
  std::cout << output.dump(4) << '\n';
  return 0;
}
