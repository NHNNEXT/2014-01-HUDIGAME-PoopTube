#include "stdafx.h"
#include "JsonParser.h"

namespace pooptube {
	std::string JsonParser::JsonToString( Json::Value& src )
	{
		Json::StyledWriter writer;
		return writer.write( src );
	}

	Json::Value JsonParser::StringToJson( std::string& src )
	{
		Json::Reader reader;
		Json::Value result;
		reader.parse( src, result );
		return result;
	}

	std::string JsonParser::FileToString( std::string& filename )
	{
		FILE *fStream = nullptr;
		if( fopen_s( &fStream, filename.c_str(), "r+t" ) != 0 )
			return nullptr;
		
		std::string result("");
		char buffer[20] = { '\0', };
		while( fread_s( buffer, 20, sizeof(char), 10, fStream ) != 0 ){
			result += buffer;
		}
		fclose( fStream );
		return result;
	}

	Json::Value JsonParser::FileToJson( std::string& filename )
	{
		std::string jsonStr = FileToString( filename );
		return StringToJson( jsonStr );
	}

	bool JsonParser::StringToFile( std::string& src, std::string& filename )
	{
		FILE *fStream;
		if( fopen_s( &fStream, filename.c_str(), "w+t" ) != 0 )
			return 1;

		fwrite( src.c_str(), src.size(), 1, fStream );
		fclose( fStream );
		return 0;
	}

	bool JsonParser::JsonToFile( Json::Value& src, std::string& filename )
	{
		std::string jsonStr = JsonToString( src );
		return StringToFile( jsonStr, filename );
	}

}
