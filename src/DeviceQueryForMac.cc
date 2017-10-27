#include <node.h>
#include <v8.h>
#include <iostream>

using namespace v8;
using namespace node;
using namespace std;

void Method(const v8::FunctionCallbackInfo<v8::Value>& args) {  
	Isolate* isolate = Isolate::GetCurrent();    

	// 参数长度判断 
	if (args.Length() < 1) { 
		isolate->ThrowException(Exception::TypeError( 
			String::NewFromUtf8(isolate, "Wrong number of arguments"))); 
		return; 
	} 
 
	// 参数类型判断 
	if (!args[0]->IsNumber()) {
		isolate->ThrowException(Exception::TypeError( 
			String::NewFromUtf8(isolate, "argumnets must be number"))); 
		return;
	} 
	 //是否加密,默认RSA加密
	//int type = args[0]->NumberValue();
	
	args.GetReturnValue().Set(String::NewFromUtf8(isolate, "3208AE7BF5F07AD47C009C8FAFE653827D97087EA0DE14DECF4D6D095D0C1B88BACCDEB70AFB33C1B68E1A8B795314ACA6F71D7FBC9F4A76CA6D14A6ADA7539E2958027EB878A335F4A823997614DD10C7A2C01D7CEEAF996050C43283D3E1BD1641A923D23EF25D9FC7301D447500FD6ABDAC1D7AC024A759FAC1576C5D1A7D"));
}
  
void init(Handle<Object> exports) {  
  NODE_SET_METHOD(exports, "GetDC", Method);
}  
  
NODE_MODULE(DeviceQuery, init)