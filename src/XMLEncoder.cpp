//
// Created by gnilk on 17.09.25.
//

#include <utility>
#include <assert.h>
#include "XMLEncoder.h"

using namespace gnilk;

static const std::string begintag("<");
static const std::string beginendtag("</");
static const std::string singleendtag("/>");
static const std::string endtag(">");
static const std::string xmlquote("\"");
static const std::string xmlequals("=");
static const std::string nextfield(",");
static const std::string xmlempty("");
static const std::string xmlspace(" ");

static const std::string xmlEnvelope("<?xml version=\"1.0\"?>");


void XMLEncoder::PrettyPrint(bool use) {
    pretty = use;
    eol = pretty ? gnilk::StringWriter::eol : "";
}
void XMLEncoder::WriteEnvelopeOnFirstObject(bool use) {
    writeEnvelopeOnFirstObject = use;
}

#define spacing() (pretty?std::string(fieldStack.size(), '\t') : xmlempty)

void XMLEncoder::WriteEnvelope() {
    if (writeEnvelopeOnFirstObject && objectStack.empty()) {
        ss << xmlEnvelope << eol;
    }
}
void XMLEncoder::BeginObject(const std::string &name) {
    assert(!name.empty());
    WriteEnvelope();
    ss << spacing() << begintag << name << endtag << eol;

    objectStack.push_back(name);
    fieldStack.push_back(fieldCount);
    fieldCount = 0;
}

static std::string attr_value(const EncoderObjectAttribute &attr) {
    const int *pInt = std::get_if<int>(&attr.value);
    if (pInt) {
        return std::to_string(*pInt);
    }
    const double *pDouble = std::get_if<double>(&attr.value);
    if (pDouble) {
        return std::to_string(*pDouble);
    }
    return std::get<std::string>(attr.value);
}

// rvalue / lvalue wrappers - see 'Begin/Single'-ObjectImpl
void XMLEncoder::BeginObject(const std::string &name, const std::vector<EncoderObjectAttribute > &&attributes) {
    BeginObjectImpl(name, attributes);
}
void XMLEncoder::SingleObject(const std::string &name, const std::vector<EncoderObjectAttribute > &&attributes) {
    SingleObjectImpl(name, attributes);
}
void XMLEncoder::BeginObject(const std::string &name, const std::vector<EncoderObjectAttribute > &attributes) {
    BeginObjectImpl(name, attributes);
}
void XMLEncoder::SingleObject(const std::string &name, const std::vector<EncoderObjectAttribute > &attributes) {
    SingleObjectImpl(name, attributes);
}

void XMLEncoder::BeginObjectImpl(const std::string &name, const std::vector<EncoderObjectAttribute > &attributes) {
    assert(!name.empty());

    WriteEnvelope();
    ss << spacing() << begintag << name;
    if (attributes.size() > 0) {
        for (auto &attr : attributes) {
            ss << xmlspace;
            ss << attr.name << xmlequals << xmlquote << EncoderObjectAttribute::ToString(attr) << xmlquote;
        }
    }
    ss << endtag << eol;

    objectStack.push_back(name);
    fieldStack.push_back(fieldCount);
    fieldCount = 0;

}
void XMLEncoder::SingleObjectImpl(const std::string &name, const std::vector<EncoderObjectAttribute > &attributes) {
    assert(!name.empty());

    WriteEnvelope();
    ss << spacing() << begintag << name;
    if (attributes.size() > 0) {
        for (auto &attr : attributes) {
            ss << xmlspace;
            ss << attr.name << xmlequals << xmlquote << attr_value(attr) << xmlquote;
        }
    }
    ss << singleendtag << eol;
}


void XMLEncoder::EndObject() {
    auto name = objectStack.back();
    objectStack.pop_back();

    fieldCount = fieldStack.back();
    fieldStack.pop_back();

    ss << spacing() << beginendtag << name << endtag << eol;
}

void XMLEncoder::WriteBoolField(const std::string &name, bool value) {
    ss << spacing() << begintag << name << endtag << std::to_string(value) << beginendtag << name << endtag << eol;
}
void XMLEncoder::WriteIntField(const std::string &name, int value) {
    ss << spacing() << begintag << name << endtag << std::to_string(value) << beginendtag << name << endtag << eol;
}
void XMLEncoder::WriteInt64Field(const std::string &name, int64_t value) {
    ss << spacing() << begintag << name << endtag << std::to_string(value) << beginendtag << name << endtag << eol;
}
void XMLEncoder::WriteFloatField(const std::string &name, double value) {
    ss << spacing() << begintag << name << endtag << std::to_string(value) << beginendtag << name << endtag << eol;
}
void XMLEncoder::WriteTextField(const std::string &name, const std::string &value) {
    ss << spacing() << begintag << name << endtag << value << beginendtag << name << endtag << eol;
}

