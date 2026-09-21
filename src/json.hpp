#pragma once
// Minimal dependency-free JSON reader, used only to load data/*.json.
// It is deliberately tiny: the mapping files are flat objects of objects.
#include <map>
#include <string>
#include <variant>
#include <vector>

namespace okkhor::json {

struct Value;
using Object = std::map<std::string, Value>;
using Array = std::vector<Value>;

struct Value {
    std::variant<std::nullptr_t, bool, double, std::string, Array, Object> v{nullptr};

    bool is_null()   const { return std::holds_alternative<std::nullptr_t>(v); }
    bool is_object() const { return std::holds_alternative<Object>(v); }
    bool is_string() const { return std::holds_alternative<std::string>(v); }
    bool is_number() const { return std::holds_alternative<double>(v); }

    const Object&      as_object() const { return std::get<Object>(v); }
    const std::string& as_string() const { return std::get<std::string>(v); }
    double             as_number() const { return std::get<double>(v); }

    const Value* find(const std::string& key) const {
        if (!is_object()) return nullptr;
        const auto& o = as_object();
        auto it = o.find(key);
        return it == o.end() ? nullptr : &it->second;
    }
    std::string string_or(const std::string& key, const std::string& fallback) const {
        const Value* x = find(key);
        return (x && x->is_string()) ? x->as_string() : fallback;
    }
    int int_or(const std::string& key, int fallback) const {
        const Value* x = find(key);
        return (x && x->is_number()) ? static_cast<int>(x->as_number()) : fallback;
    }
};

// Throws std::runtime_error on malformed input.
Value parse(const std::string& text);
Value parse_file(const std::string& path);

}  // namespace okkhor::json
