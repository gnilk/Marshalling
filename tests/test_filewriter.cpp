//
// Created by gnilk on 17.09.25.
//
#include <string>
#include <stdio.h>
#include <testinterface.h>
#include "FileWriter.h"

using namespace gnilk;

extern "C" int test_filewriter_create(ITesting *t) {
    auto fw = FileWriter::Create(nullptr);
    auto res = fw->Write(nullptr, 0);
    TR_ASSERT(t, res == -1);
    return kTR_Pass;
}

extern "C" int test_filewriter_createnew(ITesting *t) {
    FILE *f = fopen("__test__filewriter.tst", "w+");
    TR_ASSERT(t, f != nullptr);
    auto fw = FileWriter::Create(f);
    std::string dummy = "apakaka";
    auto res = fw->Write(dummy.c_str(), dummy.length());
    TR_ASSERT(t, res > 0);
    res = fw->Flush();
    TR_ASSERT(t, res == 0);

    fclose(f);
    return kTR_Pass;
}
