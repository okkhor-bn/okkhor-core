// End-to-end: Latin phonetic input -> Bangla UTF-8.
#include <string>

#include "okkhor.hpp"
#include "test_util.hpp"

using namespace okkhor;
using testing::check;
using testing::check_true;

int main() {
  Engine engine = Engine::from_data_dir(find_data_dir());
  auto t = [&](const std::string &in) { return engine.transliterate(in); };

  // Section 24 of the specification.
  check("k", t("k"), "ক");
  check("ka", t("ka"), "কা");
  check("ki", t("ki"), "কি");
  check("ko", t("ko"), "ক");
  check("kO", t("kO"), "কো");

  check("oi", t("oi"), "অই");
  check("koi", t("koi"), "কই");
  check("o", t("o"), "অ");

  check("kt", t("kt"), "ক্ত");
  check("kta", t("kta"), "ক্তা");
  check("k,,", t("k,,"), "ক্");
  check("kt,,", t("kt,,"), "ক্ত্");

  check("`o", t("`o"), "");
  check("`a", t("`a"), "া");
  check("`i", t("`i"), "ি");
  check("`I", t("`I"), "ী");
  check("`u", t("`u"), "ু");
  check("`U", t("`U"), "ূ");
  check("`e", t("`e"), "ে");
  check("`O", t("`O"), "ো");
  check("`OU", t("`OU"), "ৌ");

  check("k|t", t("k|t"), "ক\u200Cত");

  // Consonant + each vowel.
  check("ka", t("ka"), "কা");
  check("kI", t("kI"), "কী");
  check("ku", t("ku"), "কু");
  check("kU", t("kU"), "কূ");
  check("krri", t("krri"), "কৃ");
  check("ke", t("ke"), "কে");
  check("kOI", t("kOI"), "কৈ");
  check("kOU", t("kOU"), "কৌ");

  // Longest-match tokenization.
  check("kha", t("kha"), "খা");
  check("chi", t("chi"), "ছি");
  check("Sh", t("Sha"), "ষা");
  check("Ng", t("Nga"), "ঙা");

  // Conjuncts of three and four members, built algebraically.
  check("kkhr", t("kkhr"), "ক্খ্র");
  check("strI", t("strI"), "স্ত্রী");

  // Fola is just a rendering of a dcons.
  check("kro", t("kro"), "ক্র");
  check("kzo", t("kzo"), "ক্য");
  check("bidzut", t("bidzut"), "বিদ্যুত");

  // Backtick as a virtual consonant base.
  check("`ka", t("`ka"), "্কা");

  // Accents.
  check("bangla", t("bangla"), "বাংলা");
  check("cha^d", t("cha^d"), "ছাঁদ");

  // Words, purely from the algebra -- no dictionary, no per-word rules.
  // Note that "kkh" is k + kh = ক্খ; ক্ষ is written k + Sh, as the algebra
  // demands.
  check("okkhor", t("okkhor"), "অক্খর");
  check("okShor", t("okShor"), "অক্ষর");
  check("amar", t("amar"), "আমার");
  check("bangla", t("bangla"), "বাংলা");

  // Whitespace terminates a unit; conjuncts never cross it.
  check("k t", t("k t"), "ক ত");
  check("ami tOmake", t("ami tOmake"), "আমি তোমাকে");

  // Unknown input is preserved.
  // `o` contributes no dependent sign, and digits are not mapped by default.
  check("hello@123", t("hello@123"), "হেল্ল@123");
  check_true("unknown characters survive",
             t("@#%").find("@#%") != std::string::npos);

  return testing::report("phonetic_tests");
}
