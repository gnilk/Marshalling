//
// Created by gnilk on 17.09.25.
//

#ifndef GNILK_FILEWRITER_H
#define GNILK_FILEWRITER_H

#include <stdio.h>
#include <memory>

#include "IWriter.h"


namespace gnilk {
    // This just wraps write access to an open file - IT DOES NOT CLOSE in the DTOR!!!
    class FileWriter : public IWriter {
    public:
        FileWriter() = delete;
        explicit FileWriter(FILE *file, bool closeOnExit = false) : f(file), bCloseOnExit(closeOnExit){

        }

        static IWriter::Ref Create(FILE *f, bool closeOnExit = false) {
            return std::make_shared<FileWriter>(f, closeOnExit);
        }

        virtual ~FileWriter() {
            if (bCloseOnExit) {
                fclose(f);
            }
        };

        int32_t Write(const void *data, size_t nbytes) override{
            if (f == nullptr) {
                return -1;
            }
            return fwrite(data, 1, nbytes, f);
        }
        int32_t Flush() override {
            if (f == nullptr) {
                return -1;
            }
            return fflush(f);
        }

    private:
        FILE *f = nullptr;
        bool bCloseOnExit = false;

    };
}

#endif //GNILK_FILEWRITER_H
