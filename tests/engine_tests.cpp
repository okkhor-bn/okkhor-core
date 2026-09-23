#include <string>

#include "core/okkhor.hpp"
#include "test_util.hpp"

int main() {
  const okkhor::Engine engine;

  // -------------------------------------------------------------------------
  // Basic Latin -> Bangla vowels
  // -------------------------------------------------------------------------

  testing::check("Latin -> Bangla: o",
                 engine.transliterate_latin_to_bangla("o"), "অ");

  testing::check("Latin -> Bangla: a",
                 engine.transliterate_latin_to_bangla("a"), "আ");

  testing::check("Latin -> Bangla: i",
                 engine.transliterate_latin_to_bangla("i"), "ই");

  testing::check("Latin -> Bangla: I",
                 engine.transliterate_latin_to_bangla("I"), "ঈ");

  testing::check("Latin -> Bangla: u",
                 engine.transliterate_latin_to_bangla("u"), "উ");

  testing::check("Latin -> Bangla: U",
                 engine.transliterate_latin_to_bangla("U"), "ঊ");

  testing::check("Latin -> Bangla: rri",
                 engine.transliterate_latin_to_bangla("rri"), "ঋ");

  testing::check("Latin -> Bangla: e",
                 engine.transliterate_latin_to_bangla("e"), "এ");

  testing::check("Latin -> Bangla: OI",
                 engine.transliterate_latin_to_bangla("OI"), "ঐ");

  testing::check("Latin -> Bangla: O",
                 engine.transliterate_latin_to_bangla("O"), "ও");

  testing::check("Latin -> Bangla: OU",
                 engine.transliterate_latin_to_bangla("OU"), "ঔ");

  // -------------------------------------------------------------------------
  // Basic Latin -> Bangla consonants
  // -------------------------------------------------------------------------

  testing::check("Latin -> Bangla: k",
                 engine.transliterate_latin_to_bangla("k"), "ক");

  testing::check("Latin -> Bangla: kh",
                 engine.transliterate_latin_to_bangla("kh"), "খ");

  testing::check("Latin -> Bangla: g",
                 engine.transliterate_latin_to_bangla("g"), "গ");

  testing::check("Latin -> Bangla: gh",
                 engine.transliterate_latin_to_bangla("gh"), "ঘ");

  testing::check("Latin -> Bangla: Ng",
                 engine.transliterate_latin_to_bangla("Ng"), "ঙ");

  testing::check("Latin -> Bangla: c",
                 engine.transliterate_latin_to_bangla("c"), "চ");

  testing::check("Latin -> Bangla: ch",
                 engine.transliterate_latin_to_bangla("ch"), "ছ");

  testing::check("Latin -> Bangla: j",
                 engine.transliterate_latin_to_bangla("j"), "জ");

  testing::check("Latin -> Bangla: jh",
                 engine.transliterate_latin_to_bangla("jh"), "ঝ");

  testing::check("Latin -> Bangla: NG",
                 engine.transliterate_latin_to_bangla("NG"), "ঞ");

  testing::check("Latin -> Bangla: T",
                 engine.transliterate_latin_to_bangla("T"), "ট");

  testing::check("Latin -> Bangla: Th",
                 engine.transliterate_latin_to_bangla("Th"), "ঠ");

  testing::check("Latin -> Bangla: D",
                 engine.transliterate_latin_to_bangla("D"), "ড");

  testing::check("Latin -> Bangla: Dh",
                 engine.transliterate_latin_to_bangla("Dh"), "ঢ");

  testing::check("Latin -> Bangla: N",
                 engine.transliterate_latin_to_bangla("N"), "ণ");

  testing::check("Latin -> Bangla: t",
                 engine.transliterate_latin_to_bangla("t"), "ত");

  testing::check("Latin -> Bangla: th",
                 engine.transliterate_latin_to_bangla("th"), "থ");

  testing::check("Latin -> Bangla: d",
                 engine.transliterate_latin_to_bangla("d"), "দ");

  testing::check("Latin -> Bangla: dh",
                 engine.transliterate_latin_to_bangla("dh"), "ধ");

  testing::check("Latin -> Bangla: n",
                 engine.transliterate_latin_to_bangla("n"), "ন");

  testing::check("Latin -> Bangla: p",
                 engine.transliterate_latin_to_bangla("p"), "প");

  testing::check("Latin -> Bangla: ph",
                 engine.transliterate_latin_to_bangla("ph"), "ফ");

  testing::check("Latin -> Bangla: b",
                 engine.transliterate_latin_to_bangla("b"), "ব");

  testing::check("Latin -> Bangla: bh",
                 engine.transliterate_latin_to_bangla("bh"), "ভ");

  testing::check("Latin -> Bangla: v",
                 engine.transliterate_latin_to_bangla("v"), "ভ");

  testing::check("Latin -> Bangla: m",
                 engine.transliterate_latin_to_bangla("m"), "ম");

  testing::check("Latin -> Bangla: z",
                 engine.transliterate_latin_to_bangla("z"), "য");

  testing::check("Latin -> Bangla: y",
                 engine.transliterate_latin_to_bangla("y"), "য়");

  testing::check("Latin -> Bangla: r",
                 engine.transliterate_latin_to_bangla("r"), "র");

  testing::check("Latin -> Bangla: l",
                 engine.transliterate_latin_to_bangla("l"), "ল");

  testing::check("Latin -> Bangla: sh",
                 engine.transliterate_latin_to_bangla("sh"), "শ");

  testing::check("Latin -> Bangla: Sh",
                 engine.transliterate_latin_to_bangla("Sh"), "ষ");

  testing::check("Latin -> Bangla: s",
                 engine.transliterate_latin_to_bangla("s"), "স");

  testing::check("Latin -> Bangla: h",
                 engine.transliterate_latin_to_bangla("h"), "হ");

  testing::check("Latin -> Bangla: R",
                 engine.transliterate_latin_to_bangla("R"), "ড়");

  testing::check("Latin -> Bangla: Rh",
                 engine.transliterate_latin_to_bangla("Rh"), "ঢ়");

  testing::check("Latin -> Bangla: t''",
                 engine.transliterate_latin_to_bangla("t''"), "ৎ");

  // -------------------------------------------------------------------------
  // Consonant aliases
  // -------------------------------------------------------------------------

  testing::check("Alias: f -> ph", engine.transliterate_latin_to_bangla("f"),
                 "ফ");

  testing::check("Alias: F -> ph", engine.transliterate_latin_to_bangla("F"),
                 "ফ");

  testing::check("Alias: pH -> ph", engine.transliterate_latin_to_bangla("pH"),
                 "ফ");

  testing::check("Alias: S -> sh", engine.transliterate_latin_to_bangla("S"),
                 "শ");

  testing::check("Alias: SH -> Sh", engine.transliterate_latin_to_bangla("SH"),
                 "ষ");

  testing::check("Alias: v -> bh", engine.transliterate_latin_to_bangla("v"),
                 "ভ");

  // -------------------------------------------------------------------------
  // Basic vowel attachment
  // -------------------------------------------------------------------------

  testing::check("Latin -> Bangla: ka",
                 engine.transliterate_latin_to_bangla("ka"), "কা");

  testing::check("Latin -> Bangla: ki",
                 engine.transliterate_latin_to_bangla("ki"), "কি");

  testing::check("Latin -> Bangla: kI",
                 engine.transliterate_latin_to_bangla("kI"), "কী");

  testing::check("Latin -> Bangla: ku",
                 engine.transliterate_latin_to_bangla("ku"), "কু");

  testing::check("Latin -> Bangla: kU",
                 engine.transliterate_latin_to_bangla("kU"), "কূ");

  testing::check("Latin -> Bangla: krri",
                 engine.transliterate_latin_to_bangla("krri"), "কৃ");

  testing::check("Latin -> Bangla: ke",
                 engine.transliterate_latin_to_bangla("ke"), "কে");

  testing::check("Latin -> Bangla: kOI",
                 engine.transliterate_latin_to_bangla("kOI"), "কৈ");

  testing::check("Latin -> Bangla: kO",
                 engine.transliterate_latin_to_bangla("kO"), "কো");

  testing::check("Latin -> Bangla: kOU",
                 engine.transliterate_latin_to_bangla("kOU"), "কৌ");

  // -------------------------------------------------------------------------
  // Structural controls
  // -------------------------------------------------------------------------

  testing::check("Latin -> Bangla: explicit hasanta",
                 engine.transliterate_latin_to_bangla("k.,"), "ক্");

  testing::check("Latin -> Bangla: hasanta + ZWNJ macro",
                 engine.transliterate_latin_to_bangla("k,,"), "ক্‌");

  testing::check("Latin -> Bangla: ZWNJ",
                 engine.transliterate_latin_to_bangla("k;"), "ক‌");

  testing::check("Latin -> Bangla: virtual consonant",
                 engine.transliterate_latin_to_bangla("k`"), "ক");

  // -------------------------------------------------------------------------
  // Accents / nukta
  // -------------------------------------------------------------------------

  testing::check("Latin -> Bangla: anusvara",
                 engine.transliterate_latin_to_bangla("kng"), "কং");

  testing::check("Latin -> Bangla: chandrabindu",
                 engine.transliterate_latin_to_bangla("k^"), "কঁ");

  testing::check("Latin -> Bangla: visarga",
                 engine.transliterate_latin_to_bangla("k:"), "কঃ");

  testing::check("Latin -> Bangla: nukta",
                 engine.transliterate_latin_to_bangla("k,."), "ক়");

  // -------------------------------------------------------------------------
  // Rules: kkh
  // -------------------------------------------------------------------------

  testing::check("Rule: kkh", engine.transliterate_latin_to_bangla("kkh"),
                 "ক্ষ");

  testing::check("Rule: kkh + vowel",
                 engine.transliterate_latin_to_bangla("kkha"), "ক্ষা");

                 
  // -------------------------------------------------------------------------
  // Rules: gg
  // -------------------------------------------------------------------------

  testing::check("Rule: gg", engine.transliterate_latin_to_bangla("gg"), "জ্ঞ");

  testing::check("Rule: gg + vowel",
                 engine.transliterate_latin_to_bangla("gga"), "জ্ঞা");

  // -------------------------------------------------------------------------
  // Rules: ggg
  // -------------------------------------------------------------------------

  testing::check("Rule: ggg", engine.transliterate_latin_to_bangla("ggg"),
                 "গ্গ");

  testing::check("Rule: ggg + vowel",
                 engine.transliterate_latin_to_bangla("ggga"), "গ্গা");

  // -------------------------------------------------------------------------
  // Rules: ng
  // -------------------------------------------------------------------------

  testing::check("Rule: ng before vowel",
                 engine.transliterate_latin_to_bangla("nga"), "ঙা");

  testing::check("Rule: ng before consonant",
                 engine.transliterate_latin_to_bangla("ngk"), "ংক");

  // -------------------------------------------------------------------------
  // Rules: w
  // -------------------------------------------------------------------------

  testing::check("Rule: w at word start",
                 engine.transliterate_latin_to_bangla("w"), "ও");

  testing::check("Rule: w after consonant",
                 engine.transliterate_latin_to_bangla("kw"), "ক্ব");

  // -------------------------------------------------------------------------
  // Rules: ,,
  // -------------------------------------------------------------------------

  testing::check("Rule: ,,", engine.transliterate_latin_to_bangla(",,"),
                 "্‌");

  testing::check("Rule: k,,", engine.transliterate_latin_to_bangla("k,,"),
                 "ক্‌");

  // -------------------------------------------------------------------------
  // Rules: ;;
  // -------------------------------------------------------------------------

  testing::check("Rule: ;;", engine.transliterate_latin_to_bangla(";;"), ";");

  testing::check("Rule: k;;", engine.transliterate_latin_to_bangla("k;;"),
                 "ক;");

  // -------------------------------------------------------------------------
  // Rules: rya
  // -------------------------------------------------------------------------

  testing::check("Rule: rya", engine.transliterate_latin_to_bangla("rya"),
                 "র‍্যা");

  // -------------------------------------------------------------------------
  // Rules: ..
  // -------------------------------------------------------------------------

  testing::check("Rule: ..", engine.transliterate_latin_to_bangla(".."), ".");

  testing::check("Rule: .. after word",
                 engine.transliterate_latin_to_bangla("ka.."), "কা.");

  // -------------------------------------------------------------------------
  // Rules: oo
  // -------------------------------------------------------------------------

  testing::check("Rule: oo", engine.transliterate_latin_to_bangla("oo"), "ঃ");

  // -------------------------------------------------------------------------
  // Punctuation
  // -------------------------------------------------------------------------

  testing::check("Punctuation: .", engine.transliterate_latin_to_bangla("."),
                 "।");

  testing::check("Punctuation: $", engine.transliterate_latin_to_bangla("$"),
                 "৳");

  testing::check("Punctuation: h'", engine.transliterate_latin_to_bangla("h'"),
                 "ঽ");

  // -------------------------------------------------------------------------
  // Bengali digits
  // -------------------------------------------------------------------------

  testing::check("Digit: 0", engine.transliterate_latin_to_bangla("0"), "০");

  testing::check("Digit: 1", engine.transliterate_latin_to_bangla("1"), "১");

  testing::check("Digit: 2", engine.transliterate_latin_to_bangla("2"), "২");

  testing::check("Digit: 3", engine.transliterate_latin_to_bangla("3"), "৩");

  testing::check("Digit: 4", engine.transliterate_latin_to_bangla("4"), "৪");

  testing::check("Digit: 5", engine.transliterate_latin_to_bangla("5"), "৫");

  testing::check("Digit: 6", engine.transliterate_latin_to_bangla("6"), "৬");

  testing::check("Digit: 7", engine.transliterate_latin_to_bangla("7"), "৭");

  testing::check("Digit: 8", engine.transliterate_latin_to_bangla("8"), "৮");

  testing::check("Digit: 9", engine.transliterate_latin_to_bangla("9"), "৯");

  // -------------------------------------------------------------------------
  // Bangla -> Latin: structural controls
  // -------------------------------------------------------------------------

  testing::check("Bangla -> Latin: explicit hasanta",
                 engine.transliterate_bangla_to_latin("ক্"), "k.,");

  testing::check("Bangla -> Latin: hasanta + ZWNJ",
                 engine.transliterate_bangla_to_latin("ক্‌"), "k,,");

  testing::check("Bangla -> Latin: ZWNJ",
                 engine.transliterate_bangla_to_latin("ক‌"), "k;");

  // -------------------------------------------------------------------------
  // Bangla -> Latin: basic vowels
  // -------------------------------------------------------------------------

  testing::check("Bangla -> Latin: independent অ",
                 engine.transliterate_bangla_to_latin("অ"), "o");

  testing::check("Bangla -> Latin: independent আ",
                 engine.transliterate_bangla_to_latin("আ"), "a");

  testing::check("Bangla -> Latin: independent ই",
                 engine.transliterate_bangla_to_latin("ই"), "i");

  testing::check("Bangla -> Latin: independent ঈ",
                 engine.transliterate_bangla_to_latin("ঈ"), "I");

  testing::check("Bangla -> Latin: independent উ",
                 engine.transliterate_bangla_to_latin("উ"), "u");

  testing::check("Bangla -> Latin: independent ঊ",
                 engine.transliterate_bangla_to_latin("ঊ"), "U");

  testing::check("Bangla -> Latin: independent ঋ",
                 engine.transliterate_bangla_to_latin("ঋ"), "rri");

  testing::check("Bangla -> Latin: independent এ",
                 engine.transliterate_bangla_to_latin("এ"), "e");

  testing::check("Bangla -> Latin: independent ঐ",
                 engine.transliterate_bangla_to_latin("ঐ"), "OI");

  testing::check("Bangla -> Latin: independent ও",
                 engine.transliterate_bangla_to_latin("ও"), "O");

  testing::check("Bangla -> Latin: independent ঔ",
                 engine.transliterate_bangla_to_latin("ঔ"), "OU");

  // -------------------------------------------------------------------------
  // Bangla -> Latin: dependent vowels
  // -------------------------------------------------------------------------

  testing::check("Bangla -> Latin: কা",
                 engine.transliterate_bangla_to_latin("কা"), "ka");

  testing::check("Bangla -> Latin: কি",
                 engine.transliterate_bangla_to_latin("কি"), "ki");

  testing::check("Bangla -> Latin: কী",
                 engine.transliterate_bangla_to_latin("কী"), "kI");

  testing::check("Bangla -> Latin: কু",
                 engine.transliterate_bangla_to_latin("কু"), "ku");

  testing::check("Bangla -> Latin: কূ",
                 engine.transliterate_bangla_to_latin("কূ"), "kU");

  testing::check("Bangla -> Latin: কৃ",
                 engine.transliterate_bangla_to_latin("কৃ"), "krri");

  testing::check("Bangla -> Latin: কে",
                 engine.transliterate_bangla_to_latin("কে"), "ke");

  testing::check("Bangla -> Latin: কৈ",
                 engine.transliterate_bangla_to_latin("কৈ"), "kOI");

  testing::check("Bangla -> Latin: কো",
                 engine.transliterate_bangla_to_latin("কো"), "kO");

  testing::check("Bangla -> Latin: কৌ",
                 engine.transliterate_bangla_to_latin("কৌ"), "kOU");

  // -------------------------------------------------------------------------
  // Bangla -> Latin: accents
  // -------------------------------------------------------------------------

  testing::check("Bangla -> Latin: anusvara",
                 engine.transliterate_bangla_to_latin("কং"), "kng");

  testing::check("Bangla -> Latin: chandrabindu",
                 engine.transliterate_bangla_to_latin("কঁ"), "k^");

  testing::check("Bangla -> Latin: visarga",
                 engine.transliterate_bangla_to_latin("কঃ"), "koo");

  testing::check("Bangla -> Latin: nukta",
                 engine.transliterate_bangla_to_latin("ক়"), "k,.");

  // -------------------------------------------------------------------------
  // Round trip: simple vowels
  // -------------------------------------------------------------------------

  testing::check("Round trip: a",
                 engine.transliterate_bangla_to_latin(
                     engine.transliterate_latin_to_bangla("a")),
                 "a");

  testing::check("Round trip: i",
                 engine.transliterate_bangla_to_latin(
                     engine.transliterate_latin_to_bangla("i")),
                 "i");

  testing::check("Round trip: I",
                 engine.transliterate_bangla_to_latin(
                     engine.transliterate_latin_to_bangla("I")),
                 "I");

  testing::check("Round trip: u",
                 engine.transliterate_bangla_to_latin(
                     engine.transliterate_latin_to_bangla("u")),
                 "u");

  testing::check("Round trip: U",
                 engine.transliterate_bangla_to_latin(
                     engine.transliterate_latin_to_bangla("U")),
                 "U");

  testing::check("Round trip: rri",
                 engine.transliterate_bangla_to_latin(
                     engine.transliterate_latin_to_bangla("rri")),
                 "rri");

  testing::check("Round trip: e",
                 engine.transliterate_bangla_to_latin(
                     engine.transliterate_latin_to_bangla("e")),
                 "e");

  testing::check("Round trip: OI",
                 engine.transliterate_bangla_to_latin(
                     engine.transliterate_latin_to_bangla("OI")),
                 "OI");

  testing::check("Round trip: O",
                 engine.transliterate_bangla_to_latin(
                     engine.transliterate_latin_to_bangla("O")),
                 "O");

  testing::check("Round trip: OU",
                 engine.transliterate_bangla_to_latin(
                     engine.transliterate_latin_to_bangla("OU")),
                 "OU");

  // -------------------------------------------------------------------------
  // Round trip: structural controls
  // -------------------------------------------------------------------------

  testing::check("Round trip: hasanta",
                 engine.transliterate_latin_to_bangla(
                     engine.transliterate_bangla_to_latin("ক্")),
                 "ক্");

  testing::check("Round trip: hasanta + ZWNJ",
                 engine.transliterate_latin_to_bangla(
                     engine.transliterate_bangla_to_latin("ক্‌")),
                 "ক্‌");

  testing::check("Round trip: consonant + hasanta + consonant",
                 engine.transliterate_latin_to_bangla(
                     engine.transliterate_bangla_to_latin("ক্‌র")),
                 "ক্‌র");

  testing::check(
      "Round trip: multiple ZWNJ-separated consonants",
      engine.transliterate_latin_to_bangla(
          engine.transliterate_bangla_to_latin("ক্‌ন্‌ম")),
      "ক্‌ন্‌ম");

  // -------------------------------------------------------------------------
  // Round trip: conjuncts
  // -------------------------------------------------------------------------

  testing::check("Round trip: simple conjunct",
                 engine.transliterate_latin_to_bangla(
                     engine.transliterate_bangla_to_latin("ক্ন")),
                 "ক্ন");

  testing::check("Round trip: conjunct + vowel",
                 engine.transliterate_latin_to_bangla(
                     engine.transliterate_bangla_to_latin("ক্না")),
                 "ক্না");

  testing::check("Round trip: three-consonant conjunct",
                 engine.transliterate_latin_to_bangla(
                     engine.transliterate_bangla_to_latin("ক্ন্ত")),
                 "ক্ন্ত");

  testing::check("Round trip: ক্ষ",
                 engine.transliterate_latin_to_bangla(
                     engine.transliterate_bangla_to_latin("ক্ষ")),
                 "ক্ষ");

  testing::check("Round trip: ক্ষ + vowel",
                 engine.transliterate_latin_to_bangla(
                     engine.transliterate_bangla_to_latin("ক্ষি")),
                 "ক্ষি");

  testing::check(
      "Round trip: conjunct + macro",
      engine.transliterate_latin_to_bangla(
          engine.transliterate_bangla_to_latin("ক্ষ্‌র")),
      "ক্ষ্‌র");

  // -------------------------------------------------------------------------
  // Round trip: accents
  // -------------------------------------------------------------------------

  testing::check("Round trip: chandrabindu",
                 engine.transliterate_latin_to_bangla(
                     engine.transliterate_bangla_to_latin("কাঁ")),
                 "কাঁ");

  testing::check("Round trip: anusvara",
                 engine.transliterate_latin_to_bangla(
                     engine.transliterate_bangla_to_latin("বং")),
                 "বং");

  testing::check("Round trip: visarga",
                 engine.transliterate_latin_to_bangla(
                     engine.transliterate_bangla_to_latin("কঃ")),
                 "কঃ");

  testing::check("Round trip: nukta",
                 engine.transliterate_latin_to_bangla(
                     engine.transliterate_bangla_to_latin("ক়")),
                 "ক়");

  testing::check("Round trip: conjunct + chandrabindu",
                 engine.transliterate_latin_to_bangla(
                     engine.transliterate_bangla_to_latin("ক্ষাঁ")),
                 "ক্ষাঁ");

//   // -------------------------------------------------------------------------
//   // Round trip: rules
//   // -------------------------------------------------------------------------

//   testing::check("Round trip: kkh",
//                  engine.transliterate_bangla_to_latin(
//                      engine.transliterate_latin_to_bangla("kkh")),
//                  "kSh");

//   testing::check("Round trip: gg",
//                  engine.transliterate_bangla_to_latin(
//                      engine.transliterate_latin_to_bangla("gg")),
//                  "gg");

//   testing::check("Round trip: ggg",
//                  engine.transliterate_bangla_to_latin(
//                      engine.transliterate_latin_to_bangla("ggg")),
//                  "ggg");

//   testing::check("Round trip: w",
//                  engine.transliterate_bangla_to_latin(
//                      engine.transliterate_latin_to_bangla("w")),
//                  "w");

//   testing::check("Round trip: ,,,",
//                  engine.transliterate_bangla_to_latin(
//                      engine.transliterate_latin_to_bangla(",,")),
//                  ",,");

//   testing::check("Round trip: ;;",
//                  engine.transliterate_bangla_to_latin(
//                      engine.transliterate_latin_to_bangla(";;")),
//                  ";;");

  // -------------------------------------------------------------------------
  // Round trip: common words
  // -------------------------------------------------------------------------

  testing::check("Round trip: amar",
                 engine.transliterate_bangla_to_latin(
                     engine.transliterate_latin_to_bangla("amar")),
                 "amar");

  testing::check("Round trip: বাংলা",
                 engine.transliterate_latin_to_bangla(
                     engine.transliterate_bangla_to_latin("বাংলা")),
                 "বাংলা");

  testing::check("Round trip: মন্ত্র",
                 engine.transliterate_latin_to_bangla(
                     engine.transliterate_bangla_to_latin("মন্ত্র")),
                 "মন্ত্র");

  // -------------------------------------------------------------------------
  // Longer real-world words
  // -------------------------------------------------------------------------

  const std::string bangla_words[] = {
      "বাংলাদেশ",  "রাজশাহী",   "পদার্থবিজ্ঞান", "কম্পিউটার",   "বিজ্ঞান",
      "প্রযুক্তি",    "সফটওয়্যার",  "হার্ডওয়্যার",   "গণিত",       "বিশ্ববিদ্যালয়",
      "প্রোগ্রামিং", "অ্যালগরিদম", "সিমুলেশন",     "অপ্টিমাইজেশন"};

  for (const std::string &word : bangla_words) {
    testing::check("Round trip: " + word,
                   engine.transliterate_latin_to_bangla(
                       engine.transliterate_bangla_to_latin(word)),
                   word);
  }

  // -------------------------------------------------------------------------
  // Sentences / whitespace / punctuation
  // -------------------------------------------------------------------------

  testing::check("Round trip: whitespace",
                 engine.transliterate_latin_to_bangla(
                     engine.transliterate_bangla_to_latin("বাংলা ভাষা")),
                 "বাংলা ভাষা");

  testing::check("Round trip: multiple spaces",
                 engine.transliterate_latin_to_bangla(
                     engine.transliterate_bangla_to_latin("বাংলা  ভাষা")),
                 "বাংলা  ভাষা");

  testing::check("Round trip: punctuation",
                 engine.transliterate_latin_to_bangla(
                     engine.transliterate_bangla_to_latin("বাংলা।")),
                 "বাংলা।");

  testing::check("Round trip: currency",
                 engine.transliterate_latin_to_bangla(
                     engine.transliterate_bangla_to_latin("৳")),
                 "৳");

  testing::check("Round trip: Bengali digits",
                 engine.transliterate_latin_to_bangla(
                     engine.transliterate_bangla_to_latin("১২৩৪৫৬৭৮৯০")),
                 "১২৩৪৫৬৭৮৯০");

  // -------------------------------------------------------------------------
  // Mixed content
  // -------------------------------------------------------------------------

  testing::check(
      "Round trip: Bangla sentence",
      engine.transliterate_latin_to_bangla(engine.transliterate_bangla_to_latin(
          "আমি বাংলা ভাষায় প্রোগ্রামিং করি।")),
      "আমি বাংলা ভাষায় প্রোগ্রামিং করি।");

  testing::check("Round trip: Bangla + digits",
                 engine.transliterate_latin_to_bangla(
                     engine.transliterate_bangla_to_latin("বাংলাদেশ ২০২৬")),
                 "বাংলাদেশ ২০২৬");

  // -------------------------------------------------------------------------
  // Original hard cases
  // -------------------------------------------------------------------------

  const std::string hard = "ক্ষ্ম্যজ্ঞ্যন্ত্‌র্য্যশ্চ্ছ্বঙ্ক্ষ্ট্র্দ্ধ্ব্যাঁক্‌ন্‌ম্গ্ন্ত্র্যৌক্ক্হৃঙ্"
                           "ঘ"
                           "্"
                           "ন"
                           "্"
                           "য";

  const std::string soft = "ক্ক্হৃ";

  testing::check("Round trip: hard",
                 engine.transliterate_latin_to_bangla(
                     engine.transliterate_bangla_to_latin(hard)),
                 hard);

  testing::check("Round trip: soft",
                 engine.transliterate_latin_to_bangla(
                     engine.transliterate_bangla_to_latin(soft)),
                 soft);

  testing::check("Round trip: hard + soft",
                 engine.transliterate_latin_to_bangla(
                     engine.transliterate_bangla_to_latin(hard + soft)),
                 hard + soft);

  // -------------------------------------------------------------------------
  // Final report
  // -------------------------------------------------------------------------

  return testing::report("engine");
}