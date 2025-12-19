//
// Created by gnilk on 16.12.25.
//
// Note: does not support 'content' of the tag nor repeated sub-tag's (i.e. lists)...
//

#include "XMLDecoder.h"
#include "DecoderHelpers.h"
#include <charconv>

using namespace gnilk;

static xml::Tag::Ref FindNode(const xml::Tag::Ref &root, const std::string &name);

// Helpers..

XMLDecoder::XMLDecoder(const std::string &data)  : docData(data) {
}

XMLDecoder::XMLDecoder(IReader::Ref instream) : docData () {
    // FIXME: the XML Parser does not support streams...
}



bool XMLDecoder::Unmarshal(IUnmarshal *rootObject) {
    // Sanity checks
    if (doc == nullptr) {
        // can't parse and empty XML document
        if (docData.empty()) return false;
        if (!Initialize()) return false;
    }
    if (rootObject == nullptr) return false;
    auto tag = doc->GetRoot();
    if (tag == nullptr) return false;

    // This doesn't make sense...  why did I do it like this??
    // traverse and unmarshal this document
    for(auto &rootChild : tag->GetChildren()) {
        if (!TraverseFrom(rootChild, rootObject)) {
            return false;
        }
    }

    return true;
}

bool XMLDecoder::TraverseFrom(const xml::Tag::Ref tag, IUnmarshal *pObject) {
    for(auto &attr : tag->GetAttributes()) {
        pObject->SetField(attr->GetName(), attr->GetValue());
    }

    for(auto &childTag : tag->GetChildren()) {
        auto pObjectChild = pObject->GetUnmarshalForField(childTag->GetName());
        if (pObjectChild == nullptr) continue;
        // Can't happen - but might in the future...
        if (!TraverseFrom(childTag, pObjectChild)) {
            return false;
        }
    }
    return true;
}

void XMLDecoder::Begin(const std::string &xmldata) {
    docData = xmldata;
    Initialize();
}

bool XMLDecoder::Initialize() {
    doc = xml::XMLParser::Load(docData);
    if (doc == nullptr) {
        return false;
    }
    auto root = doc->GetRoot();
    if (root == nullptr) {
        return false;
    }
    tagStack.push(doc->GetRoot());
    return true;
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

static xml::Tag::Ref FindNode(const xml::Tag::Ref &root, const std::string &name) {
    if (root->GetName() == name) return root;
    for(auto &ch : root->GetChildren()) {
        if (ch->GetName() == name) {
            return ch;
        }
    }
    return nullptr;
}
