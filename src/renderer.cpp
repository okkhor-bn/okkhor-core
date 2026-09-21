#include "renderer.hpp"

#include <type_traits>

namespace okkhor {

// dcons = hosonto + bcons. A fola form, when the data supplies one, is just
// another rendering of the same dcons -- never a separate structural type.
std::string Renderer::render_dependent(const DependentConsonant& dc) const {
    const ConsonantEntry& e = mapping_->consonant(dc.value.id);
    if (!e.fola.empty()) return e.fola;
    return std::string(kHasanta) + e.base;
}

std::string Renderer::render(const OrthographicUnit& unit) const {
    std::string out;

    // base: bcons renders visibly, vcons renders as nothing.
    if (const auto* bc = std::get_if<BaseConsonant>(&unit.base))
        out += mapping_->consonant(bc->value.id).base;

    for (const DependentConsonant& dc : unit.conjuncts) out += render_dependent(dc);

    // The inherent অ is a semantic state, not a character: the dependent form
    // of `o` is the empty string, so `vn` falls out of the data.
    if (unit.vowel) out += mapping_->vowel(unit.vowel->value.id).dependent;

    if (unit.explicit_hasanta) out += kHasanta;

    for (const Accent& a : unit.accents) out += mapping_->accent(a.id).sign;

    if (unit.zwnj_after) out += kZwnj;

    return out;
}

std::string Renderer::render(const IndependentVowel& v) const {
    std::string out = mapping_->vowel(v.value.id).independent;
    for (const Accent& a : v.accents) out += mapping_->accent(a.id).sign;
    if (v.zwnj_after) out += kZwnj;
    return out;
}

std::string Renderer::render(const Element& element) const {
    return std::visit(
        [this](const auto& e) -> std::string {
            using T = std::decay_t<decltype(e)>;
            if constexpr (std::is_same_v<T, Literal>) return e.text;
            else return this->render(e);
        },
        element);
}

std::string Renderer::render(const Document& doc) const {
    std::string out;
    for (const Element& e : doc) out += render(e);
    return out;
}

}  // namespace okkhor
