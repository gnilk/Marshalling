//
// Created by gnilk on 16.12.25.
//

#ifndef GNILK_IDESERIALIZABLE_H
#define GNILK_IDESERIALIZABLE_H

#include "IDecoder.h"

namespace gnilk {
    // Defines a class supporting serialization
    class IDeserializable {
    public:
        virtual ~IDeserializable() = default;
        virtual void DeserializeFrom(IDecoder &decoder) = 0;
    };

}

#endif //GNILK_IDESERIALIZABLE_H
