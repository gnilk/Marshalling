//
// Created by gnilk on 17.09.25.
//

#ifndef GNILK_ISERIALIZABLE_H
#define GNILK_ISERIALIZABLE_H

#include "IEncoder.h"

namespace gnilk {
    // Defines a class supporting serialization
    class ISerializable {
    public:
        virtual ~ISerializable() = default;
        virtual void SerializeTo(IEncoder &encoder) = 0;
    };
}

#endif //GNILK_ISERIALIZABLE_H
