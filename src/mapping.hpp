#pragma once
// Phase 0: the data-driven mapping tables (data/*.json).
#include <string>
#include <unordered_map>
#include <vector>

#include "tokenizer.hpp"

namespace okkhor {

struct VowelEntry {
  int id = -1;
  std::string latin;
  std::string independent; // "অ", "আ", ...
  std::string dependent;   // "", "া", ...  (empty for `o` on purpose)
};

struct ConsonantEntry {
  int id = -1;
  std::string latin;
  std::string base; // "ক"
  std::string fola; // optional rendering of dcons inside a conjunct; "" =
                    // hosonto + base
};

struct AccentEntry {
  int id = -1;
  std::string latin;
  std::string sign; // "ং", "ঁ", "ঃ"
};

// What a matched Latin key means.
struct Rule {
  TokenType type = TokenType::Unknown;
  int id = -1;
  std::string literal;
};

class Mapping {
public:
  // Loads vowels.json, consonants.json, controls.json, punctuation.json.
  static Mapping load(const std::string &data_dir);

  const Rule *lookup(const std::string &key) const;
  std::size_t max_key_length() const { return max_key_len_; }

  const VowelEntry &vowel(int id) const;
  const ConsonantEntry &consonant(int id) const;
  const AccentEntry &accent(int id) const;

  const std::vector<VowelEntry> &vowels() const { return vowels_; }
  const std::vector<ConsonantEntry> &consonants() const { return consonants_; }

  // Convenience for tests and tooling: Latin key -> table id (-1 if absent).
  int vowel_id(const std::string &latin) const;
  int consonant_id(const std::string &latin) const;

private:
  void add_rule(const std::string &key, Rule rule);

  std::unordered_map<std::string, Rule> rules_;
  std::vector<VowelEntry> vowels_;
  std::vector<ConsonantEntry> consonants_;
  std::vector<AccentEntry> accents_;
  std::size_t max_key_len_ = 1;
};

} // namespace okkhor
