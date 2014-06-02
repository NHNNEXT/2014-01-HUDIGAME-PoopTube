#pragma once
#include "Scene.h"
#include "JsonParser.h"

using namespace pooptube;

class MainCharacter;

class Stage :
	public Scene
{
public:
	Stage();
	virtual ~Stage();

	static Stage* Create( std::string filename );
	virtual bool Init( std::string filename );

	virtual void Render();

	virtual void Update( float dTime );
	virtual void UpdateInput();

private:
	float				mTimeForFPS = 0.f;

	MainCharacter		*mCharacter = nullptr;

	void				_LoadGround( Json::Value& jsonData );
	void				_LoadCamera( Json::Value& jsonData );
	void				_LoadSunLight( Json::Value& jsonData );
	void				_LoadCreature( Json::Value& jsonData );
	void				_LoadLightOrb( Json::Value& jsonData );
	void				_LoadBillBoard( Json::Value& jsonData );
	void				_LoadTree( Json::Value& jsonData );
	void				_LoadNode( Json::Value& jsonData );
	void				_SetCommonData( Node* target, Json::Value& jsonData );
};

