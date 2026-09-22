#include <string>

#include "core/okkhor.hpp"
#include "test_util.hpp"

int main() {
 

  const std::string data_dir = okkhor::find_data_dir();

  const okkhor::Engine engine = okkhor::Engine::from_data_dir(data_dir);

  // -------------------------------------------------------------------------
  // Latin -> Bangla: explicit structural controls
  // -------------------------------------------------------------------------

  testing::check("Latin -> Bangla: explicit hasanta",
                 engine.transliterate_latin_to_bangla("k,."), "ক্");

  testing::check("Latin -> Bangla: hasanta + ZWNJ macro",
                 engine.transliterate_latin_to_bangla("k,,"), "ক্‌");

  testing::check("Latin -> Bangla: ZWNJ",
                 engine.transliterate_latin_to_bangla("k|"), "ক\u200C");

  testing::check("Latin -> Bangla: ZWJ",
                 engine.transliterate_latin_to_bangla("k~"), "ক~");

  // -------------------------------------------------------------------------
  // Bangla -> Latin: explicit structural controls
  // -------------------------------------------------------------------------

  testing::check("Bangla -> Latin: explicit hasanta",
                 engine.transliterate_bangla_to_latin("ক্"), "k,.");

  testing::check("Bangla -> Latin: hasanta + ZWNJ macro",
                 engine.transliterate_bangla_to_latin("ক্‌"), "k,,");

  testing::check("Bangla -> Latin: ZWNJ",
                 engine.transliterate_bangla_to_latin("ক\u200C"), "k|");

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
      "Round trip: two macro-separated consonants",
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

  testing::check("Round trip: conjunct with vowel",
                 engine.transliterate_latin_to_bangla(
                     engine.transliterate_bangla_to_latin("ক্না")),
                 "ক্না");

  testing::check("Round trip: three-consonant conjunct",
                 engine.transliterate_latin_to_bangla(
                     engine.transliterate_bangla_to_latin("ক্ন্ত")),
                 "ক্ন্ত");

  testing::check(
      "Round trip: conjunct followed by macro",
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

  testing::check("Round trip: conjunct + chandrabindu",
                 engine.transliterate_latin_to_bangla(
                     engine.transliterate_bangla_to_latin("ক্ষাঁ")),
                 "ক্ষাঁ");

  // -------------------------------------------------------------------------
  // Round trip: combinations
  // -------------------------------------------------------------------------

  testing::check("Round trip: kO",
                 engine.transliterate_bangla_to_latin(
                     engine.transliterate_latin_to_bangla("kO")),
                 "kO");

  testing::check("Round trip: kOI",
                 engine.transliterate_bangla_to_latin(
                     engine.transliterate_latin_to_bangla("kOI")),
                 "kOI");

  testing::check("Round trip: kOU",
                 engine.transliterate_bangla_to_latin(
                     engine.transliterate_latin_to_bangla("kOU")),
                 "kOU");

  testing::check("Round trip: conjunct + vowel",
                 engine.transliterate_bangla_to_latin(
                     engine.transliterate_latin_to_bangla("knta")),
                 "knta");

  testing::check("Round trip: conjunct + macro",
                 engine.transliterate_bangla_to_latin(
                     engine.transliterate_latin_to_bangla("kn,,m")),
                 "kn,,m");

  // -------------------------------------------------------------------------
  // Round trip: words
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
  // Original hard case
  // -------------------------------------------------------------------------

  const std::string hard = "ক্ষ্ম্যজ্ঞ্যন্ত্‌র্য্যশ্চ্ছ্বঙ্ক্ষ্ট্র্দ্ধ্ব্যাঁক্‌ন্‌ম্গ্ন্ত্র্যৌক্ক্হৃঙ্ঘ্ন্"
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


  return testing::report("engine");
}