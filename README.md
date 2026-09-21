# Okkhor

An Avro-like Bangla phonetic input engine built on an algebraic orthographic
model rather than on a table of word mappings.

```text
Latin phonetic input
        ↓  tokenizer      (src/tokenizer.*)      longest-match over data/*.json
semantic phonetic tokens
        ↓  parser         (src/parser.*)         calls the algebra, never Unicode
algebraic Bengali structure
        ↓  algebra        (src/orthography.*)    OrthographicUnit, ids only
internal representation
        ↓  renderer       (src/renderer.*)       the only layer that knows Unicode
Bangla UTF-8
```

## Build

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

The mapping data is copied next to the binary; the executable also honours
`--data <dir>` and the `OKKHOR_DATA` environment variable.

## Use

```sh
./build/okkhor kta            # ক্তা
./build/okkhor --tokens kta   # tokens + internal structure + output
./build/okkhor                # reads stdin line by line
```

## The algebra

The equations in the specification are the source of truth, and the code is a
direct transcription of them:

| equation                    | implementation                                                    |
| --------------------------- | ----------------------------------------------------------------- |
| `C → BC + H + অ`            | `make_consonant` (the inherent `অ` is a state, never a character) |
| `C + V → BC + DV`           | `add_vowel`                                                       |
| `C + C → BC + DC + VN`      | `add_consonant`                                                   |
| `C…C + V → BC + DC…DC + DV` | repeated `add_consonant`, then `add_vowel`                        |
| `C + ,, → BC + H`           | `terminate_with_hasanta`                                          |
| `` ` `` ` → VC`             | `make_virtual_consonant`                                          |
| `` ` `` ` + V → VC + DV`    | `make_virtual_consonant` + `add_vowel`                            |
| `H + অ → VN`                | the dependent form of `o` is the empty string in `vowels.json`    |

The algebraic operations return `false` when they are not defined for a unit in
its current state (a second vowel, a consonant after a vowel, anything after an
explicit hasanta or a ZWNJ). The parser reads that as "close this unit and open
a new one", which is the whole of its cluster-boundary logic — there are no
per-word or per-conjunct rules anywhere in the code.

Conjuncts of any length fall out of this for free: `strI` → স্ত্রী,
`kkhr` → ক্খ্র.

## Fola

Fola is not a structural type. It is `dcons = hosonto + bcons` like any other
dependent consonant; the renderer may substitute a fola rendering when the data
supplies one. In `consonants.json` that is the optional `"fola"` field, present
on `r`, `z`, `b`, `m`, `l`. In Unicode these happen to be the same code point
sequence as the default rendering (the font does the shaping), so the field is
mostly a hook for renderers or scripts where they differ.

## The `o` vowel

Deliberately, and unlike Avro:

```text
o   → অ          ko  → ক
O   → ও          kO  → কো
oi  → অই         koi → কই
```

`o` is the vowel of the inherent `অ`; its dependent form is empty, so
`hosonto + অ → vn` is data, not a special case in the code.

## Data

```text
data/vowels.json       latin → { id, ind, dep }
data/consonants.json   latin → { id, base, fola? }
data/controls.json     hasanta (",,"), zwnj ("|"), virtual ("`"), accents
data/punctuation.json  latin → replacement
```

Ids must be dense and unique within a table; the loader rejects duplicates and
duplicate Latin keys across all four files. Unknown input is never discarded —
whitespace, punctuation and anything unrecognised close the current unit and
pass through verbatim (digits are not mapped by default; add them to
`punctuation.json` if you want ০-৯).

## Deliberately absent

No spelling correction, dictionary, fuzzy matching, frequency heuristics or
prediction. Those belong in a layer above this deterministic core. Phase 7
(richer Avro-compatible phonetic rules) should be built by extending the
mapping data and the tokenizer, not by adding word rules to the parser.

## Layout

```text
CMakeLists.txt
data/      vowels.json  consonants.json  controls.json  punctuation.json
src/       json.*  mapping.*  tokenizer.*  orthography.*  parser.*  renderer.*
           okkhor.*  main.cpp
tests/     algebra_tests.cpp   (algebra + internal representation, no phonetics)
           phonetic_tests.cpp  (end-to-end Latin → Bangla)
```

`src/json.*` is a ~200-line JSON reader so the project has no third-party
dependencies.
