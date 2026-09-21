// Tests the algebra and the internal representation, independently of any
// phonetic behaviour: units are built by calling the algebra directly.
#include <string>

#include "mapping.hpp"
#include "okkhor.hpp"
#include "orthography.hpp"
#include "renderer.hpp"
#include "test_util.hpp"

using namespace okkhor;
using testing::check;
using testing::check_true;

int main() {
  Mapping mapping = Mapping::load(find_data_dir());
  Renderer renderer(mapping);

  const Consonant K{mapping.consonant_id("k")};
  const Consonant T{mapping.consonant_id("t")};
  const Consonant R{mapping.consonant_id("r")};
  const Vowel O_INHERENT{mapping.vowel_id("o")};
  const Vowel A{mapping.vowel_id("a")};
  const Vowel I{mapping.vowel_id("i")};

  // C -> BC (inherent অ is a state, not a character)
  {
    OrthographicUnit u = make_consonant(K);
    check_true("make_consonant leaves the unit open", is_open(u));
    check_true("make_consonant has no vowel", !u.vowel.has_value());
    check("C -> ক", renderer.render(u), "ক");
  }

  // H + অ -> VN
  {
    OrthographicUnit u = make_consonant(K);
    check_true("add_vowel(o) succeeds", add_vowel(u, O_INHERENT));
    check_true("vowel is stored semantically", u.vowel.has_value());
    check("C + o -> ক", renderer.render(u), "ক");
  }

  // C + V -> BC + DV
  {
    OrthographicUnit u = make_consonant(K);
    add_vowel(u, A);
    check("C + a -> কা", renderer.render(u), "কা");
    check_true("a vowel closes the unit", !is_open(u));
    check_true("no second vowel", !add_vowel(u, I));
    check_true("no consonant after a vowel", !add_consonant(u, T));
  }

  // C + C -> BC + DC + VN
  {
    OrthographicUnit u = make_consonant(K);
    check_true("add_consonant succeeds", add_consonant(u, T));
    check_true("one dependent consonant", u.conjuncts.size() == 1);
    check("C + C -> ক্ত", renderer.render(u), "ক্ত");
  }

  // C + C + V -> BC + DC + DV
  {
    OrthographicUnit u = make_consonant(K);
    add_consonant(u, T);
    add_vowel(u, A);
    check("C + C + V -> ক্তা", renderer.render(u), "ক্তা");
  }

  // Arbitrarily long conjuncts: C + C + C, and C + C + C + V
  {
    OrthographicUnit u = make_consonant(Consonant{mapping.consonant_id("s")});
    add_consonant(u, T);
    add_consonant(u, R);
    check("C + C + C -> স্ত্র", renderer.render(u), "স্ত্র");
    add_vowel(u, Vowel{mapping.vowel_id("I")});
    check("C + C + C + V -> স্ত্রী", renderer.render(u), "স্ত্রী");
    check_true("three-member cluster", u.conjuncts.size() == 2);
  }

  // C + ,, -> BC + H, including on a conjunct
  {
    OrthographicUnit u = make_consonant(K);
    check_true("terminate_with_hasanta succeeds", terminate_with_hasanta(u));
    check("C + ,, -> ক্", renderer.render(u), "ক্");
    check_true("hasanta seals the unit", !add_vowel(u, A));

    OrthographicUnit c = make_consonant(K);
    add_consonant(c, T);
    terminate_with_hasanta(c);
    check("C + C + ,, -> ক্ত্", renderer.render(c), "ক্ত্");
  }

  // ` -> VC
  {
    OrthographicUnit u = make_virtual_consonant();
    check_true("virtual base", is_virtual(u));
    check("` -> \"\"", renderer.render(u), "");

    OrthographicUnit v = make_virtual_consonant();
    add_vowel(v, A);
    check("` + V -> া", renderer.render(v), "া");

    OrthographicUnit o = make_virtual_consonant();
    add_vowel(o, O_INHERENT);
    check("` + o -> \"\"", renderer.render(o), "");

    OrthographicUnit c = make_virtual_consonant();
    add_consonant(c, R);
    check("` + C -> ্র", renderer.render(c), "্র");
    add_vowel(c, A);
    check("` + C + V -> ্রা", renderer.render(c), "্রা");
  }

  // ZWNJ
  {
    OrthographicUnit u = make_consonant(K);
    add_zwnj(u);
    check("C + zwnj", renderer.render(u), "ক\u200C");
    check_true("zwnj seals the unit", !add_consonant(u, T));
  }

  // Independent vowel
  {
    IndependentVowel v{O_INHERENT, {}, false};
    check("independent o -> অ", renderer.render(v), "অ");
  }

  return testing::report("algebra_tests");
}
