#pragma once

#include <vector>

#include "core/orthography.hpp"
#include "core/token.hpp"

namespace okkhor::bangla_to_latin {

Document parse(const std::vector<Token> &tokens);

} // namespace okkhor::bangla_to_latin