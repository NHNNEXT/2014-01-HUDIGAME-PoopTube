#include "stdafx.h"
#include "JsonParser.h"

namespace pooptube {
	JsonParser::JsonParser(char* path) : mJsonFilePath(path)
	{
	}
	JsonParser::JsonParser() 
	{
	}
	bool JsonParser::ReadFromFile(const char* filename, char* buffer, int len) 
	{
		FILE* r = nullptr; // nullptr? NULL?
		fopen_s(&r, filename, "r");

		if (NULL == r)
			return false;;

		fread(buffer, 1, len, r);
		fclose(r);

		return true;
	}

	void JsonParser::JsonToCString()
	{
		FILE* jsonfp = nullptr;

		fopen_s(&jsonfp, mJsonFilePath, "r");

		const int BufferLength = 1024;
		char readBuffer[BufferLength] = { 0, };

		ReadFromFile(mJsonFilePath, readBuffer, BufferLength);

		std::string config_doc = readBuffer;

		Json::Value root;
		Json::Reader reader;

		bool parsingSuccessful = reader.parse(config_doc, root);

		if (!parsingSuccessful) {
			std::cout << "Parsing Fail" << std::endl << reader.getFormatedErrorMessages();
		} 
		
		const Json::Value resourceFilePath = root["path"];
		for (int index = 0; index < resourceFilePath.size(); ++index) {
			strcat_s(mPath, resourceFilePath[index].asCString());
			strcat_s(mPath, " ");
		}
		fclose(jsonfp);
	}
	
	JsonParser* JsonParser::GetInstance()
	{
		JsonParser* pInstance = new JsonParser();
		return pInstance;
	}
}
