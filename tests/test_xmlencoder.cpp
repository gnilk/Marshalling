//
// Created by gnilk on 17.09.25.
//

#include <testinterface.h>

#include "FileWriter.h"
#include "XMLEncoder.h"

using namespace gnilk;

extern "C" int test_xmlencoder_simple(ITesting *t) {

    FileWriter::Ref fw = FileWriter::Create(stdout);
    XMLEncoder encoder(fw);

    encoder.PrettyPrint(true);
    encoder.BeginObject("Root");
    //encoder.WriteIntField("value", 32);
    encoder.EndObject();

    printf("\n");

    return kTR_Pass;
}
extern "C" int test_xmlencoder_simple_fields(ITesting *t) {
    FileWriter::Ref fw = FileWriter::Create(stdout);
    XMLEncoder encoder(fw);

    encoder.PrettyPrint(true);
    encoder.BeginObject("Root");
    encoder.WriteIntField("IntField", 32);
    encoder.WriteBoolField("BoolField", true);
    encoder.WriteTextField("TextField", "some text goes here");
    encoder.WriteFloatField("FloatField", 123.456);
    encoder.EndObject();

    printf("\n");

    return kTR_Pass;
}

extern "C" int test_xmlencoder_objwattrs(ITesting *t) {

    FileWriter::Ref fw = FileWriter::Create(stdout);
    XMLEncoder encoder(fw);

    encoder.PrettyPrint(true);
    //encoder.BeginObject("Root", {{"uid","123"}, {"type","mamma"}});
    encoder.BeginObject("Root", {{"intvalue",123}, {"floatvalue",1.3},{"string","wwefwef"}});
    //encoder.WriteIntField("value", 32);
    encoder.EndObject();

    printf("\n");

    return kTR_Pass;
}

extern "C" int test_xmlencoder_singleobj(ITesting *t) {

    FileWriter::Ref fw = FileWriter::Create(stdout);
    XMLEncoder encoder(fw);

    encoder.PrettyPrint(true);

    encoder.BeginObject("Root", {{"intvalue",123}, {"floatvalue",1.3},{"string","wwefwef"}});
    encoder.SingleObject("Point", {{"x-pos",10.0}, {"y-pos",20.0}});

    encoder.EndObject();

    printf("\n");

    return kTR_Pass;
}

extern "C" int test_xmlencoder_feature(ITesting *t) {

    FileWriter::Ref fw = FileWriter::Create(stdout);
    XMLEncoder encoder(fw);

    TR_ASSERT(t, encoder.IsFeatureSupported(IEncoder::kFeature::kSupportsAttributes));

    return kTR_Pass;
}
