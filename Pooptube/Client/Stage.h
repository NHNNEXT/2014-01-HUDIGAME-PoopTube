#pragma once
#include "Scene.h"
#include "JsonParser.h"

using namespace pooptube;

namespace pooptube {
	class Sprite;
	class SkyBox;
}

class MainCharacter;
class LightOrb;
class Creature;

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
	std::string			mFileName; // 테스트용 : 리로드 가능
	void				_LoadFile( std::string filename );
	void				_LoadGround( Json::Value& jsonData );
	void				_LoadCamera( Json::Value& jsonData );
	void				_LoadSunLight( Json::Value& jsonData );
	void				_LoadCreature( Json::Value& jsonData );
	void				_LoadLightOrb( Json::Value& jsonData );
	void				_LoadBillBoard( Json::Value& jsonData );
	void				_LoadTree1( Json::Value& jsonData );
	void				_LoadTree2( Json::Value& jsonData );
	void				_LoadTree3( Json::Value& jsonData );
	void				_LoadNode( Json::Value& jsonData );
	void				_SetCommonData( Node* target, Json::Value& jsonData );

	int					mTotalDamage = 0;
	float				mCreatureAttackTime= 0.f;
	enum				{ mOrbCount = 12, };
	Sprite				*mClearPoint[mOrbCount];
	Sprite				*mYellow[mOrbCount];
	Sprite				*mPink[mOrbCount];
	
	SkyBox	*mSkyBox = nullptr;

	std::list<LightOrb*> mLightOrbList;
	std::list<Creature*> mCreatureList;
};

