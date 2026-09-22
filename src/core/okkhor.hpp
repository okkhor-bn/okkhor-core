#pragma once
// Façade over the pipeline:
//   Latin -> tokenizer -> rule engine -> parser (algebra) -> renderer -> Bangla
//   UTF-8
#include <iostream>
#include <string>
#include <vector>

#include "latin_to_bangla/index.hpp"

#include "core/mapping.hpp"
#include "core/orthography.hpp"

namespace okkhor {

class Engine {
public:
  Engine(Mapping mapping, RuleEngine rule_engine)
      : mapping_(std::move(mapping)), rule_engine_(std::move(rule_engine)),
        renderer_(mapping_) {}

  static Engine from_data_dir(const std::string &data_dir) {
    Mapping m = Mapping::load(data_dir);

    RuleEngine re;
    std::string rules_path =
        data_dir.empty() ? "rules.json" : data_dir + "/rules.json";

    // Load contextual rules into RuleEngine
    re.load_file(rules_path, m);

    return Engine(std::move(m), std::move(re));
  }

  std::vector<Token> tokenize_input(const std::string &input) const {
    return tokenize(input, mapping_);
  }

  // The internal representation, incorporating contextual rule transformations
  Document analyze(const std::string &input) const {
    std::vector<Token> raw_tokens = tokenize(input, mapping_);
    std::vector<Token> rewritten_tokens =
        rule_engine_.apply(raw_tokens, mapping_);
    return parse(rewritten_tokens);
  }

  std::string transliterate(const std::string &input) const {
    return renderer_.render(analyze(input));
  }

  const Mapping &mapping() const { return mapping_; }
  const RuleEngine &rule_engine() const { return rule_engine_; }
  const Renderer &renderer() const { return renderer_; }

private:
  Mapping mapping_;
  RuleEngine rule_engine_;
  Renderer renderer_;
};

// Searches a few conventional locations for the data directory.
std::string find_data_dir(const std::string &hint = "");

} // namespace okkhor