//
// Created by gnilk on 17.09.25.
//

#ifndef GNILK_XMLENCODER_H
#define GNILK_XMLENCODER_H

#include <vector>
#include <variant>
#include "StringWriter.h"
#include "IEncoder.h"

namespace gnilk {

    class XMLEncoder : public BaseEncoder {
    public:
        explicit XMLEncoder(IWriter::Ref outStream) : ss(outStream) {
            SupportFeature(IEncoder::kFeature::kSupportsAttributes);
            BaseEncoder::Begin(outStream);
        }
        virtual ~XMLEncoder() = default;

        void PrettyPrint(bool use);
        void WriteEnvelopeOnFirstObject(bool use);

        void BeginObject(const std::string &name) override;
        // Specialized
        void BeginObject(const std::string &name, const std::vector<EncoderObjectAttribute > &&attributes) override;
        void SingleObject(const std::string &name, const std::vector<EncoderObjectAttribute > &&attributes) override;

        void BeginObject(const std::string &name, const std::vector<EncoderObjectAttribute > &attributes) override;
        void SingleObject(const std::string &name, const std::vector<EncoderObjectAttribute > &attributes) override;

        void EndObject() override;

        void WriteBoolField(const std::string &name, bool value) override;
        void WriteIntField(const std::string &name, int value) override;
        void WriteInt64Field(const std::string &name, int64_t value) override;
        void WriteFloatField(const std::string &name, double value) override;
        void WriteTextField(const std::string &name, const std::string &value) override;

    protected:
        void BeginObjectImpl(const std::string &name, const std::vector<EncoderObjectAttribute > &attributes);
        void SingleObjectImpl(const std::string &name, const std::vector<EncoderObjectAttribute > &attributes);

        void WriteEnvelope();
    private:
        bool pretty = false;
        bool writeEnvelopeOnFirstObject = false;
        std::string eol = "";
        StringWriter ss;
        std::vector<std::string> objectStack;
        std::vector<int> fieldStack;
        int fieldCount;


    };
}


#endif //GNILK_XMLENCODER_H
