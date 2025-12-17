//
// Created by gnilk on 17.12.2025.
//

#include <testinterface.h>
#include "JSONDecoder.h"
#include "IDeserializable.h"

class MyOtherObject : public gnilk::IDeserializable {
public:
    MyOtherObject() = default;
    virtual ~MyOtherObject() = default;

    void DeserializeFrom(gnilk::IDecoder &decoder) override {
        if (!decoder.BeginObject("subobj")) {
            return;
        }
        auto optNum = decoder.ReadIntField("other_num");
        if (optNum.has_value()) {
            num = *optNum;
        } else {
            num = -1;
        }

        decoder.EndObject();
    }
public:
    int num;
};

class MyRootObject : public gnilk::IDeserializable {
public:
    MyRootObject() = default;
    virtual ~MyRootObject() = default;

    void DeserializeFrom(gnilk::IDecoder &decoder) override {
        if (!decoder.BeginObject("MyRootObject")) {
            return;
        }
        auto optNum = decoder.ReadIntField("num");
        if (optNum.has_value()) {
            num = *optNum;
        } else {
            num = -1;
        }
        other.DeserializeFrom(decoder);
        decoder.EndObject();
    }
public:
    int num;
    MyOtherObject other;
};



using namespace gnilk;

extern "C" int test_jsondecoder_object_simple(ITesting *t) {
    static std::string data = "{ " \
                              "\"num\" : 1 " \
                              "}";

    JSONDecoder decoder;
    MyRootObject myObj;
    decoder.Begin(data);
    myObj.DeserializeFrom(decoder);
    TR_ASSERT(t, myObj.num == 1);
    return kTR_Pass;
}

extern "C" int test_jsondecoder_object_wobject(ITesting *t) {
    static std::string data = "{ " \
                              "\"subobj\" : { \"other_num\" : 1 } " \
                              "}";

    JSONDecoder decoder;
    MyRootObject myObj;
    decoder.Begin(data);
    myObj.DeserializeFrom(decoder);
    TR_ASSERT(t, myObj.num == -1);
    TR_ASSERT(t, myObj.other.num == 1);
    return kTR_Pass;
}