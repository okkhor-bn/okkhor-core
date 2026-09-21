#pragma once
// Phase 5: tokens -> algebraic operations. The parser never touches Unicode.
#include <vector>

#include "orthography.hpp"
#include "tokenizer.hpp"

namespace okkhor {

Document parse(const std::vector<Token> &tokens);

} // namespace okkhor
