#include "parser.hpp"

namespace okkhor::bangla_to_latin {

Document parse(const std::vector<Token> &tokens, const Mapping &mapping) {

  Document document;

  OrthographicUnit current;
  bool has_current = false;

  std::size_t i = 0;

  auto close_unit = [&]() {
    if (!has_current)
      return;

    document.emplace_back(current);

    current = OrthographicUnit{};
    has_current = false;
  };

  auto peek = [&]() -> const Token * {
    if (i + 1 >= tokens.size())
      return nullptr;

    return &tokens[i + 1];
  };

  auto is_word_end = [&]() {
    if (i + 1 >= tokens.size())
      return true;

    const TokenType next_type = tokens[i + 1].type;

    return next_type == TokenType::Whitespace;
  };

  auto start_consonant = [&](Consonant consonant) {
    current = make_consonant(consonant);
    has_current = true;
  };

  while (i < tokens.size()) {

    const Token &token = tokens[i];

    switch (token.type) {

    case TokenType::Consonant: {

      Consonant consonant{token.canonical_key};

      const Token *next = peek();

      const bool followed_by_hasanta = next && next->type == TokenType::Hasanta;
      const bool followed_by_consonant =
          next && next->type == TokenType::Consonant;

      const bool after_hasanta =
          i > 0 && tokens[i - 1].type == TokenType::Hasanta;

      if (!has_current) {
        start_consonant(consonant);
        break;
      }

      if (!after_hasanta || current.explicit_hasanta) {
        if (!current.vowel && !current.explicit_hasanta) {
          add_vowel(current, Vowel{"o"});
        }

        close_unit();

        start_consonant(consonant);

        break;
      }

      current.conjuncts.emplace_back(DependentConsonant{consonant});

      if (!followed_by_hasanta) {
        if (!current.vowel && !current.explicit_hasanta &&
            followed_by_consonant) {
          add_vowel(current, Vowel{"o"});
        }
        close_unit();
      }

      break;
    }

    case TokenType::Vowel: {

      Vowel vowel{token.canonical_key};

      if (!has_current) {

        document.emplace_back(IndependentVowel{vowel, {}, false, false});

        break;
      }

      if (!add_vowel(current, vowel)) {

        close_unit();

        document.emplace_back(IndependentVowel{vowel, {}, false, false});
      }

      break;
    }

    case TokenType::Accent: {

      Accent accent{token.canonical_key};

      if (!has_current) {

        document.emplace_back(Literal{token.value});

        break;
      }

      if (!add_accent(current, accent)) {

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

      const Token *next = peek();

      const Token *prev = (i > 0) ? &tokens[i - 1] : nullptr;

      std::string possible_key =
          (prev ? prev->canonical_key : "") + (next ? next->canonical_key : "");

      const Rule *rule = next ? mapping.lookup(possible_key) : nullptr;

      const bool followed_by_consonant =
          next && next->type == TokenType::Consonant;

      const bool followed_by_zwnj = next && next->type == TokenType::ZWNJ;

      /*
       * Hasanta + consonant.
       *
       * Normally the hasanta is structural:
       *
       *     ন্‌ত
       *
       * The next consonant becomes a conjunct member,
       * so the hasanta itself does not need to be stored.
       *
       * However, if a rule exists for the sequence,
       * the hasanta is meaningful and must be preserved.
       */
      if (followed_by_consonant) {

        if (rule) {
          terminate_with_hasanta(current);
        }

        break;
      }

      /*
       * Explicit hasanta + ZWNJ.
       *
       * This must always preserve the hasanta.
       */
      if (followed_by_zwnj) {

        if (!terminate_with_hasanta(current)) {

          close_unit();

          document.emplace_back(Literal{token.value});
        }

        break;
      }

      /*
       * Standalone explicit hasanta.
       */
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
    case TokenType::Unknown: {

      close_unit();

      document.emplace_back(Literal{token.value});

      break;
    }
    }

    ++i;
  }

  /*
   * If the final unit is an ordinary open consonant,
   * give it its inherent vowel before closing.
   */
  if (has_current && !current.vowel && !current.explicit_hasanta &&
      !is_word_end()) {
    add_vowel(current, Vowel{"o"});
  }

  close_unit();

  return document;
}

} // namespace okkhor::bangla_to_latin