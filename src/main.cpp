
// okkhor - interactive / piped Bangla phonetic transliteration.
//
//   okkhor                     read stdin line by line
//   okkhor amar sonar          Latin -> Bangla
//   okkhor --reverse আমি       Bangla -> Latin
//   okkhor --tokens kta        dump the internal representation
//
// Latin -> Bangla is the default direction.

#include <iostream>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

#include "core/okkhor.hpp"

namespace {

enum class Direction { LatinToBangla, BanglaToLatin };

void print_tokens(const std::vector<okkhor::Token> &tokens) {

  for (const okkhor::Token &token : tokens) {
    std::cout << " " << okkhor::token_type_name(token.type) << "("
              << token.canonical_key << " : " << token.value << ")";
  }

  std::cout << "\n";
}

void print_structure(const okkhor::Mapping &mapping,
                     const okkhor::Document &document) {

  std::cout << "structure   :";

  for (const okkhor::Element &element : document) {

    std::visit(
        [&](const auto &unit) {
          using T = std::decay_t<decltype(unit)>;

          if constexpr (std::is_same_v<T, okkhor::OrthographicUnit>) {

            std::cout << " [base=";

            if (const auto *bc =
                    std::get_if<okkhor::BaseConsonant>(&unit.base)) {

              const auto *consonant = mapping.consonant(bc->value.key);

              if (consonant) {
                std::cout << consonant->canonical_key;
              } else {
                std::cout << "?";
              }

            } else {
              std::cout << "vcons";
            }

            for (const auto &dc : unit.conjuncts) {

              const auto *consonant = mapping.consonant(dc.value.key);

              std::cout << " +dcons(";

              if (consonant) {
                std::cout << consonant->canonical_key;
              } else {
                std::cout << "?";
              }

              std::cout << ")";
            }

            if (unit.vowel) {

              const auto *vowel = mapping.vowel(unit.vowel->value.key);

              std::cout << " +dvowel(";

              if (vowel) {
                std::cout << vowel->canonical_key;
              } else {
                std::cout << "?";
              }

              std::cout << ")";
            }

            for (const auto &accent : unit.accents) {

              const auto *entry = mapping.accent(accent.key);

              std::cout << " +accent(";

              if (entry) {
                std::cout << entry->canonical_key;
              } else {
                std::cout << "?";
              }

              std::cout << ")";
            }

            if (unit.explicit_hasanta) {
              std::cout << " +hasanta";
            }

            if (unit.zwnj_after) {
              std::cout << " +zwnj";
            }

            if (unit.zwj_after) {
              std::cout << " +zwj";
            }

            std::cout << "]";

          } else if constexpr (std::is_same_v<T, okkhor::IndependentVowel>) {

            const auto *vowel = mapping.vowel(unit.value.key);

            std::cout << " [vowel(";

            if (vowel) {
              std::cout << vowel->canonical_key;
            } else {
              std::cout << "?";
            }

            std::cout << ")]";

            if (unit.zwnj_after) {
              std::cout << " +zwnj";
            }

            if (unit.zwj_after) {
              std::cout << " +zwj";
            }

          } else if constexpr (std::is_same_v<T, okkhor::Literal>) {

            std::cout << " [literal \"" << unit.text << "\"]";
          }
        },
        element);
  }

  std::cout << "\n";
}

void describe(const okkhor::Engine &engine, const std::string &input,
              Direction direction) {

  std::cout << "input       : " << input << "\n";

  // -----------------------------------------------------------------
  // Step 1: Tokenize
  // -----------------------------------------------------------------

  std::vector<okkhor::Token> raw_tokens = engine.tokenize_input(input);

  std::cout << "raw tokens  :";
  print_tokens(raw_tokens);

  // -----------------------------------------------------------------
  // Latin -> Bangla
  // -----------------------------------------------------------------

  if (direction == Direction::LatinToBangla) {

    // Step 2: Apply Latin -> Bangla rules.

    std::vector<okkhor::Token> rewritten_tokens =
        engine.latin_to_bangla_rules().apply(raw_tokens, engine.mapping());

    std::cout << "rule tokens :";
    print_tokens(rewritten_tokens);

    // Step 3: Parse into orthographic structure.

    const okkhor::Document document = engine.analyze_latin_to_bangla(input);

    print_structure(engine.mapping(), document);

    // Step 4: Render.

    std::cout << "output      : " << engine.transliterate_latin_to_bangla(input)
              << "\n";

    return;
  }

  // -----------------------------------------------------------------
  // Bangla -> Latin
  // -----------------------------------------------------------------

  // Step 2: Apply reversible Bangla -> Latin rules.

  std::vector<okkhor::Token> rewritten_tokens =
      engine.bangla_to_latin_rules().apply(raw_tokens, engine.mapping());

  std::cout << "rule tokens :";
  print_tokens(rewritten_tokens);

  // Step 3: Parse into orthographic structure.

  const okkhor::Document document = engine.analyze_bangla_to_latin(input);

  print_structure(engine.mapping(), document);

  // Step 4: Render.

  std::cout << "output      : " << engine.transliterate_bangla_to_latin(input)
            << "\n";
}

} // namespace

int main(int argc, char **argv) {

  std::string data_dir;
  bool verbose = false;

  Direction direction = Direction::LatinToBangla;

  std::vector<std::string> words;

  for (int i = 1; i < argc; ++i) {

    std::string arg = argv[i];

    if (arg == "--data" && i + 1 < argc) {

      data_dir = argv[++i];

    } else if (arg == "--tokens" || arg == "-v") {

      verbose = true;

    } else if (arg == "--reverse" || arg == "-r") {

      direction = Direction::BanglaToLatin;

    } else if (arg == "--help" || arg == "-h") {

      std::cout << "usage: okkhor "
                << "[--data DIR] "
                << "[--reverse] "
                << "[--tokens] "
                << "[text...]\n";

      std::cout << "\n"
                << "default: Latin -> Bangla\n"
                << "--reverse, -r: Bangla -> Latin\n"
                << "--tokens, -v: show internal representation\n";

      return 0;

    } else {

      words.push_back(arg);
    }
  }

  try {

    okkhor::Engine engine =
        okkhor::Engine::from_data_dir(okkhor::find_data_dir(data_dir));

    // -----------------------------------------------------------------
    // Arguments
    // -----------------------------------------------------------------

    if (!words.empty()) {

      std::string joined;

      for (std::size_t i = 0; i < words.size(); ++i) {

        if (i) {
          joined += ' ';
        }

        joined += words[i];
      }

      if (verbose) {

        describe(engine, joined, direction);

      } else if (direction == Direction::LatinToBangla) {

        std::cout << engine.transliterate_latin_to_bangla(joined) << "\n";

      } else {

        std::cout << engine.transliterate_bangla_to_latin(joined) << "\n";
      }

      return 0;
    }

    // -----------------------------------------------------------------
    // stdin
    // -----------------------------------------------------------------

    std::string line;

    while (std::getline(std::cin, line)) {

      if (verbose) {

        describe(engine, line, direction);

      } else if (direction == Direction::LatinToBangla) {

        std::cout << engine.transliterate_latin_to_bangla(line) << "\n";

      } else {

        std::cout << engine.transliterate_bangla_to_latin(line) << "\n";
      }
    }

    return 0;

  } catch (const std::exception &e) {

    std::cerr << "okkhor: " << e.what() << "\n";

    return 1;
  }
}