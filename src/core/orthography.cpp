#include "orthography.hpp"

namespace okkhor {

OrthographicUnit make_consonant(Consonant c) {
  OrthographicUnit u;
  u.base = BaseConsonant{c};
  return u; // C = BC + H + অ = BC + VN
}

OrthographicUnit make_virtual_consonant() {
  OrthographicUnit u;
  u.base = VirtualConsonant{};
  return u; // ` = VC
}

bool is_virtual(const OrthographicUnit &u) {
  return std::holds_alternative<VirtualConsonant>(u.base);
}

// A unit is sealed once it carries a terminal marker.
static bool sealed(const OrthographicUnit &u) {
  return u.explicit_hasanta || u.zwnj_after;
}

bool is_open(const OrthographicUnit &u) {
  return !sealed(u) && !u.vowel && u.accents.empty();
}

bool add_vowel(OrthographicUnit &u, Vowel v) {
  if (!is_open(u))
    return false;
  u.vowel = DependentVowel{v}; // BC + DC... + DV
  return true;
}

bool add_consonant(OrthographicUnit &u, Consonant c) {
  if (!is_open(u))
    return false;
  u.conjuncts.push_back(DependentConsonant{c}); // BC + DC + VN
  return true;
}

bool add_accent(OrthographicUnit &u, Accent a) {
  if (sealed(u))
    return false;
  u.accents.push_back(a);
  return true;
}

bool terminate_with_hasanta(OrthographicUnit &u) {
  if (sealed(u) || u.vowel)
    return false;
  u.explicit_hasanta = true; // BC + DC... + H
  return true;
}

void add_hasanta(OrthographicUnit &u) { u.explicit_hasanta = true; }

void add_zwnj(OrthographicUnit &u) { u.zwnj_after = true; }
void add_zwj(OrthographicUnit &u) { u.zwj_after = true; }

} // namespace okkhor
