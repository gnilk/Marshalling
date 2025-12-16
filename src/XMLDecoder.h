//
// Created by gnilk on 16.12.25.
//

#ifndef GNILK_XMLDECODER_H
#define GNILK_XMLDECODER_H

#include "IDecoder.h"
#include "XMLParser.h"

namespace gnilk {

    class XMLDecoder : public BaseDecoder {
    public:
        XMLDecoder() = default;
        virtual ~XMLDecoder() = default;

        void Begin(IReader::Ref incoming) override {
            // FIXME: Not supported..
        }

        void Begin(const std::string &xmldata);


        bool BeginObject(const std::string &name) override;
        bool HasObject(const std::string &name) override;
        void EndObject() override;

        std::optional<bool> ReadBoolField(const std::string &name) override;
        std::optional<int> ReadIntField(const std::string &name) override;
        std::optional<int64_t> ReadInt64Field(const std::string &name) override;
        std::optional<float> ReadFloatField(const std::string &name) override;
        std::optional<std::string> ReadTextField(const std::string &name) override;
    protected:
        std::unique_ptr<xml::Document> doc;
        std::stack<xml::Tag::Ref> tagStack;
    };

}

#endif //GNILK_XMLDECODER_H
