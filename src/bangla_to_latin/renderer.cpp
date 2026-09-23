#include "renderer.hpp"

#include <type_traits>
#include <variant>

namespace okkhor::bangla_to_latin {

std::string Renderer::render(const OrthographicUnit &unit) const {

  std::string out;

  // ---------------------------------------------------------
  // Base consonant
  // ---------------------------------------------------------

  if (const auto *base = std::get_if<BaseConsonant>(&unit.base)) {

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

  for (const DependentConsonant &dc : unit.conjuncts) {

    out += dc.value.key;
  }

  // ---------------------------------------------------------
  // Dependent vowel
  // ---------------------------------------------------------

  if (unit.vowel) {

    out += unit.vowel->value.key;
  }


  if (unit.explicit_hasanta) {

    out += mapping_->controls().at("hasanta");
  }

  if (unit.zwnj_after) {
    out += mapping_->controls().at("zwnj");
  }

  if (unit.zwj_after) {
    out += "~";
  }

  for (const Accent &accent : unit.accents) {

    out += accent.key;
  }

  return out;
}

std::string Renderer::render(const IndependentVowel &vowel) const {

  std::string out;

  out += vowel.value.key;

  for (const Accent &accent : vowel.accents) {

    out += accent.key;
  }

  if (vowel.zwnj_after) {

    out += mapping_->controls().at("zwnj");
  }

  if (vowel.zwj_after) {

    out += "~";
  }

  return out;
}

std::string Renderer::render(const Element &element) const {

  return std::visit(
      [this](const auto &e) -> std::string {
        using T = std::decay_t<decltype(e)>;

        if constexpr (std::is_same_v<T, Literal>) {

          return e.text;
        } else {

          return render(e);
        }
      },
      element);
}

std::string Renderer::render(const Document &document) const {

  std::string out;

  for (const Element &element : document) {

    out += render(element);
  }

  return out;
}

} // namespace okkhor::bangla_to_latin