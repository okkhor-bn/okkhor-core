#pragma once

namespace okkhor::default_data {

inline constexpr const char* vowels = R"OK_VOW(
{
  "o": { "ind": "অ", "dep": "" },
  "a": { "ind": "আ", "dep": "া" },
  "i": { "ind": "ই", "dep": "ি" },
  "I": { "ind": "ঈ", "dep": "ী" },
  "u": { "ind": "উ", "dep": "ু" },
  "U": { "ind": "ঊ", "dep": "ূ" },
  "rri": { "ind": "ঋ", "dep": "ৃ" },
  "e": { "ind": "এ", "dep": "ে" },
  "OI": { "ind": "ঐ", "dep": "ৈ" },
  "O": { "ind": "ও", "dep": "ো" },
  "OU": { "ind": "ঔ", "dep": "ৌ" }
}

)OK_VOW";

inline constexpr const char* consonants = R"OK_CON(
{
  "k": { "base": "ক" },
  "kh": { "base": "খ" },
  "g": { "base": "গ" },
  "gh": { "base": "ঘ" },
  "Ng": { "base": "ঙ" },
  "c": { "base": "চ" },
  "ch": { "base": "ছ" },
  "j": { "base": "জ" },
  "jh": { "base": "ঝ" },
  "NG": { "base": "ঞ" },
  "T": { "base": "ট" },
  "Th": { "base": "ঠ" },
  "D": { "base": "ড" },
  "Dh": { "base": "ঢ" },
  "N": { "base": "ণ" },
  "t": { "base": "ত" },
  "th": { "base": "থ" },
  "d": { "base": "দ" },
  "dh": { "base": "ধ" },
  "n": { "base": "ন" },
  "p": { "base": "প" },
  "ph": { "base": "ফ", "aliases": ["f", "F", "pH"] },
  "b": { "base": "ব" },
  "bh": { "base": "ভ" },
  "v": { "base": "ভ" },
  "m": { "base": "ম" },
  "z": { "base": "য" },
  "y": { "base": "য়" },
  "r": { "base": "র" },
  "l": { "base": "ল" },
  "sh": { "base": "শ", "aliases": ["S"] },
  "Sh": { "base": "ষ", "aliases": ["SH"] },
  "s": { "base": "স" },
  "h": { "base": "হ" },
  "R": { "base": "ড়" },
  "Rh": { "base": "ঢ়" },
  "t''": { "base": "ৎ" }
}

)OK_CON";

inline constexpr const char* controls = R"OK_CTL(
{
  "hasanta": { ".,": "্" },
  "zwnj": { ";": "\u200c" },
  "zwj": { ",;": "\u200d" },
  "virtual": { "`": "" },
  "accents": {
    "ng": { "sign": "ং" },
    "^": { "sign": "ঁ" },
    ":": { "sign": "ঃ" },
    ",.": { "sign": "়" }
  }
}
)OK_CTL";

inline constexpr const char* punctuation = R"OK_PUN(
{
  ".": "।",
  "..": "॥",
  "$": "৳",
  "h'": "ঽ",
  "'": "'",
  "0": "০",
  "1": "১",
  "2": "২",
  "3": "৩",
  "4": "৪",
  "5": "৫",
  "6": "৬",
  "7": "৭",
  "8": "৮",
  "9": "৯"
}

)OK_PUN";

inline constexpr const char* rules = R"OK_RUL(
{
  "rules": {
    "kkh": [
      {
        "reversible": true,
        "when": {
          "always": true
        },
        "action": {
          "type": "token",
          "value": ["k", "Sh"]
        }
      }
    ],
    "gg": [
      {
        "reversible": true,
        "when": {
          "always": true
        },
        "action": {
          "type": "token",
          "value": ["j", "NG"]
        }
      }
    ],

    "ggg": [
      {
        "reversible": true,
        "when": {
          "always": true
        },
        "action": {
          "type": "token",
          "value": ["g", ".,", "g"]
        }
      }
    ],

    "ng": [
      {
        "reversible": false,
        "when": {
          "before-vowel": true
        },
        "action": {
          "type": "token",
          "value": ["Ng"]
        }
      },
      {
        "reversible": false,
        "when": {
          "word-start": true
        },
        "action": {
          "type": "token",
          "value": ["Ng", ";"]
        }
      }
    ],

    "w": [
      {
        "reversible": false,
        "when": {
          "word-start": true
        },
        "action": {
          "type": "literal",
          "value": "ও"
        }
      },
      {
        "reversible": false,
        "when": {
          "after-consonant": true
        },
        "action": {
          "type": "token",
          "value": ["b"]
        }
      }
    ],

    ",,": [
      {
        "reversible": true,
        "desc": "Non joining hasanta",
        "when": {
          "always": true
        },
        "action": {
          "type": "token",
          "value": [".,", ";"]
        }
      }
    ],
    ";;": [
      {
        "desc": "; bypassing zwnj",
        "when": {
          "always": true
        },
        "action": {
          "type": "literal",
          "value": ";"
        }
      }
    ],

    "rya": [
      {
        "reversible": true,
        "desc": "allow র‍্যা to render properly as r+zwj+z+a",
        "when": {
          "always": true
        },
        "action": {
          "type": "token",
          "value": ["r", ",;", "`", "z", "a"]
        }
      }
    ],
    "..": [
      {
        "reversible": false,
        "desc": "Dot bypassing dari",
        "when": {
          "always": true
        },
        "action": {
          "type": "literal",
          "value": "."
        }
      }
    ],
    "z,.": [
      {
        "desc": "য+় to য়",
        "reversible": true,
        "rmap": true,
        "when": {
          "always": true
        },
        "action": {
          "type": "token",
          "value": ["y"]
        }
      }
    ]
  }
}

)OK_RUL";

}
