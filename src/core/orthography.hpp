#pragma once


#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace okkhor {
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