//
// Created by gnilk on 17.12.2025.
//

#ifndef GNILK_JSONDECODER_H
#define GNILK_JSONDECODER_H

#include "IDecoder.h"
#include "JSONParser.h"

namespace gnilk {
    class JSONDecoder : public BaseDecoder {
    public:
        JSONDecoder() = default;
        virtual ~JSONDecoder() = default;

        void Begin(IReader::Ref incoming) override;
        void Begin(const std::string &jsondata);


        bool BeginObject(const std::string &name) override;
        bool HasObject(const std::string &name) override;
        void EndObject() override;

        std::optional<bool> ReadBoolField(const std::string &name) override;
        std::optional<int> ReadIntField(const std::string &name) override;
        std::optional<int64_t> ReadInt64Field(const std::string &name) override;
        std::optional<float> ReadFloatField(const std::string &name) override;
        std::optional<std::string> ReadTextField(const std::string &name) override;

    protected:
        void Initialize();
    protected:
        std::unique_ptr<JSONDoc> doc;
        // Only objects for now - arrays will come later...
        std::stack<JSONObject::Ref> objStack;


    };
}


#endif //ENCDEC_JSONDECODER_H
