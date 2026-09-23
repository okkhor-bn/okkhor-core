#pragma once

// Façade over the pipelines:
//
// Latin -> tokenizer -> rule engine -> parser -> renderer -> Bangla
// Bangla -> tokenizer -> rule engine -> parser -> renderer -> Latin
//
// The core engine owns shared mapping data and exposes both directions.
//
// Default data is embedded into the executable at build time.
// An explicit data directory may be supplied to load external data.
//
// Direction-specific implementation stays inside latin_to_bangla and
// bangla_to_latin namespaces.

#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "core/mapping.hpp"
#include "core/orthography.hpp"
#include "core/token.hpp"
#include "core/tokenizer.hpp"
#include "util/json.hpp"

#include "bangla_to_latin/index.hpp"
#include "latin_to_bangla/index.hpp"

namespace okkhor {

class Engine {

public:
  // -------------------------------------------------------------
  // Construction
  // -------------------------------------------------------------

  // Uses the JSON data embedded into the executable.
  Engine();

  // Uses JSON data from an explicitly supplied directory.
  explicit Engine(const std::string &data_dir);

  // Construct directly from already-loaded objects.
  Engine(Mapping mapping, latin_to_bangla::RuleEngine latin_to_bangla_rules,
         bangla_to_latin::RuleEngine bangla_to_latin_rules)
      : mapping_(std::move(mapping)),
        latin_to_bangla_rules_(std::move(latin_to_bangla_rules)),
        bangla_to_latin_rules_(std::move(bangla_to_latin_rules)),
        latin_to_bangla_renderer_(mapping_),
        bangla_to_latin_renderer_(mapping_) {}

  // -------------------------------------------------------------
  // Shared tokenization
  // -------------------------------------------------------------

  std::vector<Token> tokenize_input(const std::string &input,
                                    WorkingDirection direction) const {
    return tokenize(input, mapping_, direction);
  }

  // -------------------------------------------------------------
  // Latin -> Bangla
  // -------------------------------------------------------------

  Document analyze_latin_to_bangla(const std::string &input) const {
    std::vector<Token> raw_tokens =
        tokenize(input, mapping_, WorkingDirection::Forward);
    std::vector<Token> rewritten_tokens =
        latin_to_bangla_rules_.apply(raw_tokens, mapping_);
    return latin_to_bangla::parse(rewritten_tokens, mapping_);
  }

  std::string transliterate_latin_to_bangla(const std::string &input) const {
    return latin_to_bangla_renderer_.render(analyze_latin_to_bangla(input));
  }

  // -------------------------------------------------------------
  // Bangla -> Latin
  // -------------------------------------------------------------

  std::vector<Token>
  analyze_bangla_to_latin_tokens(const std::string &input) const {
    std::vector<Token> raw_tokens =
        tokenize(input, mapping_, WorkingDirection::Reverse);

    std::vector<Token> rewritten_tokens = raw_tokens;
    // bangla_to_latin_rules_.apply(raw_tokens, mapping_);

    return rewritten_tokens;
  }

  Document analyze_bangla_to_latin(const std::string &input) const {
    std::vector<Token> rewritten_tokens = analyze_bangla_to_latin_tokens(input);

    return bangla_to_latin::parse(rewritten_tokens, mapping_);
  }

  std::string analyze_bangla_to_latin_string(const std::string &input) const {
    return bangla_to_latin_rules_.apply(input);
  }

  std::string transliterate_bangla_to_latin(const std::string &input) const {
    return analyze_bangla_to_latin_string(
        bangla_to_latin_renderer_.render(analyze_bangla_to_latin(input)));
  }

  // -------------------------------------------------------------
  // Shared state
  // -------------------------------------------------------------

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

  Engine(const Engine &) = delete;
  Engine &operator=(const Engine &) = delete;
  Engine(Engine &&) = delete;
  Engine &operator=(Engine &&) = delete;

private:
  Mapping mapping_;

  // Latin -> Bangla rules.
  latin_to_bangla::RuleEngine latin_to_bangla_rules_;

  // Bangla -> Latin rules.
  //
  // This reads the SAME rules.json, but its RuleEngine
  // constructs reverse rules according to its own logic.
  bangla_to_latin::RuleEngine bangla_to_latin_rules_;

  latin_to_bangla::Renderer latin_to_bangla_renderer_;
  bangla_to_latin::Renderer bangla_to_latin_renderer_;
};

} // namespace okkhor