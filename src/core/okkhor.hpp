
#pragma once

// Façade over the pipelines:
//
// Latin -> tokenizer -> rule engine -> parser -> renderer -> Bangla
// Bangla -> tokenizer -> rule engine -> parser -> renderer -> Latin
//
// The core engine owns shared mapping data and exposes both directions.
//
// Direction-specific implementation stays inside latin_to_bangla and
// bangla_to_latin namespaces.

#include <string>
#include <utility>
#include <vector>

#include "core/mapping.hpp"
#include "core/orthography.hpp"
#include "core/token.hpp"
#include "core/tokenizer.hpp"

#include "bangla_to_latin/index.hpp"
#include "latin_to_bangla/index.hpp"

namespace okkhor {

class Engine {

public:
  Engine(Mapping mapping, latin_to_bangla::RuleEngine latin_to_bangla_rules,
         bangla_to_latin::RuleEngine bangla_to_latin_rules)

      : mapping_(std::move(mapping)),
        latin_to_bangla_rules_(std::move(latin_to_bangla_rules)),
        bangla_to_latin_rules_(std::move(bangla_to_latin_rules)),
        latin_to_bangla_renderer_(mapping_),
        bangla_to_latin_renderer_(mapping_) {}

  static Engine from_data_dir(const std::string &data_dir) {

    Mapping mapping = Mapping::load(data_dir);

    const std::string rules_path =
        data_dir.empty() ? "rules.json" : data_dir + "/rules.json";

    // -------------------------------------------------------------
    // Latin -> Bangla rule engine
    // -------------------------------------------------------------

    latin_to_bangla::RuleEngine latin_to_bangla_rules;

    latin_to_bangla_rules.load_file(rules_path, mapping);

    // -------------------------------------------------------------
    // Bangla -> Latin rule engine
    // -------------------------------------------------------------

    bangla_to_latin::RuleEngine bangla_to_latin_rules;

    bangla_to_latin_rules.load_file(rules_path, mapping);

    return Engine(std::move(mapping), std::move(latin_to_bangla_rules),
                  std::move(bangla_to_latin_rules));
  }

  // -----------------------------------------------------------------
  // Shared tokenization
  // -----------------------------------------------------------------

  std::vector<Token> tokenize_input(const std::string &input) const {

    return tokenize(input, mapping_);
  }

  // -----------------------------------------------------------------
  // Latin -> Bangla
  // -----------------------------------------------------------------

  Document analyze_latin_to_bangla(const std::string &input) const {

    std::vector<Token> raw_tokens = tokenize(input, mapping_);

    std::vector<Token> rewritten_tokens =
        latin_to_bangla_rules_.apply(raw_tokens, mapping_);

    return latin_to_bangla::parse(rewritten_tokens);
  }

  std::string transliterate_latin_to_bangla(const std::string &input) const {

    return latin_to_bangla_renderer_.render(analyze_latin_to_bangla(input));
  }

  // -----------------------------------------------------------------
  // Bangla -> Latin
  // -----------------------------------------------------------------

  std::vector<Token>
  analyze_bangla_to_latin_tokens(const std::string &input) const {

    std::vector<Token> raw_tokens = tokenize(input, mapping_);

    std::vector<Token> rewritten_tokens =
        bangla_to_latin_rules_.apply(raw_tokens, mapping_);

    return rewritten_tokens;
  }

  Document analyze_bangla_to_latin(const std::string &input) const {

    std::vector<Token> rewritten_tokens = analyze_bangla_to_latin_tokens(input);

    return bangla_to_latin::parse(rewritten_tokens);
  }

  std::string transliterate_bangla_to_latin(const std::string &input) const {

    return bangla_to_latin_renderer_.render(analyze_bangla_to_latin(input));
  }

  // -----------------------------------------------------------------
  // Shared state
  // -----------------------------------------------------------------

  const Mapping &mapping() const { return mapping_; }

  const latin_to_bangla::RuleEngine &latin_to_bangla_rules() const {

    return latin_to_bangla_rules_;
  }

  const bangla_to_latin::RuleEngine &bangla_to_latin_rules() const {

    return bangla_to_latin_rules_;
  }

  const latin_to_bangla::Renderer &latin_to_bangla_renderer() const {

    return latin_to_bangla_renderer_;
  }

  const bangla_to_latin::Renderer &bangla_to_latin_renderer() const {

    return bangla_to_latin_renderer_;
  }

private:
  Mapping mapping_;

  // Latin -> Bangla rules.
  latin_to_bangla::RuleEngine latin_to_bangla_rules_;

  // Bangla -> Latin rules.
  //
  // This reads the SAME rules.json, but its RuleEngine
  // only constructs reverse rules from `when: { always: true }`.
  bangla_to_latin::RuleEngine bangla_to_latin_rules_;

  latin_to_bangla::Renderer latin_to_bangla_renderer_;

  bangla_to_latin::Renderer bangla_to_latin_renderer_;
};

// Searches a few conventional locations for the data directory.
std::string find_data_dir(const std::string &hint = "");

} // namespace okkhor
