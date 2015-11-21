#include <node.h>

using namespace v8;
using namespace node;

void GetString(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();
	args.GetReturnValue().Set(String::NewFromUtf8(isolate, "I'm from nw_simple_addon"));
}
	
void _Init(Local<Object> exports, Local<Object> module) {
	NODE_SET_METHOD(exports, "getString", GetString);
}

NODE_MODULE(main, _Init)


