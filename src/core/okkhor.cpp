#include "core/okkhor.hpp"

#include <fstream>
#include <iterator>
#include <stdexcept>
#include <string>

#include "default_data.hpp"
#include "util/json.hpp"

namespace okkhor {

namespace {

std::string read_file(const std::string &path) {
  std::ifstream file(path, std::ios::binary);

  if (!file) {
    throw std::runtime_error("could not open Okkhor data file: " + path);
  }

  return std::string(std::istreambuf_iterator<char>(file),
                     std::istreambuf_iterator<char>());
}

std::string data_path(const std::string &data_dir,
                      const std::string &filename) {
  if (data_dir.empty()) {
    return filename;
  }

  return data_dir + "/" + filename;
}

} // namespace

Engine::Engine()
    : mapping_(Mapping::load(json::parse(default_data::vowels),
                             json::parse(default_data::consonants),
                             json::parse(default_data::controls),
                             json::parse(default_data::punctuation))),
      latin_to_bangla_rules_(), bangla_to_latin_rules_(),
      latin_to_bangla_renderer_(mapping_), bangla_to_latin_renderer_(mapping_) {


  latin_to_bangla_rules_.load(json::parse(default_data::rules), mapping_);
  bangla_to_latin_rules_.load(json::parse(default_data::rules), mapping_);
}

Engine::Engine(const std::string &data_dir)
    : mapping_(Mapping::load(
          json::parse(read_file(data_path(data_dir, "vowels.json"))),
          json::parse(read_file(data_path(data_dir, "consonants.json"))),
          json::parse(read_file(data_path(data_dir, "controls.json"))),
          json::parse(read_file(data_path(data_dir, "punctuation.json"))))),
      latin_to_bangla_rules_(), bangla_to_latin_rules_(),
      latin_to_bangla_renderer_(mapping_), bangla_to_latin_renderer_(mapping_) {

  const std::string rules_json = read_file(data_path(data_dir, "rules.json"));
  latin_to_bangla_rules_.load(json::parse(rules_json), mapping_);
  bangla_to_latin_rules_.load(json::parse(rules_json), mapping_);
}


} // namespace okkhor