#pragma once

// Phase 1 + Phase 2: semantic types and the Okkhor algebra.
//
//   H + V  -> DV
//   H + BC -> DC
//   H + অ  -> VN
//
//   C      -> BC + H + অ == BC
//   C + V  -> BC + DV
//   C + C  -> BC + DC + VN
//   C + ,, -> BC + H
//   `      -> VC
//
// Nothing here knows anything about Unicode or about Latin input.

#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace okkhor {

// Canonical keys into the mapping tables.
// Never raw Unicode.
//
// Examples:
//   "k"  -> ক
//   "sh" -> শ
//   "a"  -> আ
//
// Aliases are resolved by Mapping before they reach this layer.
struct Vowel {
  std::string key;
};

struct Consonant {
  std::string key;
};

struct Accent {
  std::string key;
};

inline bool operator==(const Vowel &a, const Vowel &b) {

  return a.key == b.key;
}

inline bool operator==(const Consonant &a, const Consonant &b) {

  return a.key == b.key;
}

inline bool operator==(const Accent &a, const Accent &b) {

  return a.key == b.key;
}

struct BaseConsonant {
  Consonant value;

}; // visible base consonant

struct VirtualConsonant {}; // vcons: visually-nothing base (`)

using Base = std::variant<BaseConsonant, VirtualConsonant>;

struct DependentConsonant {
  Consonant value;

}; // dcons = hosonto + bcons

struct DependentVowel {
  Vowel value;

}; // dvowel = hosonto + vowel

// One orthographic cluster:
//
//   base
//   zero or more dependent consonants
//   optional dependent vowel
//   optional accents
//   terminal flags
//
struct OrthographicUnit {

  Base base{VirtualConsonant{}};

  std::vector<DependentConsonant> conjuncts;

  std::optional<DependentVowel> vowel;

  std::vector<Accent> accents;

  bool explicit_hasanta = false;

  bool zwnj_after = false;

  bool zwj_after = false;
};

// A vowel with no active consonant base stands on its own.
struct IndependentVowel {

  Vowel value;

  std::vector<Accent> accents;

  bool zwnj_after = false;

  bool zwj_after = false;
};

// Anything Okkhor does not interpret:
// whitespace, punctuation, unknown input.
struct Literal {
  std::string text;
};

using Element = std::variant<OrthographicUnit, IndependentVowel, Literal>;

using Document = std::vector<Element>;

// ---- the algebra -----------------------------------------------------------

OrthographicUnit make_consonant(Consonant c);

OrthographicUnit make_virtual_consonant();

// Each of these returns false when the operation is not defined for the unit
// in its current state; the caller then closes the unit and opens a new one.

bool add_vowel(OrthographicUnit &u, Vowel v);

bool add_consonant(OrthographicUnit &u, Consonant c);

bool add_accent(OrthographicUnit &u, Accent a);

bool terminate_with_hasanta(OrthographicUnit &u);

void add_zwnj(OrthographicUnit &u);

void add_zwj(OrthographicUnit &u);

// True while the unit can still absorb more material.
bool is_open(const OrthographicUnit &u);

bool is_virtual(const OrthographicUnit &u);

} // namespace okkhor