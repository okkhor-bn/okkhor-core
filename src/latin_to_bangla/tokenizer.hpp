#pragma once

// Phase 4: Latin phonetic input -> semantic tokens.
// Longest match wins.

#include <string>
#include <vector>
#include "core/token.hpp"

namespace okkhor {

class Mapping;



struct Token {
  TokenType type = TokenType::Unknown;
  // Canonical Latin representation of this token.
  //
  // Examples:
  //
  //   "k" -> "k"
  //   "S" -> "sh"       (alias)
  //   "sh" -> "sh"      (canonical)
  //
  // Controls and literals generally leave this empty.
  std::string canonical_key;

  // The exact Latin input text consumed by the tokenizer.
  //
  // Example:
  //
  //   input = "S"
  //   latin = "S"
  //   canonical_key = "sh"
  //
  std::string latin;

  // Pass-through output for punctuation, whitespace,
  // unknown input, and controls where appropriate.
  std::string literal;
};

std::vector<Token> tokenize(const std::string &input, const Mapping &mapping);

const char *token_type_name(TokenType t);

} // namespace okkhor