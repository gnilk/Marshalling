//
// Created by gnilk on 18.12.2025.
//
#include <testinterface.h>
#include "../src/IniDecoder.h"
#include "IDeserializable.h"

using namespace gnilk;

class MySection : public IDeserializable {
public:
    MySection() = default;
    MySection(const std::string &name) : sectionName(name) {

    }
    virtual ~MySection() = default;
    void DeserializeFrom(IDecoder &decoder) {
        if (!decoder.BeginObject(sectionName)) {
            return;
        }
        auto optStr = decoder.ReadTextField("str");
        if (optStr.has_value()) {
            strValue = *optStr;
        }
        auto optBool = decoder.ReadBoolField("bool");
        if (optBool.has_value()) {
            boolValue = *optBool;
        }
        auto optInt = decoder.ReadIntField("int");
        if (optInt.has_value()) {
            intValue = *optInt;
        }
        auto optFloat = decoder.ReadFloatField("float");
        if (optFloat.has_value()) {
            floatValue = *optFloat;
        }
        decoder.EndObject();
    }
public:
    std::string strValue = {};
    int intValue = 0;
    float floatValue = 0;
    bool boolValue = false;
public:
    std::string sectionName = "section";
};

extern "C" int test_inidecoder_simple(ITesting *t) {
    static std::string data = "[section]\n"\
                              "str=value\n";



    MySection section;
    IniDecoder decoder(data);

    section.DeserializeFrom(decoder);

    TR_ASSERT(t, section.strValue == "value");
    return kTR_Pass;
}

extern "C" int test_inidecoder_types(ITesting *t) {
    static std::string data = "[section]\n"\
                              "str=value\n"\
                              "bool=true\n"\
                              "int=42\n"\
                              "float=1.23\n";



    MySection section;
    IniDecoder decoder(data);

    section.DeserializeFrom(decoder);

    TR_ASSERT(t, section.strValue == "value");
    TR_ASSERT(t, section.boolValue == true);
    TR_ASSERT(t, section.intValue == 42);
    TR_ASSERT(t, fabs(section.floatValue - 1.23) < 0.01);
    return kTR_Pass;
}

class MultiSection : public IDeserializable {
public:
    MultiSection() = default;
    virtual ~MultiSection() = default;
    void DeserializeFrom(IDecoder &decoder) {
        if (decoder.HasObject(sectionA.sectionName)) {
            sectionA.DeserializeFrom(decoder);
        }
        if (decoder.HasObject(sectionB.sectionName)) {
            sectionB.DeserializeFrom(decoder);
        }
    }
public:
    MySection sectionA = MySection("sectionA");
    MySection sectionB = MySection("sectionB");
};

extern "C" int test_inidecoder_multisection(ITesting *t) {
    static std::string data = "[sectionA]\n"\
                              "str=valueA\n"\
                              "[sectionB]\n"\
                              "str=valueB\n";


    MultiSection multiSection;
    IniDecoder iniDecoder(data);

    multiSection.DeserializeFrom(iniDecoder);
    TR_ASSERT(t, multiSection.sectionA.strValue == "valueA");
    TR_ASSERT(t, multiSection.sectionB.strValue == "valueB");

    return kTR_Pass;
}