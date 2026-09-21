#pragma once
// Phase 4: Latin phonetic input -> semantic tokens. Longest match wins.
#include <string>
#include <vector>

namespace okkhor {

class Mapping;

enum class TokenType {
  Vowel,
  Consonant,
  VirtualConsonant, // ` : the vcons base
  Accent,
  Hasanta, // ,,
  ZWNJ,    // |
  Whitespace,
  Punctuation,
  Unknown
};

struct Token {
  TokenType type = TokenType::Unknown;
  int id = -1;       // index into the vowel / consonant / accent tables
  std::string latin; // the input text this token consumed
  std::string
      literal; // pass-through output (punctuation, whitespace, unknown, ZWNJ)
};

std::vector<Token> tokenize(const std::string &input, const Mapping &mapping);

const char *token_type_name(TokenType t);

} // namespace okkhor
