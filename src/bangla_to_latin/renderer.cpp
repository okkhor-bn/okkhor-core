#include "renderer.hpp"

#include <type_traits>
#include <variant>

namespace okkhor::bangla_to_latin {

std::string Renderer::render(
    const OrthographicUnit& unit) const {

    std::string out;

    // ---------------------------------------------------------
    // Base consonant
    // ---------------------------------------------------------

    if (const auto* base =
            std::get_if<BaseConsonant>(&unit.base)) {

        out += base->value.key;
    }

    // ---------------------------------------------------------
    // Structural conjuncts
    //
    // The hasanta between consonants is structural and therefore
    // is NOT emitted in the Latin representation.
    //
    //     ক্ন -> kn
    //     ক্ম -> km
    //     ক্ষ -> kSh
    // ---------------------------------------------------------

    for (const DependentConsonant& dc :
         unit.conjuncts) {

        out += dc.value.key;
    }

    // ---------------------------------------------------------
    // Dependent vowel
    // ---------------------------------------------------------

    if (unit.vowel) {

        out += unit.vowel->value.key;
    }

    // ---------------------------------------------------------
    // Explicit hasanta
    //
    // Hasanta is always represented by ",.".
    //
    //     ক্ -> k,.
    //
    // Never emit ",,".
    // ---------------------------------------------------------

    if (unit.explicit_hasanta) {

        out += ",.";
    }

    // ---------------------------------------------------------
    // ZWNJ
    //
    // ZWNJ is always represented by "|".
    //
    // Therefore:
    //
    //     ক‌  -> k|
    //     ক্‌ -> k,.|
    //
    // Never combine them into ",,".
    // ---------------------------------------------------------

    if (unit.zwnj_after) {

        out += "|";
    }

    // ---------------------------------------------------------
    // ZWJ
    // ---------------------------------------------------------

    if (unit.zwj_after) {

        out += "~";
    }

    // ---------------------------------------------------------
    // Accents
    // ---------------------------------------------------------

    for (const Accent& accent :
         unit.accents) {

        out += accent.key;
    }

    return out;
}

std::string Renderer::render(
    const IndependentVowel& vowel) const {

    std::string out;

    // ---------------------------------------------------------
    // Independent vowel
    // ---------------------------------------------------------

    out += vowel.value.key;

    // ---------------------------------------------------------
    // Accents
    // ---------------------------------------------------------

    for (const Accent& accent :
         vowel.accents) {

        out += accent.key;
    }

    // ---------------------------------------------------------
    // ZWNJ / ZWJ
    // ---------------------------------------------------------

    if (vowel.zwnj_after) {

        out += "|";
    }

    if (vowel.zwj_after) {

        out += "~";
    }

    return out;
}

std::string Renderer::render(
    const Element& element) const {

    return std::visit(
        [this](const auto& e) -> std::string {

            using T = std::decay_t<decltype(e)>;

            if constexpr (
                std::is_same_v<T, Literal>) {

                return e.text;
            }
            else {

                return render(e);
            }
        },
        element
    );
}

std::string Renderer::render(
    const Document& document) const {

    std::string out;

    for (const Element& element :
         document) {

        out += render(element);
    }

    return out;
}

} // namespace okkhor::bangla_to_latin