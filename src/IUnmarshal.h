//
// Created by gnilk on 18.12.2025.
//

#ifndef GNILK_IUNMARSHAL_H
#define GNILK_IUNMARSHAL_H

#include <string>

namespace gnilk {
    class IUnmarshal {
    public:

    public:
        virtual ~IUnmarshal() = default;

        virtual bool SetField(const std::string &fieldName, const std::string &fieldValue) = 0;
        virtual IUnmarshal *GetUnmarshalForField(const std::string &fieldName) = 0;
        virtual bool PushToArray(const std::string &arrayName, IUnmarshal *pData) = 0;
    };

    class BaseUnmarshal : public IUnmarshal {
    public:
        virtual ~BaseUnmarshal() = default;
        bool SetField(const std::string &fieldName, const std::string &fieldValue) override { return false; }
        IUnmarshal *GetUnmarshalForField(const std::string &fieldName) override {return nullptr; }
        bool PushToArray(const std::string &arrayName, IUnmarshal *pData) override {return false; }
    };
}

#endif //GNILK_IUNMARSHAL_H
