#pragma once

#include <string>
#include <vector>

#include "core/mapping.hpp"
#include "core/token.hpp"
#include "util/json.hpp"

namespace okkhor::bangla_to_latin {

class RuleEngine {
public:
    RuleEngine() = default;

    void load(
        const json::Value& doc,
        const Mapping& mapping
    );

    std::vector<Token> apply(
        const std::vector<Token>& tokens,
        const Mapping& mapping
    ) const;

private:
    struct Rule {
        std::vector<Token> pattern;
        std::vector<Token> replacement;
    };

    std::vector<Rule> rules_;
};

} // namespace okkhor::bangla_to_latin