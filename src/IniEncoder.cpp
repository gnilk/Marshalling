//
// Created by gnilk on 12/12/2022.
//

#include "IniEncoder.h"

using namespace gnilk;



// This is an optional call to set the writer
void IniEncoder::Begin(IWriter::Ref writer) {
    ss.Begin(writer);
}

// This is used by the messages
void IniEncoder::BeginObject(const std::string &name) {
    if (name.length() > 0) {
        ss << "[" << name << "]" << StringWriter::eol;
    }
}

void IniEncoder::EndObject() {
    // Add an empty line - makes it easier to read the resulting output
    ss << StringWriter::eol;
}

// Note: 'hasNext' is deprecated, parameter ignored
void IniEncoder::WriteBoolField(const std::string &name, bool value) {
    ss << name << " = " << value << StringWriter::eol;
}

void IniEncoder::WriteIntField(const std::string &name, int value) {
    ss << name << " = " << value << StringWriter::eol;
}
void IniEncoder::WriteInt64Field(const std::string &name, int64_t value) {
    ss << name << " = " << value << StringWriter::eol;
}

void IniEncoder::WriteFloatField(const std::string &name, double value) {
    ss << name << " = " << value << StringWriter::eol;
}

void IniEncoder::WriteTextField(const std::string &name, const std::string &value) {
    ss << name << " = " << value << StringWriter::eol;
}
