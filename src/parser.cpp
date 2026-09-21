#include "parser.hpp"

#include <optional>

namespace okkhor {

Document parse(const std::vector<Token>& tokens) {
    Document doc;
    std::optional<OrthographicUnit> cur;  // the unit currently being built

    auto flush = [&] {
        if (cur) {
            doc.push_back(*cur);
            cur.reset();
        }
    };

    for (const Token& t : tokens) {
        switch (t.type) {
            case TokenType::Consonant: {
                // C + C -> BC + DC + VN, otherwise start a fresh unit.
                if (cur && add_consonant(*cur, Consonant{t.id})) break;
                flush();
                cur = make_consonant(Consonant{t.id});
                break;
            }

            case TokenType::VirtualConsonant: {
                // ` always opens a new unit with a visually-nothing base.
                flush();
                cur = make_virtual_consonant();
                break;
            }

            case TokenType::Vowel: {
                // C + V -> BC + DV; a vowel with no open base stands alone.
                if (cur && add_vowel(*cur, Vowel{t.id})) break;
                flush();
                doc.push_back(IndependentVowel{Vowel{t.id}, {}, false});
                break;
            }

            case TokenType::Accent: {
                if (cur && add_accent(*cur, Accent{t.id})) break;
                if (!cur && !doc.empty() && std::holds_alternative<IndependentVowel>(doc.back())) {
                    std::get<IndependentVowel>(doc.back()).accents.push_back(Accent{t.id});
                    break;
                }
                flush();
                OrthographicUnit u = make_virtual_consonant();
                add_accent(u, Accent{t.id});
                doc.push_back(std::move(u));
                break;
            }

            case TokenType::Hasanta: {
                // C + ,, -> BC + H
                if (cur && terminate_with_hasanta(*cur)) {
                    flush();
                    break;
                }
                flush();
                OrthographicUnit u = make_virtual_consonant();
                terminate_with_hasanta(u);
                doc.push_back(std::move(u));
                break;
            }

            case TokenType::ZWNJ: {
                if (cur) {
                    add_zwnj(*cur);
                    flush();
                } else if (!doc.empty() && std::holds_alternative<IndependentVowel>(doc.back())) {
                    std::get<IndependentVowel>(doc.back()).zwnj_after = true;
                } else {
                    doc.push_back(Literal{t.literal});
                }
                break;
            }

            // Whitespace, punctuation and unknown input close the current unit
            // and are passed through untouched.
            case TokenType::Whitespace:
            case TokenType::Punctuation:
            case TokenType::Unknown:
            default: {
                flush();
                doc.push_back(Literal{t.literal});
                break;
            }
        }
    }

    flush();
    return doc;
}

}  // namespace okkhor
