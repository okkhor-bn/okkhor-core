#include "parser.hpp"

namespace okkhor::bangla_to_latin {

Document parse(const std::vector<Token> &tokens) {
  Document document;

  OrthographicUnit current;
  bool has_current = false;

  auto close_unit = [&]() {
    if (!has_current)
      return;

    document.emplace_back(current);
    current = OrthographicUnit{};
    has_current = false;
  };

  for (std::size_t i = 0; i < tokens.size(); ++i) {

    const Token &token = tokens[i];

    switch (token.type) {

    case TokenType::Consonant: {
      Consonant c{token.canonical_key};

      if (!has_current) {
        current = make_consonant(c);
        has_current = true;
        break;
      }

      if (!add_consonant(current, c)) {
        close_unit();

        current = make_consonant(c);
        has_current = true;
      }

      break;
    }

    case TokenType::Vowel: {
      Vowel v{token.canonical_key};

      if (!has_current) {
        document.emplace_back(IndependentVowel{v, {}, false, false});
        break;
      }

      if (!add_vowel(current, v)) {
        close_unit();

        document.emplace_back(IndependentVowel{v, {}, false, false});
      }

      break;
    }

    case TokenType::Accent: {
      Accent a{token.canonical_key};

      if (!has_current) {
        document.emplace_back(Literal{token.value});
        break;
      }

      if (!add_accent(current, a)) {
        close_unit();
        document.emplace_back(Literal{token.value});
      }

      break;
    }

    case TokenType::Hasanta: {
      if (!has_current) {
        document.emplace_back(Literal{token.value});
        break;
      }

      const bool has_next = i + 1 < tokens.size();

      const bool next_is_consonant =
          has_next && tokens[i + 1].type == TokenType::Consonant;

      const bool next_is_zwnj =
          has_next && tokens[i + 1].type == TokenType::ZWNJ;

      if (next_is_consonant) {
        break;
      }

      if (next_is_zwnj) {
        document.emplace_back(Literal{token.value});
        break;
      }

      if (!terminate_with_hasanta(current)) {
        close_unit();
        document.emplace_back(Literal{token.value});
      }

      break;
    }

    case TokenType::ZWNJ: {
      if (!has_current) {
        document.emplace_back(Literal{token.value});
        break;
      }

      add_zwnj(current);
      break;
    }

    case TokenType::ZWJ: {
      if (!has_current) {
        document.emplace_back(Literal{token.value});
        break;
      }

      add_zwj(current);
      break;
    }

    case TokenType::VirtualConsonant: {
      close_unit();

      current = make_virtual_consonant();
      has_current = true;
      break;
    }

    case TokenType::Special: {
      close_unit();
      document.emplace_back(Literal{token.value});
      break;
    }

    case TokenType::Whitespace:
    case TokenType::Punctuation:
    case TokenType::Unknown:
      close_unit();
      document.emplace_back(Literal{token.value});
      break;
    }
  }

  close_unit();

  return document;
}

} // namespace okkhor::bangla_to_latin