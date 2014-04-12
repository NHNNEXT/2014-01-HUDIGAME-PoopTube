#pragma once

#include "stdafx.h"
#include "../Library/JsonCpp/include/json/json.h"

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

	private:
		//Json::Value mRoot;
		const char* mPath;
	};
}