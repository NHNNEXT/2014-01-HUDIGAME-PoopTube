#pragma once

#include "stdafx.h"
#include "../Library/JsonCpp/include/json/json.h"	// agebreak : ��δ� �Ӽ����� �ְ�, ���⼭�� �׳� ��� ���ϸ� �ִ°� ����

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
		char* mJsonFilePath;		// agebreak : ���� �� ������ �����ұ�??
		char mPath[1024];
	};
}