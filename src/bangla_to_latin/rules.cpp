#include "rules.hpp"

#include <fstream>
#include <stdexcept>
#include <string>
#include <utility>

#include "util/json.hpp"

namespace okkhor::bangla_to_latin {

namespace {

Token make_token(const std::string &key, const Mapping &mapping) {

  const Rule *rule = mapping.lookup(key);

  if (!rule) {

    throw std::runtime_error("Bangla -> Latin rule references "
                             "unknown token: " +
                             key);
  }

  Token token;

  token.type = rule->type;
  token.canonical_key = rule->canonical_key;
  token.value = key;

  return token;
}



std::vector<Token> parse_token_list(const json::Value &value,
                                    const Mapping &mapping) {

  if (!value.is_array()) {

    throw std::runtime_error("rule token value must be an array");
  }

  std::vector<Token> tokens;

  for (const json::Value &item : value.as_array()) {

    if (!item.is_string()) {

      throw std::runtime_error("rule token value must contain "
                               "only strings");
    }

    const std::string key = item.as_string();

    tokens.push_back(make_token(key, mapping));
  }

  return tokens;
}

bool is_always_rule(const json::Value &when) {

  if (!when.is_object())
    return false;

  const json::Value *always = when.find("always");

  return always && always->is_bool() && always->as_bool();
}

bool token_matches(const Token &input, const Token &pattern) {

  return input.type == pattern.type &&
         input.canonical_key == pattern.canonical_key;
}

} // namespace

void RuleEngine::load(const json::Value &doc, const Mapping &mapping) {

  if (!doc.is_object()) {
    throw std::runtime_error("Invalid rulebook format: expected JSON object");
  }

  const json::Value *rules = doc.find("rules");

  if (!rules || !rules->is_object()) {

    throw std::runtime_error("rules.json: expected object 'rules'");
  }

  rules_.clear();

  for (const auto &[key, rule_list] : rules->as_object()) {

    if (!rule_list.is_array()) {

      throw std::runtime_error("rules.json: rule '" + key +
                               "' must contain an array");
    }

    for (const json::Value &rule_value : rule_list.as_array()) {

      if (!rule_value.is_object()) {

        throw std::runtime_error("rules.json: rule entry must "
                                 "be an object");
      }

      const json::Value *when = rule_value.find("when");

      const json::Value *action = rule_value.find("action");

      if (!when || !when->is_object()) {

        throw std::runtime_error("rules.json: rule '" + key +
                                 "' is missing 'when'");
      }

      if (!action || !action->is_object()) {

        throw std::runtime_error("rules.json: rule '" + key +
                                 "' is missing 'action'");
      }

      if (!is_always_rule(*when))
        continue;

      const json::Value *type = action->find("type");

      const json::Value *value = action->find("value");

      if (!type || !type->is_string()) {

        throw std::runtime_error("rules.json: rule '" + key +
                                 "' action is missing "
                                 "string 'type'");
      }

      if (!value) {

        throw std::runtime_error("rules.json: rule '" + key +
                                 "' action is missing "
                                 "'value'");
      }

      const std::string action_type = type->as_string();

      if (action_type != "token")
        continue;

      if (!value->is_array()) {

        throw std::runtime_error("rules.json: reversible token "
                                 "rule '" +
                                 key + "' must have an array value");
      }

      std::vector<Token> pattern = parse_token_list(*value, mapping);

      if (pattern.empty()) {

        throw std::runtime_error("rules.json: reversible rule '" + key +
                                 "' has an empty replacement");
      }

      std::vector<Token> replacement;
      replacement.push_back(make_token(key, mapping));

      Rule reverse_rule;

      reverse_rule.pattern = std::move(pattern);

      reverse_rule.replacement = std::move(replacement);

      rules_.push_back(std::move(reverse_rule));
    }
  }
}

std::vector<Token> RuleEngine::apply(const std::vector<Token> &tokens,
                                     const Mapping &) const {

  std::vector<Token> current = tokens;

  for (const Rule &rule : rules_) {

    if (rule.pattern.empty())
      continue;

    std::vector<Token> output;

    std::size_t i = 0;

    while (i < current.size()) {

      bool matched = false;

      if (i + rule.pattern.size() <= current.size()) {

        matched = true;

        for (std::size_t j = 0; j < rule.pattern.size(); ++j) {

          if (!token_matches(current[i + j], rule.pattern[j])) {

            matched = false;
            break;
          }
        }
      }

      if (matched) {

        output.insert(output.end(), rule.replacement.begin(),
                      rule.replacement.end());

        i += rule.pattern.size();
      } else {

        output.push_back(current[i]);

        ++i;
      }
    }

    current = std::move(output);
  }

  return current;
}

} // namespace okkhor::bangla_to_latin