#include "rules.hpp"
#include "util/json.hpp"
#include <stdexcept>

namespace okkhor {

void RuleEngine::load_string(const std::string &json_str, Mapping &mapping) {
  json::Value doc = json::parse(json_str);
  const json::Value *rules_node = doc.find("rules");
  if (!rules_node || !rules_node->is_object())
    throw std::runtime_error("Invalid rulebook format: missing 'rules' object");

  rules_.clear();

  for (const auto &[key, rule_array] : rules_node->as_object()) {
    if (!rule_array.is_array())
      continue;

    // Ensure tokenizer recognizes multi-char keys (e.g., "kkh", "t''")
    if (!mapping.lookup(key)) {
      mapping.add_rule(key, Rule{TokenType::Unknown, "", key});
    }

    std::vector<ContextRule> parsed_rules;
    for (const json::Value &r : rule_array.as_array()) {
      ContextRule cr;

      if (const json::Value *when = r.find("when")) {
        const json::Value *always_v = when->find("always");
        cr.condition.always =
            always_v && always_v->is_bool() && always_v->as_bool();

        const json::Value *ws_v = when->find("word-start");
        cr.condition.word_start = ws_v && ws_v->is_bool() && ws_v->as_bool();

        const json::Value *ac_v = when->find("after-consonant");
        cr.condition.after_consonant =
            ac_v && ac_v->is_bool() && ac_v->as_bool();

        const json::Value *av_v = when->find("after-vowel");
        cr.condition.after_vowel = av_v && av_v->is_bool() && av_v->as_bool();
      }

      if (const json::Value *act = r.find("action")) {
        std::string type_str = act->string_or("type", "");
        if (type_str == "token") {
          cr.action.type = Action::Type::TokenList;
          if (const json::Value *vals = act->find("value");
              vals && vals->is_array()) {
            for (const auto &v : vals->as_array()) {
              if (v.is_string())
                cr.action.token_values.push_back(v.as_string());
            }
          }
        } else if (type_str == "literal") {
          cr.action.type = Action::Type::Literal;
          cr.action.literal_value = act->string_or("value", "");
        }
      }
      parsed_rules.push_back(cr);
    }
    rules_[key] = parsed_rules;
  }
}

void RuleEngine::load_file(const std::string &path, Mapping &mapping) {
  json::Value doc = json::parse_file(path);
  const json::Value *rules_node = doc.find("rules");
  if (!rules_node || !rules_node->is_object())
    throw std::runtime_error("Invalid rulebook format: missing 'rules' object");

  rules_.clear();

  for (const auto &[key, rule_array] : rules_node->as_object()) {
    if (!rule_array.is_array())
      continue;

    if (!mapping.lookup(key)) {
      mapping.add_rule(key, Rule{TokenType::Unknown, "", key});
    }

    std::vector<ContextRule> parsed_rules;
    for (const json::Value &r : rule_array.as_array()) {
      ContextRule cr;

      if (const json::Value *when = r.find("when")) {
        const json::Value *always_v = when->find("always");
        cr.condition.always =
            always_v && always_v->is_bool() && always_v->as_bool();

        const json::Value *ws_v = when->find("word-start");
        cr.condition.word_start = ws_v && ws_v->is_bool() && ws_v->as_bool();

        const json::Value *ac_v = when->find("after-consonant");
        cr.condition.after_consonant =
            ac_v && ac_v->is_bool() && ac_v->as_bool();

        const json::Value *av_v = when->find("after-vowel");
        cr.condition.after_vowel = av_v && av_v->is_bool() && av_v->as_bool();
      }

      if (const json::Value *act = r.find("action")) {
        std::string type_str = act->string_or("type", "");
        if (type_str == "token") {
          cr.action.type = Action::Type::TokenList;
          if (const json::Value *vals = act->find("value");
              vals && vals->is_array()) {
            for (const auto &v : vals->as_array()) {
              if (v.is_string())
                cr.action.token_values.push_back(v.as_string());
            }
          }
        } else if (type_str == "literal") {
          cr.action.type = Action::Type::Literal;
          cr.action.literal_value = act->string_or("value", "");
        }
      }
      parsed_rules.push_back(cr);
    }
    rules_[key] = parsed_rules;
  }
}

bool RuleEngine::evaluate_condition(const Condition &cond, bool is_word_start,
                                    TokenType last_type) const {
  if (cond.always)
    return true;
  if (cond.word_start && is_word_start)
    return true;
  if (cond.after_consonant && last_type == TokenType::Consonant)
    return true;
  if (cond.after_vowel && last_type == TokenType::Vowel)
    return true;
  return false;
}

std::vector<Token> RuleEngine::apply(const std::vector<Token> &tokens,
                                     const Mapping &mapping) const {
  std::vector<Token> output;

  bool is_word_start = true;
  TokenType last_type = TokenType::Unknown;

  for (const Token &t : tokens) {
    auto it = rules_.find(t.latin);
    bool matched = false;

    if (it != rules_.end()) {
      for (const ContextRule &rule : it->second) {
        if (evaluate_condition(rule.condition, is_word_start, last_type)) {
          matched = true;

          if (rule.action.type == Action::Type::Literal) {
            Token lit_token;
            lit_token.type = TokenType::Unknown;
            lit_token.literal = rule.action.literal_value;
            lit_token.latin = t.latin;
            output.push_back(lit_token);
            last_type = TokenType::Unknown;
          } else if (rule.action.type == Action::Type::TokenList) {
            for (const std::string &sub_latin : rule.action.token_values) {
              const Rule *m_rule = mapping.lookup(sub_latin);
              if (m_rule) {
                Token sub_t;
                sub_t.type = m_rule->type;
                sub_t.canonical_key = m_rule->canonical_key;
                sub_t.latin = sub_latin;
                sub_t.literal = m_rule->literal;
                output.push_back(sub_t);
                last_type = sub_t.type;
              }
            }
          }
          break;
        }
      }
    }

    if (!matched) {
      output.push_back(t);
      last_type = t.type;
    }

    is_word_start =
        (t.type == TokenType::Whitespace || t.type == TokenType::Punctuation);
  }

  return output;
}

} // namespace okkhor