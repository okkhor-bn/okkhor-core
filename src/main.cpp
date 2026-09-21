// okkhor - interactive / piped Bangla phonetic transliteration.
//
//   okkhor                 read stdin line by line
//   okkhor amar sonar      transliterate the arguments
//   okkhor --tokens kta    dump the internal representation
#include <iostream>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

#include "okkhor.hpp"

namespace {

void describe(const okkhor::Engine &engine, const std::string &input) {
  std::cout << "input       : " << input << "\n";

  // Step 1: Tokenize
  std::vector<okkhor::Token> raw_tokens = engine.tokenize_input(input);

  std::cout << "raw tokens  :";
  for (const okkhor::Token &t : raw_tokens)
    std::cout << " " << okkhor::token_type_name(t.type) << "(" << t.latin
              << ")";
  std::cout << "\n";

  // Step 2: Apply contextual rules
  std::vector<okkhor::Token> rewritten_tokens =
      engine.rule_engine().apply(raw_tokens, engine.mapping());

  std::cout << "rule tokens :";
  for (const okkhor::Token &t : rewritten_tokens)
    std::cout << " " << okkhor::token_type_name(t.type) << "(" << t.latin
              << ")";
  std::cout << "\n";

  // Step 3: Parse rewritten tokens into AST / Document structure
  const okkhor::Document doc = okkhor::parse(rewritten_tokens);
  std::cout << "structure   :";
  for (const okkhor::Element &e : doc) {
    std::visit(
        [&](const auto &u) {
          using T = std::decay_t<decltype(u)>;
          if constexpr (std::is_same_v<T, okkhor::OrthographicUnit>) {
            std::cout << " [base=";
            if (const auto *bc = std::get_if<okkhor::BaseConsonant>(&u.base))
              std::cout << engine.mapping().consonant(bc->value.id).latin;
            else
              std::cout << "vcons";
            for (const auto &dc : u.conjuncts)
              std::cout << " +dcons("
                        << engine.mapping().consonant(dc.value.id).latin << ")";
            if (u.vowel)
              std::cout << " +dvowel("
                        << engine.mapping().vowel(u.vowel->value.id).latin
                        << ")";
            for (const auto &a : u.accents)
              std::cout << " +accent(" << engine.mapping().accent(a.id).latin
                        << ")";
            if (u.explicit_hasanta)
              std::cout << " +hasanta";
            if (u.zwnj_after)
              std::cout << " +zwnj";
            std::cout << "]";
          } else if constexpr (std::is_same_v<T, okkhor::IndependentVowel>) {
            std::cout << " [vowel(" << engine.mapping().vowel(u.value.id).latin
                      << ")]";
          } else {
            std::cout << " [literal \"" << u.text << "\"]";
          }
        },
        e);
  }
  std::cout << "\n";
  std::cout << "output      : " << engine.renderer().render(doc) << "\n";
}

} // namespace

int main(int argc, char **argv) {
  std::string data_dir;
  bool verbose = false;
  std::vector<std::string> words;

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--data" && i + 1 < argc) {
      data_dir = argv[++i];
    } else if (arg == "--tokens" || arg == "-v") {
      verbose = true;
    } else if (arg == "--help" || arg == "-h") {
      std::cout << "usage: okkhor [--data DIR] [--tokens] [text...]\n";
      return 0;
    } else {
      words.push_back(arg);
    }
  }

  try {
    okkhor::Engine engine =
        okkhor::Engine::from_data_dir(okkhor::find_data_dir(data_dir));

    if (!words.empty()) {
      std::string joined;
      for (std::size_t i = 0; i < words.size(); ++i) {
        if (i)
          joined += ' ';
        joined += words[i];
      }
      if (verbose)
        describe(engine, joined);
      else
        std::cout << engine.transliterate(joined) << "\n";
      return 0;
    }

    std::string line;
    while (std::getline(std::cin, line)) {
      if (verbose)
        describe(engine, line);
      else
        std::cout << engine.transliterate(line) << "\n";
    }
    return 0;
  } catch (const std::exception &e) {
    std::cerr << "okkhor: " << e.what() << "\n";
    return 1;
  }
}