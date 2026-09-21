#pragma once
// Phase 3: the only layer that knows actual Unicode.
#include <string>

#include "mapping.hpp"
#include "orthography.hpp"

namespace okkhor {

// U+09CD BENGALI SIGN VIRAMA
inline constexpr const char* kHasanta = "\xE0\xA7\x8D";
// U+200C ZERO WIDTH NON-JOINER
inline constexpr const char* kZwnj = "\xE2\x80\x8C";

class Renderer {
public:
    explicit Renderer(const Mapping& mapping) : mapping_(&mapping) {}

    std::string render(const Document& doc) const;
    std::string render(const Element& element) const;
    std::string render(const OrthographicUnit& unit) const;
    std::string render(const IndependentVowel& vowel) const;

private:
    std::string render_dependent(const DependentConsonant& dc) const;

    const Mapping* mapping_;
};

}  // namespace okkhor
