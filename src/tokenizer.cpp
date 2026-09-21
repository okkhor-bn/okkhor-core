#include "tokenizer.hpp"

#include <algorithm>
#include <cctype>

#include "mapping.hpp"

namespace okkhor {
namespace {

bool is_space(unsigned char c) {
  return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' ||
         c == '\f';
}

// Length of the UTF-8 sequence starting at `c`, so unknown multi-byte input
// survives intact rather than being split into mojibake.
std::size_t utf8_length(unsigned char c) {
  if ((c & 0x80) == 0x00)
    return 1;
  if ((c & 0xE0) == 0xC0)
    return 2;
  if ((c & 0xF0) == 0xE0)
    return 3;
  if ((c & 0xF8) == 0xF0)
    return 4;
  return 1;
}

} // namespace

const char *token_type_name(TokenType t) {
  switch (t) {
  case TokenType::Vowel:
    return "Vowel";
  case TokenType::Consonant:
    return "Consonant";
  case TokenType::VirtualConsonant:
    return "VirtualConsonant";
  case TokenType::Accent:
    return "Accent";
  case TokenType::Hasanta:
    return "Hasanta";
  case TokenType::ZWNJ:
    return "ZWNJ";
  case TokenType::Whitespace:
    return "Whitespace";
  case TokenType::Punctuation:
    return "Punctuation";
  case TokenType::Unknown:
    return "Unknown";
  }
  return "Unknown";
}

std::vector<Token> tokenize(const std::string &input, const Mapping &mapping) {
  std::vector<Token> out;
  const std::size_t n = input.size();
  const std::size_t max_len = mapping.max_key_length();

  std::size_t i = 0;
  while (i < n) {
    // Longest match against the mapping data.
    bool matched = false;
    std::size_t limit = std::min(max_len, n - i);
    for (std::size_t len = limit; len >= 1; --len) {
      std::string key = input.substr(i, len);
      const Rule *rule = mapping.lookup(key);
      if (!rule)
        continue;
      Token t;
      t.type = rule->type;
      t.id = rule->id;
      t.latin = key;
      t.literal = rule->literal;
      out.push_back(std::move(t));
      i += len;
      matched = true;
      break;
    }
    if (matched)
      continue;

    unsigned char c = static_cast<unsigned char>(input[i]);
    if (is_space(c)) {
      Token t;
      t.type = TokenType::Whitespace;
      t.latin = t.literal = std::string(1, input[i]);
      out.push_back(std::move(t));
      ++i;
      continue;
    }

    // Unknown input is preserved verbatim, never discarded.
    std::size_t len = std::min(utf8_length(c), n - i);
    Token t;
    t.type = TokenType::Unknown;
    t.latin = t.literal = input.substr(i, len);
    out.push_back(std::move(t));
    i += len;
  }
  return out;
}

} // namespace okkhor
