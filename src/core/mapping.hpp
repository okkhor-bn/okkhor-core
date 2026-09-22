#pragma once

// Phase 0: data-driven mapping tables (data/*.json).

#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

#include "core/token.hpp"

namespace okkhor {

struct VowelEntry {
  std::string canonical_key;       // canonical Latin key
  std::string independent; // "অ", "আ", ...
  std::string dependent;   // "", "া", ...
  std::vector<std::string> aliases;
};

struct ConsonantEntry {
  std::string canonical_key; // canonical Latin key
  std::string base;  // "ক"
  std::string fola;  // optional rendering of dcons inside conjunct
  std::vector<std::string> aliases;
};

struct OtherEntry {
  std::string canonical_key; // canonical Latin key
  std::string value;  // "ং", "ঁ", "ঃ"
  std::vector<std::string> aliases;
};

// What a matched Latin key means.
struct Rule {
  TokenType type = TokenType::Unknown;
  std::string canonical_key;
  std::string literal;
  std::string value;
};

class Mapping {
public:
  // Loads vowels.json, consonants.json, controls.json,
  // and punctuation.json.
  static Mapping load(const std::string &data_dir);

  // Looks up a Latin key, including aliases.
  const Rule *lookup(const std::string &key) const;

  std::size_t max_key_length() const { return max_key_len_; }

  // Canonical-key lookup.
  const VowelEntry *vowel(const std::string &canonical_key) const;
  const ConsonantEntry *consonant(const std::string &canonical_key) const;
  const OtherEntry *accent(const std::string &canonical_key) const;

  const std::unordered_map<std::string, VowelEntry> &vowels() const {
    return vowels_;
  }

  const std::unordered_map<std::string, ConsonantEntry> &consonants() const {
    return consonants_;
  }

  const std::unordered_map<std::string, OtherEntry> &accents() const {
    return others_;
  }


  void add_rule(const std::string &key, Rule rule);

  void set_rule(const std::string &key, Rule rule);

private:
  std::unordered_map<std::string, Rule> rules_;

  // Keyed by canonical Latin representation.
  std::unordered_map<std::string, VowelEntry> vowels_;
  std::unordered_map<std::string, ConsonantEntry> consonants_;
  std::unordered_map<std::string, OtherEntry> others_;

  std::size_t max_key_len_ = 1;
};

} // namespace okkhor