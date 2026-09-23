#include "renderer.hpp"

#include <iostream>
#include <type_traits>

namespace okkhor::latin_to_bangla {

// dcons = hosonto + bcons.
// A fola form, when the data supplies one, is just another rendering
// of the same dcons -- never a separate structural type.
std::string Renderer::render_dependent(const DependentConsonant &dc) const {

  const ConsonantEntry *e = mapping_->consonant(dc.value.key);

  if (!e->fola.empty())
    return e->fola;

  return std::string(kHasanta) + e->base;
}

std::string Renderer::render(const OrthographicUnit &unit) const {

  std::string out;

  // base:
  //   bcons renders visibly
  //   vcons renders as nothing
  if (const auto *bc = std::get_if<BaseConsonant>(&unit.base)) {

    const ConsonantEntry *e = mapping_->consonant(bc->value.key);

    out += e->base;
  }

  for (const DependentConsonant &dc : unit.conjuncts) {

    out += render_dependent(dc);
  }

  // The inherent অ is a semantic state, not a character.
  // The dependent form of `o` is empty, so vn falls out of the data.
  if (unit.vowel) {

    const VowelEntry *e = mapping_->vowel(unit.vowel->value.key);

    out += e->dependent;
  }

  if (unit.explicit_hasanta)
    out += kHasanta;

  for (const Accent &a : unit.accents) {

    const OtherEntry *e = mapping_->other(a.key);

    out += e->value;
  }

  if (unit.zwnj_after)
    out += kZwnj;

  if (unit.zwj_after)
    out += kZwj;

  return out;
}

std::string Renderer::render(const IndependentVowel &v) const {

  const VowelEntry *e = mapping_->vowel(v.value.key);



  std::string out = e->independent;

  for (const Accent &a : v.accents) {

    const OtherEntry *accent = mapping_->other(a.key);

    out += accent->value;
  }

  if (v.zwnj_after)
    out += kZwnj;

  if (v.zwj_after)
    out += kZwj;

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

std::string Renderer::render(const Document &doc) const {

  std::string out;

  for (const Element &e : doc) {

    out += render(e);
  }

  return out;
}

} // namespace okkhor::latin_to_bangla