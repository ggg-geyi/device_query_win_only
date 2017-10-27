#include <node.h>
#include <v8.h>
#include <iostream>
#include <ctime>
#include <windows.h>
#include <strsafe.h>
#include "WMI_DeviceQuery.h"
#include "RSA_EncryptData.h"

using namespace v8;
using namespace node;
using namespace std;
 
#define RSA_SIZE 512
#define PLAINTEXT_SIZE  RSA_SIZE/8
#define CIPHERTEXT_SIZE PLAINTEXT_SIZE*2

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
	int type = args[0]->NumberValue();
	
	T_DEVICE_PROPERTY properties;
	TCHAR info[PLAINTEXT_SIZE];
	TCHAR encInfo[CIPHERTEXT_SIZE*2+1];
	TCHAR encInfo2[CIPHERTEXT_SIZE+1];
	memset(info, 0, sizeof(info));
	memset(encInfo, 0, sizeof(encInfo));
	memset(encInfo2, 0, sizeof(encInfo2));
	//BIOS id
	if (WMI_DeviceQuery(4, &properties, 1)) {
		StringCchCopy(info, PLAINTEXT_SIZE, properties.szProperty);
		StringCchCat(info, PLAINTEXT_SIZE, TEXT(","));
	}  else {
		StringCchCopy(info, PLAINTEXT_SIZE, TEXT("None,"));
	}
	//CPU id
	if (WMI_DeviceQuery(3, &properties, 1)) {
		StringCchCat(info, PLAINTEXT_SIZE, properties.szProperty);
		StringCchCat(info, PLAINTEXT_SIZE, TEXT(","));
	}  else {
		StringCchCopy(info, PLAINTEXT_SIZE, TEXT("None,"));
	}
    	//Disk id
	if (WMI_DeviceQuery(1, &properties, 1)) {
		StringCchCat(info, PLAINTEXT_SIZE, properties.szProperty);
		//StringCchCat(info, PLAINTEXT_SIZE, TEXT(","));
	}  else {
		StringCchCopy(info, PLAINTEXT_SIZE, TEXT("None"));
	}
	cout << info << endl;

	TCHAR infoData[64];
	memset(infoData, 0, sizeof(infoData));
	//MainBoard id
	if (WMI_DeviceQuery(2, &properties, 1)) {
		StringCchCopy(infoData, PLAINTEXT_SIZE, properties.szProperty);
		StringCchCat(infoData, PLAINTEXT_SIZE, TEXT(","));
	}
	else {
		StringCchCopy(infoData, PLAINTEXT_SIZE, TEXT("None,"));
	}
	//Mac Address
	if (WMI_DeviceQuery(6, &properties, 1)) {
		StringCchCat(infoData, PLAINTEXT_SIZE, properties.szProperty);
		StringCchCat(infoData, PLAINTEXT_SIZE, TEXT(","));
	}
	else {
		StringCchCopy(infoData, PLAINTEXT_SIZE, TEXT("None,"));
	}
	TCHAR randData[PLAINTEXT_SIZE];
	sprintf(randData,"%I64d",time(0));
	StringCchCat(infoData, PLAINTEXT_SIZE, randData);

	if (my_rsa_entrypt((unsigned char *)info, (unsigned char *)encInfo) != 0) {
		args.GetReturnValue().Set(String::NewFromUtf8(isolate, "None"));
		return;
	}

	if (my_rsa_entrypt((unsigned char *)infoData, (unsigned char *)encInfo2) != 0) {
		args.GetReturnValue().Set(String::NewFromUtf8(isolate, "None"));
		return;
	}
	memcpy(encInfo + CIPHERTEXT_SIZE, encInfo2, CIPHERTEXT_SIZE);
	args.GetReturnValue().Set(String::NewFromUtf8(isolate, encInfo));
}
  
void init(Handle<Object> exports) {  
  NODE_SET_METHOD(exports, "GetDC", Method);
}  
  
NODE_MODULE(DeviceQuery, init)