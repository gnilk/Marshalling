//
// Created by gnilk on 17.09.25.
//

#ifndef GNILK_IENCODER_H
#define GNILK_IENCODER_H

#include <memory>
#include <vector>
#include <utility>
#include <algorithm>
#include <variant>
#include <stdint.h>
#include "IWriter.h"

namespace gnilk {

    class IEncoder {
    public:
        using Ref = std::shared_ptr<IEncoder>;
        enum class kFeature {
            kSupportsAttributes,
        };

    public:
        virtual ~IEncoder() = default;

        virtual void Begin(IWriter::Ref out) = 0;

        virtual void BeginObject(const std::string &name) = 0;
        virtual void EndObject() = 0;
        virtual void WriteBoolField(const std::string &name, bool value) = 0;
        virtual void WriteIntField(const std::string &name, int value) = 0;
        virtual void WriteInt64Field(const std::string &name, int64_t value) = 0;
        virtual void WriteFloatField(const std::string &name, double value) = 0;
        virtual void WriteTextField(const std::string &name, const std::string &value) = 0;

        virtual IWriter::Ref GetWriter() = 0;
        virtual bool IsFeatureSupported(kFeature feature) = 0;
    };

    struct EncoderObjectAttribute {
        std::string name;
        std::variant<std::string, int, double> value;

        static std::string ToString(const EncoderObjectAttribute &attr) {
            const int *pInt = std::get_if<int>(&attr.value);
            if (pInt) {
                return std::to_string(*pInt);
            }
            const double *pDouble = std::get_if<double>(&attr.value);
            if (pDouble) {
                return std::to_string(*pDouble);
            }
            return std::get<std::string>(attr.value);
        }
    };

    class IEncoderWithAttributes : public IEncoder {
    public:
        virtual ~IEncoderWithAttributes() = default;

        virtual void BeginObject(const std::string &name) = 0;
        virtual void BeginObject(const std::string &name, const std::vector<EncoderObjectAttribute > &&attributes) = 0;
        virtual void SingleObject(const std::string &name, const std::vector<EncoderObjectAttribute > &&attributes) = 0;

        virtual void BeginObject(const std::string &name, const std::vector<EncoderObjectAttribute > &attributes) = 0;
        virtual void SingleObject(const std::string &name, const std::vector<EncoderObjectAttribute > &attributes) = 0;

    };

    class BaseEncoder : public IEncoderWithAttributes {
    public:
        virtual ~BaseEncoder() = default;

        void Begin(IWriter::Ref out) override {
            writer = out;
        };

        // skeleton...
        void BeginObject(const std::string &name) override {}
        void EndObject() override {}
        void WriteBoolField(const std::string &name, bool value) override {}
        void WriteIntField(const std::string &name, int value) override {}
        void WriteInt64Field(const std::string &name, int64_t value) override {}
        void WriteFloatField(const std::string &name, double value) override {}
        void WriteTextField(const std::string &name, const std::string &value) override {}

        // From EncoderWithAttributes
        void BeginObject(const std::string &name, const std::vector<EncoderObjectAttribute > &&attributes) override {}
        void SingleObject(const std::string &name, const std::vector<EncoderObjectAttribute > &&attributes) override {}

        void BeginObject(const std::string &name, const std::vector<EncoderObjectAttribute > &attributes) override {};
        void SingleObject(const std::string &name, const std::vector<EncoderObjectAttribute > &attributes) override {};



        bool IsFeatureSupported(kFeature feature) override {
            auto it = std::find_if(supportedFeatures.begin(), supportedFeatures.end(), [&feature](const kFeature &other) { return feature == other; });
            return !(it == supportedFeatures.end());
        }

        IWriter::Ref GetWriter() override {
            return writer;
        }
    protected:
        void SupportFeature(kFeature newFeature) {
            supportedFeatures.push_back(newFeature);
        }
    protected:
        IWriter::Ref writer;
        std::vector<kFeature> supportedFeatures;
    };
}

#endif //GNILK_IENCODER_H
