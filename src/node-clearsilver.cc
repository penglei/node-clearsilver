#include "clearsilver-wrap.h"

void init(v8::Handle<v8::Object> target) {
    //Handle<FunctionTemplate> csConstructor = ClearSilver::Init(target);
    ClearSilver::Init(target);
    //csConstructor->Set(v8::String::New("parseCS"), FunctionTemplate::New(node_parseCS));
}

NODE_MODULE(clearsilver, init)

