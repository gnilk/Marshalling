//
// Created by gnilk on 17.09.25.
//

#include <testinterface.h>

#include "FileWriter.h"
#include "JSONEncoder.h"

using namespace gnilk;

extern "C" int test_jsonencoder_simple(ITesting *t) {

    FileWriter fw(stdout);
    JSONEncoder encoder(&fw);

    encoder.PrettyPrint(true);
    encoder.BeginObject("Root");
    encoder.WriteIntField("value", 32);
    encoder.EndObject();

    printf("\n");

    return kTR_Pass;
}