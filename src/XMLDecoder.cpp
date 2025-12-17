//
// Created by gnilk on 16.12.25.
//

#include "XMLDecoder.h"
#include "DecoderHelpers.h"
#include <charconv>

using namespace gnilk;

// Helpers..

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
