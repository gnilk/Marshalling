//
// Created by gnilk on 16.12.25.
//

#include "XMLDecoder.h"
#include <charconv>

using namespace gnilk;

// Helpers..
template<typename T>
[[nodiscard]]
static std::optional<T> convert_to(std::string_view sv) {

    if constexpr (std::is_same_v<T, std::string>) {
        return std::string{sv};
    }
    else if constexpr (std::is_same_v<T, std::string_view>) {
        return sv;
    }
    else if constexpr (std::is_same_v<T, bool>) {
        if (sv == "1" || sv == "true"  || sv == "TRUE")  return true;
        if (sv == "0" || sv == "false" || sv == "FALSE") return false;
        return std::nullopt;
    }
    else if constexpr (std::is_arithmetic_v<T>) {
        T out{};
        auto first = sv.data();
        auto last  = sv.data() + sv.size();

        // std::from_chars handles all integers and floating point (C++17+)
        auto [ptr, ec] = std::from_chars(first, last, out);
        if (ec == std::errc{} && ptr == last)
            return out;

        return std::nullopt;
    }
    else {
        // Unsupported type; static assert provides helpful compile-time error
        static_assert(!sizeof(T*), "convert_to<T>: Unsupported type");
    }
}

static xml::Tag::Ref FindNode(const xml::Tag::Ref &root, const std::string &name) {
    if (root->GetName() == name) return root;
    for(auto &ch : root->GetChildren()) {
        if (ch->GetName() == name) {
            return ch;
        }
    }
    return nullptr;
}

void XMLDecoder::Begin(const std::string &xmldata) {
    doc = xml::XMLParser::Load(xmldata);
    if (doc == nullptr) {
        return;
    }
    auto root = doc->GetRoot();
    if (root == nullptr) {
        return;
    }
    tagStack.push(doc->GetRoot());
}

bool XMLDecoder::BeginObject(const std::string &name) {
    if (tagStack.empty()) return false;
    auto node = FindNode(tagStack.top(), name);
    if (node != nullptr) {
        tagStack.push(node);
        return true;
    }
    return false;
}
void XMLDecoder::EndObject() {
    tagStack.pop();
}
bool XMLDecoder::HasObject(const std::string &name) {
    if (tagStack.empty()) {
        return false;
    }
    if (FindNode(tagStack.top(), name) != nullptr) {
        return true;
    }

    return false;
}

std::optional<bool> XMLDecoder::ReadBoolField(const std::string &name) {
    return {};
}
std::optional<int> XMLDecoder::ReadIntField(const std::string &name) {
    auto optAttrib = ReadTextField(name);
    if (!optAttrib.has_value()) {
        return {};
    }

    if (tagStack.empty()) return {};
    auto &attrib = tagStack.top()->GetAttributeValue(name,"");
    if (attrib.empty()) {
        return {};
    }
    auto res = convert_to<int>(attrib);
    return {res};

    return {};
}
std::optional<int64_t> XMLDecoder::ReadInt64Field(const std::string &name) {
    return {};
}
std::optional<float> XMLDecoder::ReadFloatField(const std::string &name) {
    return {};
}
std::optional<std::string> XMLDecoder::ReadTextField(const std::string &name) {
    if (tagStack.empty()) return {};
    auto &attrib = tagStack.top()->GetAttributeValue(name,"");
    if (attrib.empty()) {
        return {};
    }
    return {attrib};
}
