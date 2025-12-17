//
// Created by gnilk on 12/12/2022.
//

#ifndef GNILK_INIENCODER_H
#define GNILK_INIENCODER_H

#include <string>
#include <memory>

#include "IEncoder.h"
#include "StringWriter.h"
#include "IWriter.h"

namespace gnilk {
    class IniEncoder : public BaseEncoder {
    public:
        using Ref = std::shared_ptr<IniEncoder>;
    public:
        IniEncoder() = default;
        explicit IniEncoder(IWriter::Ref p_stream) : ss(p_stream) {

        }
        virtual ~IniEncoder() = default;

        static Ref Create(IWriter::Ref out) {
            return std::make_shared<IniEncoder>(out);
        }

        // This is an optional call to set the writer
        void Begin(IWriter::Ref out) override;

        // This is used by the messages
        void BeginObject(const std::string &name) override;
        void EndObject() override;

        // Note: 'hasNext' is deprecated, parameter ignored
        void WriteBoolField(const std::string &name, bool value) override;
        void WriteIntField(const std::string &name, int value) override;
        void WriteInt64Field(const std::string &name, int64_t value) override;
        void WriteFloatField(const std::string &name, double value) override;
        void WriteTextField(const std::string &name, const std::string &value) override;

        virtual IWriter::Ref GetWriter() override { return {}; }
        virtual bool IsFeatureSupported(kFeature feature) override {  return false; }

//        void BeginArray(const char *name, IEncoder::kArrayTypeSpec typeSpec) override;
//        void EndArray(bool hasNext = false) override;

    private:
        StringWriter ss;
    };
}


#endif //GNILK_INIENCODER_H
