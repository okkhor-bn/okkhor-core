#pragma once

enum class TokenType {
  Vowel,
  Consonant,
  VirtualConsonant, // ` : the vcons base
  Accent,
  Hasanta, // ,,
  ZWNJ, // |
  ZWJ, // ~
  Whitespace,
  Punctuation,
  Unknown
};