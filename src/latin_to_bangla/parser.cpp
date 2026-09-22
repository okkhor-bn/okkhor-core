#include "parser.hpp"

#include <iostream>
#include <optional>

namespace okkhor::latin_to_bangla {

Document parse(const std::vector<Token> &tokens) {

  Document doc;

  std::optional<OrthographicUnit> cur;

  // The unit currently being built.
  auto flush = [&] {
    if (cur) {
      doc.push_back(*cur);
      cur.reset();
    }
  };

  for (int i = 0; i < tokens.size(); i++) {

    const Token &t = tokens[i];

    switch (t.type) {

    case TokenType::Consonant: {

      // C + C -> BC + DC + VN
      // Otherwise start a fresh unit.

      Consonant c{t.canonical_key};

      if (cur && add_consonant(*cur, c))
        break;

      flush();

      cur = make_consonant(c);

      break;
    }

    case TokenType::VirtualConsonant: {

      // ` always opens a new unit with a
      // visually-nothing base.

      flush();

      cur = make_virtual_consonant();

      break;
    }

    case TokenType::Vowel: {

      // C + V -> BC + DV
      // A vowel with no open base stands alone.

      Vowel v{t.canonical_key};

      if (cur && add_vowel(*cur, v))
        break;

      flush();

      IndependentVowel independent{v, {}, false, false};

      doc.push_back(std::move(independent));

      break;
    }

    case TokenType::Accent: {

      Accent a{t.canonical_key};

      if (cur && add_accent(*cur, a))
        break;

      if (!cur && !doc.empty() &&
          std::holds_alternative<IndependentVowel>(doc.back())) {

        std::get<IndependentVowel>(doc.back()).accents.push_back(a);

        break;
      }

      flush();

      OrthographicUnit u = make_virtual_consonant();

      add_accent(u, a);

      doc.push_back(std::move(u));

      break;
    }

    case TokenType::Hasanta: {

      bool next_is_zwnj = tokens.size() > i + 1 && tokens[i + 1].type == TokenType::ZWNJ;

      if(next_is_zwnj){
        add_hasanta(*cur);
        break;
      }

      if (cur && terminate_with_hasanta(*cur)) {
        flush();
        break;
      }
      flush();
      OrthographicUnit u = make_virtual_consonant();
      terminate_with_hasanta(u);
      doc.push_back(std::move(u));
      break;
    }

    case TokenType::ZWNJ: {

      if (cur) {

        add_zwnj(*cur);

        flush();

      } else if (!doc.empty() &&
                 std::holds_alternative<IndependentVowel>(doc.back())) {
        std::get<IndependentVowel>(doc.back()).zwnj_after = true;
      } else {
        doc.push_back(Literal{t.canonical_key});
      }

      break;
    }

    case TokenType::Whitespace:
    case TokenType::Punctuation:
    case TokenType::Unknown:
    default: {

      // Whitespace, punctuation and unknown input
      // close the current unit and pass through
      // untouched.

      flush();

      doc.push_back(Literal{t.value});

      break;
    }
    }
  }

  flush();

  return doc;
}

} // namespace okkhor::latin_to_bangla