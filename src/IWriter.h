//
// Created by gnilk on 28.08.2025.
//

#ifndef EW_TRACKER_IWRITER_H
#define EW_TRACKER_IWRITER_H

#include <memory>
#include <stdlib.h>
#include <stdint.h>

namespace gnilk {
    class IWriter {
    public:
        using Ref = std::shared_ptr<IWriter>;
    public:
        virtual ~IWriter() = default;
        // Returns number of bytes written or -1 on failure
        virtual int32_t Write(const void *data, size_t nbytes) = 0;
        // Return number of bytes written or -1 on failure
        virtual int32_t Flush() = 0;
    };

    // Use this for default implementations
    class BaseWriter : public IWriter {
    public:
        BaseWriter() = default;
        virtual ~BaseWriter() = default;
        int32_t Write(const void *data, size_t nbytes) override {
            // Zero bytes written
            return 0;
        }
        int32_t Flush() override {
            // Zero bytes flushed
            return 0;
        }

    };
}


#endif //EW_TRACKER_IWRITER_H
