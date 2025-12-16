//
// Created by gnilk on 23.09.25.
//

#ifndef GNILK_FILEREADER_H
#define GNILK_FILEREADER_H

#include <memory>
#include <stdio.h>

#include "IReader.h"
namespace gnilk {

    class FileReader : public IReader {
    public:
        FileReader() = delete;
        explicit FileReader(FILE *file, bool closeOnExit = false) : f(file),  bCloseOnExit(closeOnExit) {

        }
        static IReader::Ref Create(FILE *file, bool closeOnExit = false) {
            return std::make_shared<FileReader>(file, closeOnExit);
        }

        virtual ~FileReader() {
            if (bCloseOnExit) {
                fclose(f);
            }
        }

        int32_t Read(void *out, size_t maxbytes) override {
            if (f == nullptr) {
                return -1;
            }
            return fread(out, 1, maxbytes, f);
        }
        bool Available() override {
            return !feof(f);
        }

    private:
        FILE *f = nullptr;
        bool bCloseOnExit = false;
    };

}

#endif //GNILK_FILEREADER_H
