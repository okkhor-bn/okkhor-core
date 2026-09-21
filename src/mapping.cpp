#include "mapping.hpp"

#include <algorithm>
#include <stdexcept>

#include "json.hpp"

namespace okkhor {
namespace {

template <typename T>
void place(std::vector<T>& table, int id, T entry) {
    if (id < 0) throw std::runtime_error("negative id in mapping data");
    if (static_cast<std::size_t>(id) >= table.size()) table.resize(static_cast<std::size_t>(id) + 1);
    if (table[static_cast<std::size_t>(id)].id != -1)
        throw std::runtime_error("duplicate id " + std::to_string(id) + " in mapping data");
    table[static_cast<std::size_t>(id)] = std::move(entry);
}

std::string join(const std::string& dir, const char* file) {
    if (dir.empty()) return file;
    char last = dir.back();
    if (last == '/' || last == '\\') return dir + file;
    return dir + "/" + file;
}

}  // namespace

void Mapping::add_rule(const std::string& key, Rule rule) {
    if (key.empty()) throw std::runtime_error("empty key in mapping data");
    if (rules_.count(key))
        throw std::runtime_error("duplicate mapping key: " + key);
    max_key_len_ = std::max(max_key_len_, key.size());
    rules_.emplace(key, std::move(rule));
}

const Rule* Mapping::lookup(const std::string& key) const {
    auto it = rules_.find(key);
    return it == rules_.end() ? nullptr : &it->second;
}

const VowelEntry& Mapping::vowel(int id) const {
    if (id < 0 || static_cast<std::size_t>(id) >= vowels_.size())
        throw std::out_of_range("unknown vowel id");
    return vowels_[static_cast<std::size_t>(id)];
}

const ConsonantEntry& Mapping::consonant(int id) const {
    if (id < 0 || static_cast<std::size_t>(id) >= consonants_.size())
        throw std::out_of_range("unknown consonant id");
    return consonants_[static_cast<std::size_t>(id)];
}

const AccentEntry& Mapping::accent(int id) const {
    if (id < 0 || static_cast<std::size_t>(id) >= accents_.size())
        throw std::out_of_range("unknown accent id");
    return accents_[static_cast<std::size_t>(id)];
}

int Mapping::vowel_id(const std::string& latin) const {
    const Rule* r = lookup(latin);
    return (r && r->type == TokenType::Vowel) ? r->id : -1;
}

int Mapping::consonant_id(const std::string& latin) const {
    const Rule* r = lookup(latin);
    return (r && r->type == TokenType::Consonant) ? r->id : -1;
}

Mapping Mapping::load(const std::string& data_dir) {
    Mapping m;

    // ---- vowels.json ----
    {
        json::Value doc = json::parse_file(join(data_dir, "vowels.json"));
        if (!doc.is_object()) throw std::runtime_error("vowels.json: expected an object");
        for (const auto& [latin, spec] : doc.as_object()) {
            VowelEntry e;
            e.id = spec.int_or("id", -1);
            e.latin = latin;
            e.independent = spec.string_or("ind", "");
            e.dependent = spec.string_or("dep", "");
            place(m.vowels_, e.id, e);
            m.add_rule(latin, Rule{TokenType::Vowel, e.id, ""});
        }
    }

    // ---- consonants.json ----
    {
        json::Value doc = json::parse_file(join(data_dir, "consonants.json"));
        if (!doc.is_object()) throw std::runtime_error("consonants.json: expected an object");
        for (const auto& [latin, spec] : doc.as_object()) {
            ConsonantEntry e;
            e.id = spec.int_or("id", -1);
            e.latin = latin;
            e.base = spec.string_or("base", "");
            e.fola = spec.string_or("fola", "");
            place(m.consonants_, e.id, e);
            m.add_rule(latin, Rule{TokenType::Consonant, e.id, ""});
        }
    }

    // ---- controls.json ----
    {
        json::Value doc = json::parse_file(join(data_dir, "controls.json"));
        if (!doc.is_object()) throw std::runtime_error("controls.json: expected an object");

        auto simple = [&](const char* section, TokenType type) {
            const json::Value* v = doc.find(section);
            if (!v || !v->is_object()) return;
            for (const auto& [key, out] : v->as_object())
                m.add_rule(key, Rule{type, -1, out.is_string() ? out.as_string() : ""});
        };
        simple("hasanta", TokenType::Hasanta);
        simple("zwnj", TokenType::ZWNJ);
        simple("virtual", TokenType::VirtualConsonant);

        if (const json::Value* acc = doc.find("accents"); acc && acc->is_object()) {
            for (const auto& [latin, spec] : acc->as_object()) {
                AccentEntry e;
                e.id = spec.int_or("id", -1);
                e.latin = latin;
                e.sign = spec.string_or("sign", "");
                place(m.accents_, e.id, e);
                m.add_rule(latin, Rule{TokenType::Accent, e.id, ""});
            }
        }
    }

    // ---- punctuation.json ----
    {
        json::Value doc = json::parse_file(join(data_dir, "punctuation.json"));
        if (!doc.is_object()) throw std::runtime_error("punctuation.json: expected an object");
        for (const auto& [key, out] : doc.as_object())
            m.add_rule(key, Rule{TokenType::Punctuation, -1, out.is_string() ? out.as_string() : key});
    }

    return m;
}

}  // namespace okkhor
