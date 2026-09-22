#pragma once

#include <string>
#include <vector>

#include "core/mapping.hpp"
#include "core/token.hpp"

namespace okkhor::bangla_to_latin {

class RuleEngine {
public:
    void load_file(
        const std::string& path,
        const Mapping& mapping);

    std::vector<Token> apply(
        const std::vector<Token>& tokens,
        const Mapping& mapping) const;

private:
    struct Rule {
        std::vector<Token> pattern;
        std::vector<Token> replacement;
    };

    std::vector<Rule> rules_;
};

} // namespace okkhor::bangla_to_latin
