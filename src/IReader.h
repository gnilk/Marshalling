//
// Created by gnilk on 28.08.2025.
//

#ifndef GNILK_IREADER_H
#define GNILK_IREADER_H

#include <stdlib.h>
#include <stdint.h>
#include <memory>

namespace gnilk {
    // Core interfaces
    class IReader {
    public:
        using Ref = std::shared_ptr<IReader>;
    public:
        virtual ~IReader() = default;
        virtual int32_t Read(void *out, size_t maxbytes) = 0;
        virtual bool Available() = 0;
        virtual IReader *GetUnderlyingReader() { return nullptr; };
    };
}

#endif //GNILK_IREADER_H
