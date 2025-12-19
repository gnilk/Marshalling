//
// Created by gnilk on 19.12.2025.
//
#include <string>
#include <testinterface.h>
#include "../src/XMLParser.h"

using namespace gnilk;

// FIXME: Failing tests - should work...
extern "C" int test_xmlparser_attrib_whitespace(ITesting *t) {
    static std::string data = "<node field = \"value\" />";
    auto doc = xml::XMLParser::Load(data);
    TR_ASSERT(t, doc != nullptr);
    auto root = doc->GetRoot();
    TR_ASSERT(t, root != nullptr);
    auto node = root->GetFirstChild("node");
    TR_ASSERT(t, node != nullptr);
    auto value = node->GetAttributeValue("field", "not_this");
    TR_ASSERT(t, value == "value");

    return kTR_Pass;
}

extern "C" int test_xmlparser_attrib_singlquote(ITesting *t) {
    static std::string data = "<node field=\'value\' />";
    auto doc = xml::XMLParser::Load(data);
    TR_ASSERT(t, doc != nullptr);
    auto root = doc->GetRoot();
    TR_ASSERT(t, root != nullptr);
    auto node = root->GetFirstChild("node");
    TR_ASSERT(t, node != nullptr);
    auto value = node->GetAttributeValue("field", "not_this");
    TR_ASSERT(t, value == "value");

    return kTR_Pass;
}

// Ok
extern "C" int test_xmlparser_attrib_single(ITesting *t) {
    static std::string data = "<node field=\"value\" />";
    auto doc = xml::XMLParser::Load(data);
    TR_ASSERT(t, doc != nullptr);
    auto root = doc->GetRoot();
    TR_ASSERT(t, root != nullptr);
    auto node = root->GetFirstChild("node");
    TR_ASSERT(t, node != nullptr);
    auto value = node->GetAttributeValue("field", "not_this");
    TR_ASSERT(t, value == "value");

    return kTR_Pass;
}

// ok
extern "C" int test_xmlparser_attrib_mixed(ITesting *t) {
    static std::string data = "<node field='value\" />";

    // Should fail.
    auto doc = xml::XMLParser::Load(data);
    TR_ASSERT(t, doc == nullptr);

    return kTR_Pass;
}

// Ok..
extern "C" int test_xmlparser_attrib_multi(ITesting *t) {
    static std::string data = "<node field=\"value\" field2=\"value2\"/>";
    auto doc = xml::XMLParser::Load(data);
    TR_ASSERT(t, doc != nullptr);
    auto root = doc->GetRoot();
    TR_ASSERT(t, root != nullptr);
    auto node = root->GetFirstChild("node");
    TR_ASSERT(t, node != nullptr);
    std::string value;
    value = node->GetAttributeValue("field", "not_this");
    TR_ASSERT(t, value == "value");

    value = node->GetAttributeValue("field2", "not_this");
    TR_ASSERT(t, value == "value2");

    return kTR_Pass;
}

extern "C" int test_xmlparser_nested_one(ITesting *t) {
    static std::string data = "<?xml version=\"1.0\"?>\n"
                                 "<event version=\"2.0\" type=\"a-f-A-M-F\" uid=\"drone-cc-app\" time=\"2025-12-16T08:30:28.045Z\" start=\"2025-12-16T08:30:28.045Z\" stale=\"2025-12-16T08:32:28.045Z\" how=\"m-g\">\n"
                                 "        <detail>\n"
                                 "                <__tracker_cmd cmd=\"lock\" target_id=\"1\"/>\n"
                                 "                <contact callsign=\"drone-cc-app\"/>\n"
                                 "        </detail>\n"
                                 "</event>";

    // Just interested if parsing went fine...
    auto doc = xml::XMLParser::Load(data);
    TR_ASSERT(t, doc != nullptr);
    auto root = doc->GetRoot();
    TR_ASSERT(t, root != nullptr);
    auto node = root->GetFirstChild("event");
    TR_ASSERT(t, node != nullptr);

    return kTR_Pass;
}

