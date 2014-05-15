#pragma once

#include "stdafx.h"
#include "..\Library\JsonCpp\json.h" // 왜인지 아무리 프로젝트 설정을 만져도 제대로 include 경로 참조가 되질 않는다...

namespace pooptube {
	namespace JsonParser {
		std::string		JsonToString	( Json::Value& src );
		Json::Value		StringToJson	( std::string& src );
		std::string		FileToString	( std::string& filename );
		Json::Value		FileToJson		( std::string& filename );
		bool			StringToFile	( std::string& src, std::string& filename );
		bool			JsonToFile		( Json::Value& src, std::string& filename );
	};
}