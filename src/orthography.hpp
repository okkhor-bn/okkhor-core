#pragma once
// Phase 1 + Phase 2: semantic types and the Okkhor algebra.
//
//   H + V  -> DV          H + BC -> DC          H + অ -> VN
//   C      -> BC + H + অ  ==  BC
//   C + V  -> BC + DV
//   C + C  -> BC + DC + VN
//   C + ,, -> BC + H
//   `      -> VC
//
// Nothing here knows anything about Unicode or about Latin input.
#include <optional>
#include <variant>
#include <vector>
#include <string>

namespace okkhor {

// Identifiers into the mapping tables. Never raw Unicode.
struct Vowel     { int id = -1; };
struct Consonant { int id = -1; };
struct Accent    { int id = -1; };

inline bool operator==(Vowel a, Vowel b)         { return a.id == b.id; }
inline bool operator==(Consonant a, Consonant b) { return a.id == b.id; }
inline bool operator==(Accent a, Accent b)       { return a.id == b.id; }

struct BaseConsonant    { Consonant value; };  // visible base consonant
struct VirtualConsonant {};                    // vcons: visually-nothing base (`)

using Base = std::variant<BaseConsonant, VirtualConsonant>;

struct DependentConsonant { Consonant value; };  // dcons = hosonto + bcons
struct DependentVowel     { Vowel value; };      // dvowel = hosonto + vowel

// One orthographic cluster: a base, zero or more dependent consonants,
// an optional dependent vowel, optional accents, and terminal flags.
struct OrthographicUnit {
    Base base{VirtualConsonant{}};
    std::vector<DependentConsonant> conjuncts;
    std::optional<DependentVowel> vowel;
    std::vector<Accent> accents;
    bool explicit_hasanta = false;
    bool zwnj_after = false;
};

// A vowel with no active consonant base stands on its own.
struct IndependentVowel {
    Vowel value;
    std::vector<Accent> accents;
    bool zwnj_after = false;
};

// Anything Okkhor does not interpret: whitespace, punctuation, unknown input.
struct Literal { std::string text; };

using Element  = std::variant<OrthographicUnit, IndependentVowel, Literal>;
using Document = std::vector<Element>;

// ---- the algebra -----------------------------------------------------------

OrthographicUnit make_consonant(Consonant c);
OrthographicUnit make_virtual_consonant();

// Each of these returns false when the operation is not defined for the unit
// in its current state; the caller then closes the unit and opens a new one.
bool add_vowel(OrthographicUnit& u, Vowel v);
bool add_consonant(OrthographicUnit& u, Consonant c);
bool add_accent(OrthographicUnit& u, Accent a);
bool terminate_with_hasanta(OrthographicUnit& u);
void add_zwnj(OrthographicUnit& u);

// True while the unit can still absorb more material.
bool is_open(const OrthographicUnit& u);
bool is_virtual(const OrthographicUnit& u);

}  // namespace okkhor
