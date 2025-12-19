# Marshalling library
Currently Supports:
* XML, works fine (not sure about lists)
* JSON, parsing is fine but deserialization and unmarshalling has flaws
* INI, works fine

Error reporting is pretty sparse...

Quick start: look at the unmarshalling unit tests and or decoder tests.

# Build
Add the subdirectory to your project and link with the library.
Or just dump the src direction somewhere...

# Testing
You need the test-runner (see: https://github.com/gnilk/testrunner) in order to run unit tests.

# Serialization
Serialization is easy enough to understand, you just need to implement `ISerialize` in your data class and it can be 
used to encode. You might want to add a specialization to the JSON encoder to support 'attributes' from XML. I will
probably do that - but it has not been done yet.

# Deserialization
This library supports two kinds of deserialization. One is based on callback's through the `IUnmarshal` interface. 
You implement this interface (or inherit from `BaseUnmarshal`) and then you call the decoder function `decoder::Unmarshal()`.
The decoder will then traverse the document and call you for everything. See the unit testing of unmarshalling for examples
(`test_xmlunmarshalling.cpp`, `test_jsonumarshal.cpp`, `test_iniunmarshal.cpp`).

The other type of unmarshalling is based on you deciding what you want to do. Basically you call in to the API and it will try
to find what you are looking for...  This might be easier to quick & dirty hacks but for anything more complicated it quickly
becomes a lot of boilerplate dreadful code..

## Callback based deserialization / unmarshalling
Callback based deserialization is much more suited for complex hierarchies. The main drawback is that your class definition must
implement a specific interface (or inherit from a baseclass).

Example:
```c++
class MyObject : public BaseUnmarshal {
public:
    MyObject() = default;
    virtual ~MyObject() = default;
public: // BaseUnmarshal
    bool SetField(const std::string &fieldName, const std::string &fieldValue) override {
        if (fieldName == "field") {
            value = *convert_to<int>(fieldValue);
            return true;
        }
        return false;
    }
    IUnmarshal *GetUnmarshalForField(const std::string &fieldName) override {
        if (fieldName == "Object") {
            subObject = new MyObject();
            return subObject;
        }
        return nullptr;
    }
public:
    int value = 0;
    MyObject *subObject = {};
};

int main(int argc, char **argv) {
    std::string data = "{ \"field\" : 123, \"Object\" : { \"field\" : 345 } }";
    
    JSONDecoder decoder(data);
    MyObject root;
    decoder.Unmarshal(&root);
    
    return 0;
}
```

## API Based deserialization

For lack of better name - when you call into the decoder and request specific data.
For quick stuff, this get's your running pretty fast but beware that it can become hairy quite quick.

Note: You don't need to implement the 'IDeserialize' interface.

Example:
```c++
struct MyDataObject : public IDserialize {
    int someNumber;
    void DeserializeFrom(IDecoder &decoder) {
        // Data does not contain my object
        if (!decoder.BeginObject("MyDataObject")) return;
        auto optIntValue = decoder.ReadIntField("someNumber");
        if (optIntValue.has_value()) {
            someNumber = *optIntValue;
        } else {
            // handle if someNumber is missing
        }
        EndObject();
    }
};
```

This will consume an XML looking like:
```xml
<MyDataObject someNumber="2" />
```

To check if an object has a child present:
```xml
<MyDataObject>
    <MyChild>
        
    </MyChild>
</MyDataObject>
```

You would need to declare two classes/structs (basically you don't need to - but this is probably the better way).
```c++
struct MyDataObject : public IDserialize {
    void DeserializeFrom(IDecoder &decoder) {
        // Data does not contain my object
        if (!decoder.BeginObject("MyDataObject")) return;
        if (decoder.HasObject("MyChild")) {
            child.DeserializeFrom(decoder);
        }
        EndObject();
    }
}

struct MyChildObject : public IDeserialize {
    void DeserializeFrom(IDecoder &decoder) {
        // Data does not contain my object
        if (!decoder.BeginObject("MyChild")) return;
        EndObject();
    }
    
};
```

# About the code
The code has a few simple interfaces to abstract away reading of data from various sources.
It is fairly easy to strip this out or localize it if you want. Also, there are very few external dependencies except the
I/O interfaces.

## Parsers and Decoders
Parsers well parse the data and build a format specific structure. The decoders will translate the structure to
a common API and/or unmarshalling interface.

You should first and foremost work with the decoders.

Using unmarshalling you can rewrite the parser to do callback's on the fly instead of first building the structure up.
This is the main use-case from where these parser/decoders where originally used. 