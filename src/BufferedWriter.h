//
// Created by gnilk on 17.09.25.
//

#ifndef GNILK_BUFFEREDWRITER_H
#define GNILK_BUFFEREDWRITER_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <memory>
#include "IWriter.h"

namespace gnilk {
    class BufferedWriter : public BaseWriter {
    public:
        BufferedWriter() = delete;
        explicit BufferedWriter(IWriter *underlying) : writer(underlying) {

        }
        explicit  BufferedWriter(IWriter::Ref underlying) : writer(underlying) {

        }
        static IWriter::Ref Create(IWriter::Ref underlying) {
            return std::make_shared<BufferedWriter>(underlying);
        }

        virtual ~BufferedWriter() {
            if (ptrBuffer != nullptr) {
                free(ptrBuffer);
            }
        };

        int32_t Write(const void *data, size_t nbytes) override {
            if (!Ensure(nbytes)) {
                return -1;
            }
            memcpy(ptrBuffer + idxBuffer, data, nbytes);
            idxBuffer += nbytes;
            return nbytes;
        }

        int32_t Flush() override {
            auto result = writer->Write(ptrBuffer, idxBuffer);
            if (result >= 0) {
                idxBuffer = 0;
            }
            return result;
        }

    protected:
        bool Ensure(size_t nbytes) {
            // first time?
            if (ptrBuffer == nullptr) {
                // We allocate at least 1k for the buffered writer
                if (nbytes < 1024) {
                    nbytes = 1024;
                }
                ptrBuffer = static_cast<uint8_t *>(malloc(nbytes));
                if (ptrBuffer == nullptr) {
                    // out of memory
                    return false;
                }
                sizeBuffer = nbytes;
                idxBuffer = 0;
                return true;
            }
            // do we have space?
            if (nbytes < (sizeBuffer - idxBuffer)) {
                return true;
            }

            // allocate until we have enough...
            while (nbytes > (sizeBuffer - idxBuffer)) {
                auto newSize = sizeBuffer * 2;    // double the buffer size each time...
                auto newBuffer = static_cast<uint8_t *>(realloc(ptrBuffer, newSize));
                if (newBuffer == nullptr) {
                    return false;
                }
                sizeBuffer = newSize;
                ptrBuffer = newBuffer;
            }
            return true;
        }
    private:
        size_t idxBuffer = 0;
        size_t sizeBuffer = 0;
        uint8_t *ptrBuffer = nullptr;
        IWriter::Ref writer;
    };
}

#endif //GNILK_BUFFEREDWRITER_H
