#pragma once

// Phase 4: Latin phonetic input -> semantic tokens.
// Longest match wins.

#include <string>
#include <vector>
#include "core/token.hpp"

namespace okkhor {

class Mapping;

std::vector<Token> tokenize(const std::string &input, const Mapping &mapping);

const char *token_type_name(TokenType t);

} // namespace okkhor