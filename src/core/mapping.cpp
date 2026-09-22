#include "mapping.hpp"

#include <algorithm>
#include <stdexcept>
#include <utility>

#include "util/json.hpp"

namespace okkhor {

namespace {

std::string join(const std::string &dir, const char *file) {
  if (dir.empty())
    return file;

  char last = dir.back();

  if (last == '/' || last == '\\')
    return dir + file;

  return dir + "/" + file;
}

std::string capitalize_first_ascii(const std::string &key) {
  if (key.empty())
    return key;

  std::string result = key;

  if (result[0] >= 'a' && result[0] <= 'z')
    result[0] = static_cast<char>(result[0] - 'a' + 'A');

  return result;
}

} // namespace

void Mapping::add_rule(const std::string &key, Rule rule) {
  if (key.empty())
    throw std::runtime_error("empty key in mapping data");

  if (rules_.count(key))
    throw std::runtime_error("duplicate mapping key: " + key);

  max_key_len_ = std::max(max_key_len_, key.size());

  rules_.emplace(key, std::move(rule));
}

const Rule *Mapping::lookup(const std::string &key) const {
  auto it = rules_.find(key);

  return it == rules_.end() ? nullptr : &it->second;
}

const VowelEntry *Mapping::vowel(const std::string &canonical_key) const {
  auto it = vowels_.find(canonical_key);

  if (it == vowels_.end())
    throw std::out_of_range("unknown vowel key: " + canonical_key);

  return &it->second;
}

const ConsonantEntry *
Mapping::consonant(const std::string &canonical_key) const {
  auto it = consonants_.find(canonical_key);

  if (it == consonants_.end())
    throw std::out_of_range("unknown consonant key: " + canonical_key);

  return &it->second;
}

const AccentEntry *Mapping::accent(const std::string &canonical_key) const {
  auto it = accents_.find(canonical_key);

  if (it == accents_.end())
    throw std::out_of_range("unknown accent key: " + canonical_key);

  return &it->second;
}

Mapping Mapping::load(const std::string &data_dir) {
  Mapping m;

  // =========================================================
  // vowels.json
  // =========================================================

  {
    json::Value doc = json::parse_file(join(data_dir, "vowels.json"));

    if (!doc.is_object())
      throw std::runtime_error("vowels.json: expected an object");

    // -----------------------------------------------------
    // Pass 1: canonical entries
    // -----------------------------------------------------

    for (const auto &[latin, spec] : doc.as_object()) {
      VowelEntry e;

      e.latin = latin;
      e.independent = spec.string_or("ind", "");
      e.dependent = spec.string_or("dep", "");

      if (m.vowels_.count(latin))
        throw std::runtime_error("duplicate vowel key: " + latin);

      m.vowels_.emplace(latin, std::move(e));

      m.add_rule(latin, Rule{TokenType::Vowel, latin, ""});
    }

    // -----------------------------------------------------
    // Pass 2: explicit aliases
    // -----------------------------------------------------

    for (const auto &[latin, spec] : doc.as_object()) {
      const json::Value *aliases = spec.find("aliases");

      if (!aliases || !aliases->is_array())
        continue;

      for (const auto &alias : aliases->as_array()) {
        if (!alias.is_string())
          throw std::runtime_error("vowels.json: alias must be a string");

        const std::string &alias_key = alias.as_string();

        m.add_rule(alias_key, Rule{TokenType::Vowel, latin, ""});
      }
    }

    // -----------------------------------------------------
    // Pass 3: automatic capitalized aliases
    //
    // Only create the alias if the key is still unused.
    // Explicit canonical keys and explicit aliases always
    // take precedence.
    // -----------------------------------------------------

    for (const auto &[latin, spec] : doc.as_object()) {
      (void)spec;

      std::string capitalized = capitalize_first_ascii(latin);

      if (capitalized != latin && !m.rules_.count(capitalized)) {

        m.add_rule(capitalized, Rule{TokenType::Vowel, latin, ""});
      }
    }
  }

  // =========================================================
  // consonants.json
  // =========================================================

  {
    json::Value doc = json::parse_file(join(data_dir, "consonants.json"));

    if (!doc.is_object())
      throw std::runtime_error("consonants.json: expected an object");

    // -----------------------------------------------------
    // Pass 1: canonical entries
    // -----------------------------------------------------

    for (const auto &[latin, spec] : doc.as_object()) {
      ConsonantEntry e;

      e.latin = latin;
      e.base = spec.string_or("base", "");
      e.fola = spec.string_or("fola", "");

      if (m.consonants_.count(latin))
        throw std::runtime_error("duplicate consonant key: " + latin);

      m.consonants_.emplace(latin, std::move(e));

      m.add_rule(latin, Rule{TokenType::Consonant, latin, ""});
    }

    // -----------------------------------------------------
    // Pass 2: explicit aliases
    // -----------------------------------------------------

    for (const auto &[latin, spec] : doc.as_object()) {
      const json::Value *aliases = spec.find("aliases");

      if (!aliases || !aliases->is_array())
        continue;

      for (const auto &alias : aliases->as_array()) {
        if (!alias.is_string())
          throw std::runtime_error("consonants.json: alias must be a string");

        const std::string &alias_key = alias.as_string();

        m.add_rule(alias_key, Rule{TokenType::Consonant, latin, ""});
      }
    }

    // -----------------------------------------------------
    // Pass 3: automatic capitalized aliases
    // -----------------------------------------------------

    for (const auto &[latin, spec] : doc.as_object()) {
      (void)spec;

      std::string capitalized = capitalize_first_ascii(latin);

      if (capitalized != latin && !m.rules_.count(capitalized)) {

        m.add_rule(capitalized, Rule{TokenType::Consonant, latin, ""});
      }
    }
  }

  // =========================================================
  // controls.json
  // =========================================================

  {
    json::Value doc = json::parse_file(join(data_dir, "controls.json"));

    if (!doc.is_object())
      throw std::runtime_error("controls.json: expected an object");

    auto simple = [&](const char *section, TokenType type) {
      const json::Value *v = doc.find(section);

      if (!v || !v->is_object())
        return;

      for (const auto &[key, out] : v->as_object()) {

        m.add_rule(key, Rule{type, "", out.is_string() ? out.as_string() : ""});
      }
    };

    simple("hasanta", TokenType::Hasanta);
    simple("zwnj", TokenType::ZWNJ);
    simple("zwj", TokenType::ZWJ);
    simple("virtual", TokenType::VirtualConsonant);

    // -----------------------------------------------------
    // Accents
    // -----------------------------------------------------

    if (const json::Value *acc = doc.find("accents"); acc && acc->is_object()) {

      // Pass 1: canonical accent entries

      for (const auto &[latin, spec] : acc->as_object()) {

        AccentEntry e;

        e.latin = latin;
        e.sign = spec.string_or("sign", "");

        if (m.accents_.count(latin))
          throw std::runtime_error("duplicate accent key: " + latin);

        m.accents_.emplace(latin, std::move(e));

        m.add_rule(latin, Rule{TokenType::Accent, latin, ""});
      }

      // Pass 2: explicit aliases

      for (const auto &[latin, spec] : acc->as_object()) {

        const json::Value *aliases = spec.find("aliases");

        if (!aliases || !aliases->is_array())
          continue;

        for (const auto &alias : aliases->as_array()) {

          if (!alias.is_string())
            throw std::runtime_error("controls.json: "
                                     "accent alias must be a string");

          const std::string &alias_key = alias.as_string();

          m.add_rule(alias_key, Rule{TokenType::Accent, latin, ""});
        }
      }

      // Pass 3: automatic capitalized aliases

      for (const auto &[latin, spec] : acc->as_object()) {

        (void)spec;

        std::string capitalized = capitalize_first_ascii(latin);

        if (capitalized != latin && !m.rules_.count(capitalized)) {

          m.add_rule(capitalized, Rule{TokenType::Accent, latin, ""});
        }
      }
    }
  }

  // =========================================================
  // punctuation.json
  // =========================================================

  {
    json::Value doc = json::parse_file(join(data_dir, "punctuation.json"));

    if (!doc.is_object())
      throw std::runtime_error("punctuation.json: expected an object");

    for (const auto &[key, out] : doc.as_object()) {

      m.add_rule(key, Rule{TokenType::Punctuation, "",
                           out.is_string() ? out.as_string() : key});
    }
  }

  return m;
}

} // namespace okkhor