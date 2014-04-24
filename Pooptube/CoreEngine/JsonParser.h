#pragma once

#include "stdafx.h"
#include "../Library/JsonCpp/include/json/json.h"	// agebreak : 경로는 속성에서 넣고, 여기서는 그냥 헤더 파일만 있는게 좋음

namespace pooptube {
	class JsonParser {
	public:
		JsonParser(char* path);
		JsonParser();

		~JsonParser();
	public:
		
	private:
		static JsonParser* GetInstance(char* path);
		static JsonParser* GetInstance();
		bool ReadFromFile(const char* filename, char* buffer, int len);
		void JsonToCString();

	private:
		char* mJsonFilePath;		// agebreak : 과연 이 변수는 안전할까??
		char mPath[1024];
	};
}