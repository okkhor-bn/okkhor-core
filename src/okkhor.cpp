#include "okkhor.hpp"

#include <cstdlib>
#include <fstream>
#include <stdexcept>

namespace okkhor {
namespace {

bool looks_like_data_dir(const std::string& dir) {
    std::ifstream probe(dir + "/vowels.json", std::ios::binary);
    return static_cast<bool>(probe);
}

}  // namespace

std::string find_data_dir(const std::string& hint) {
    std::vector<std::string> candidates;
    if (!hint.empty()) candidates.push_back(hint);
    if (const char* env = std::getenv("OKKHOR_DATA")) candidates.emplace_back(env);
#ifdef OKKHOR_DATA_DIR
    candidates.emplace_back(OKKHOR_DATA_DIR);
#endif
    candidates.emplace_back("data");
    candidates.emplace_back("../data");
    candidates.emplace_back("../../data");

    for (const std::string& c : candidates)
        if (looks_like_data_dir(c)) return c;

    throw std::runtime_error(
        "could not locate the Okkhor data directory; pass --data <dir> or set OKKHOR_DATA");
}

}  // namespace okkhor
