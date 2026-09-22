#include <string>
#include <vector>

#include "core/mapping.hpp"
#include "core/okkhor.hpp"
#include "latin_to_bangla/rules.hpp"
#include "latin_to_bangla/tokenizer.hpp"
#include "test_util.hpp"

using namespace okkhor;
using testing::check;
using testing::check_true;

int main() {
  Mapping mapping = Mapping::load(find_data_dir());

  std::string test_json = R"({
    "rules": {
      "kkh": [
        {
          "when": { "always": true },
          "action": { "type": "token", "value": ["k", "Sh"] }
        }
      ],
      "w": [
        {
          "when": { "word-start": true },
          "action": { "type": "literal", "value": "ও" }
        },
        {
          "when": { "after-consonant": true },
          "action": { "type": "token", "value": ["b"] }
        }
      ],
      "a": [
        {
          "when": { "after-vowel": true },
          "action": { "type": "literal", "value": "আ" }
        }
      ],
      "t''": [
        {
          "when": { "always": true },
          "action": { "type": "literal", "value": "ৎ" }
        }
      ]
    }
  })";

  RuleEngine re;
  re.load_string(test_json, mapping);

  // Test 1: "kkh" -> token rewrite ("k", "Sh")
  {
    std::vector<Token> tokens = tokenize("kkh", mapping);
    std::vector<Token> rewritten = re.apply(tokens, mapping);
    check_true("kkh rewrites into 2 tokens", rewritten.size() == 2);
    if (rewritten.size() == 2) {
      check("kkh token 1", rewritten[0].latin, "k");
      check("kkh token 2", rewritten[1].latin, "Sh");
    }
  }

  // Test 2: "w" at word-start -> literal "ও"
  {
    std::vector<Token> tokens = tokenize("w", mapping);
    std::vector<Token> rewritten = re.apply(tokens, mapping);
    check_true("word-start w rewrites into 1 token", rewritten.size() == 1);
    if (!rewritten.empty()) {
      check("word-start w -> ও", rewritten[0].literal, "ও");
    }
  }

  // Test 4: "t''" -> literal "ৎ"
  {
    std::vector<Token> tokens = tokenize("t''", mapping);
    std::vector<Token> rewritten = re.apply(tokens, mapping);
    check_true("t'' rewrites to literal", rewritten.size() == 1);
    if (!rewritten.empty()) {
      check("t'' literal value", rewritten[0].literal, "ৎ");
    }
  }

  // Test 5: End-to-end engine test
  {
    Engine engine(mapping, re);
    check("bishwo test", engine.transliterate("bishwo"), "বিশ্ব");
    check("shikkha test", engine.transliterate("shikkha"), "শিক্ষা");
  }

  return testing::report("rules_tests");
}