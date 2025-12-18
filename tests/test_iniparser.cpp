//
// Created by gnilk on 18.12.2025.
//
#include <testinterface.h>
#include "../src/IniParser.h"

using namespace gnilk;

extern "C" int test_iniparser_simple(ITesting *t) {
    static std::string data = "[section]\n"\
                              "key=value\n";

    IniParser parser(data);
    TR_ASSERT(t, parser.ProcessData());
    auto section = parser.GetSection("section");
    TR_ASSERT(t, section != nullptr);
    TR_ASSERT(t, !section->values.empty());
    TR_ASSERT(t, section->values.size() == 1);
    return kTR_Pass;
}

extern "C" int test_iniparser_multisection(ITesting *t) {
    static std::string data = "[sectionA]\n"\
                              "key=value\n"\
                              "[sectionB]\n"\
                              "key=value\n"\
                              "[sectionC]\n"\
                              "key=value\n";

    IniParser parser(data);
    TR_ASSERT(t, parser.ProcessData());
    static std::vector<std::string> sections = {"sectionA", "sectionB", "sectionC"};
    for(auto &sname : sections) {
        auto s = parser.GetSection(sname);
        TR_ASSERT(t, s != nullptr);
        TR_ASSERT(t, s->name == sname);
        TR_ASSERT(t, !s->values.empty());
        printf("s: %s, values: %zu\n", s->name.c_str(), s->values.size());
        TR_ASSERT(t, s->values.size() == 1);
        std::pair<std::string, std::string> kvp = {"key", "value"};
        TR_ASSERT(t, s->values[0] == kvp);
    }
    return kTR_Pass;
}

extern "C" int test_iniparser_multikvp(ITesting *t) {
    static std::string data = "[sectionA]\n"\
                              "keyA=valueA\n"\
                              "keyB=valueB\n"\
                              "keyC=valueC\n";

    IniParser parser(data);
    TR_ASSERT(t, parser.ProcessData());
    static std::vector<std::pair<std::string, std::string>> expKVP = {
            {"keyA", "valueA"},
            {"keyB", "valueB"},
            {"keyC", "valueC"},
    };

    auto s = parser.GetSection("sectionA");
    TR_ASSERT(t, s != nullptr);
    TR_ASSERT(t, !s->values.empty());
    TR_ASSERT(t, s->values.size() == expKVP.size());
    for(size_t i=0;i<s->values.size();i++) {
        TR_ASSERT(t, s->values[i] == expKVP[i]);
    }
    return kTR_Pass;
}

extern "C" int test_iniparser_multi_multi(ITesting *t) {
    static std::string data = "[sectionA]\n"\
                              "keyA=valueA\n"\
                              "keyB=valueB\n"\
                              "keyC=valueC\n"\
                              "[sectionB]\n"\
                              "keyA=valueA\n"\
                              "keyB=valueB\n"\
                              "keyC=valueC\n"\
                              "[sectionC]\n"\
                              "keyA=valueA\n"\
                              "keyB=valueB\n"\
                              "keyC=valueC\n";


    IniParser parser(data);
    TR_ASSERT(t, parser.ProcessData());
    static std::vector<std::string> expSections = {"sectionA", "sectionB", "sectionC"};
    static std::vector<std::pair<std::string, std::string>> expKVP = {
            {"keyA", "valueA"},
            {"keyB", "valueB"},
            {"keyC", "valueC"},
    };

    for(auto &sName : expSections) {
        auto s = parser.GetSection(sName);
        TR_ASSERT(t, s != nullptr);
        TR_ASSERT(t, !s->values.empty());
        TR_ASSERT(t, s->values.size() == expKVP.size());
        for(size_t i=0;i<s->values.size();i++) {
            TR_ASSERT(t, s->values[i] == expKVP[i]);
        }
    }
    return kTR_Pass;
}