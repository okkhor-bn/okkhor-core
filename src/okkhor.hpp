#pragma once
// Façade over the pipeline:
//   Latin -> tokenizer -> parser (algebra) -> renderer -> Bangla UTF-8
#include <string>
#include <vector>

#include "mapping.hpp"
#include "orthography.hpp"
#include "parser.hpp"
#include "renderer.hpp"
#include "tokenizer.hpp"

namespace okkhor {

class Engine {
public:
  explicit Engine(Mapping mapping)
      : mapping_(std::move(mapping)), renderer_(mapping_) {}

  static Engine from_data_dir(const std::string &data_dir) {
    return Engine(Mapping::load(data_dir));
  }

  std::vector<Token> tokenize_input(const std::string &input) const {
    return tokenize(input, mapping_);
  }

  // The internal representation, for tests and tooling.
  Document analyze(const std::string &input) const {
    return parse(tokenize(input, mapping_));
  }

  std::string transliterate(const std::string &input) const {
    return renderer_.render(analyze(input));
  }

  const Mapping &mapping() const { return mapping_; }
  const Renderer &renderer() const { return renderer_; }

private:
  Mapping mapping_;
  Renderer renderer_;
};

// Searches a few conventional locations for the data directory.
std::string find_data_dir(const std::string &hint = "");

} // namespace okkhor
