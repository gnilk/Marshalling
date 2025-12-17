//
// Created by gnilk on 17.12.2025.
//

#include "DecoderHelpers.h"
#include "JSONDecoder.h"

using namespace gnilk;

static JSONObject::Ref FindObject(const JSONObject::Ref &root, const std::string &name);

void JSONDecoder::Begin(IReader::Ref incoming) {
    doc = JSONParser::Load(incoming);
    Initialize();
}

void JSONDecoder::Begin(const std::string &jsonData) {
    doc = JSONParser::Load(jsonData);
    Initialize();
}

void JSONDecoder::Initialize() {
    if (doc == nullptr) {
        return;
    }
}

bool JSONDecoder::BeginObject(const std::string &name) {
    if (objStack.empty()) {
        // If stack is empty - we assume the first call to 'BeginObject' is the actual object we want to deserialize...
        auto root = doc->GetRoot();
        auto rootObject = std::get_if<JSONObject::Ref>(&root);
        if (rootObject == nullptr) {
            // this is an array - and we don't yet process it..
            return false;
        }
        objStack.push(*rootObject);
        return true;
    }

    auto node = FindObject(objStack.top(), name);
    if (node == nullptr) {
        return false;
    }

    objStack.push(node);
    return true;
}

bool JSONDecoder::HasObject(const std::string &name) {
    if (objStack.empty()) {
        return false;
    }
    if (FindObject(objStack.top(), name) == nullptr) {
        return false;
    }
    return true;
}

void JSONDecoder::EndObject()  {
    objStack.pop();
}

std::optional<bool> JSONDecoder::ReadBoolField(const std::string &name) {
    if (objStack.empty()) return {};
    // We know this exists
    auto &value = objStack.top()->GetValue(name);
    if (value == nullptr) {
        return {};
    }
    // All except Object & Array are stored as strings...
    if (!value->IsString()) {
        return {};
    }

    auto &strValue = value->GetAsString();
    auto res = convert_to<bool>(strValue);

    return res;
}
std::optional<int> JSONDecoder::ReadIntField(const std::string &name) {
    if (objStack.empty()) return {};
    // We know this exists
    auto &value = objStack.top()->GetValue(name);
    if (value == nullptr) {
        return {};
    }
    // All except Object & Array are stored as strings...
    if (!value->IsString()) {
        return {};
    }

    auto &strValue = value->GetAsString();
    auto res = convert_to<int>(strValue);

    return res;
}
std::optional<int64_t> JSONDecoder::ReadInt64Field(const std::string &name) {
    return {};
}

std::optional<float> JSONDecoder::ReadFloatField(const std::string &name) {
    if (objStack.empty()) return {};
    // We know this exists
    auto &value = objStack.top()->GetValue(name);
    if (value == nullptr) {
        return {};
    }
    // All except Object & Array are stored as strings...
    if (!value->IsString()) {
        return {};
    }

    auto &strValue = value->GetAsString();
    auto res = convert_to<float>(strValue);

    return res;
}

std::optional<std::string> JSONDecoder::ReadTextField(const std::string &name) {
    if (objStack.empty()) return {};
    // We know this exists
    auto &value = objStack.top()->GetValue(name);
    if (value == nullptr) {
        return {};
    }
    // All except Object & Array are stored as strings...
    if (!value->IsString()) {
        return {};
    }

    auto &strValue = value->GetAsString();
    return {strValue};
}


// herlps
static JSONObject::Ref FindObject(const JSONObject::Ref &root, const std::string &name) {
    if (root->GetName() == name) return root;

    for(auto &[memberName, value] : root->GetValues()) {
        if (memberName != name) continue;
        if (value->IsObject()) return value->GetAsObject();
    }

    return {};
}
