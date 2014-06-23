#pragma once
#include "Scene.h"
#include "JsonParser.h"
#include "SoundManager.h"

using namespace pooptube;

namespace pooptube {
	class Sprite;
	class SkyBox;
	class Camera;
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
	void				_LoadFile( std::string& filename );
	void				_LoadToTarget( Node* target, Json::Value& jsonData );
	void				_LoadGround( Node* target, Json::Value& jsonData );
	void				_LoadCamera( Node* target, Json::Value& jsonData );
	void				_LoadSunLight( Node* target, Json::Value& jsonData );
	void				_LoadCreature( Node* target, Json::Value& jsonData );
	void				_LoadLightOrb( Node* target, Json::Value& jsonData );
	void				_LoadBillBoard( Node* target, Json::Value& jsonData );
	void				_LoadTree1( Node* target, Json::Value& jsonData );
	void				_LoadTree2( Node* target, Json::Value& jsonData );
	void				_LoadTree3( Node* target, Json::Value& jsonData );
	void				_LoadSoundBox( Node* target, Json::Value& jsonData );
	void				_LoadBgm( Node* target, Json::Value& jsonData );
	void				_LoadCollisionBox( Node* target, Json::Value& jsonData );
	void				_LoadNode( Node* target, Json::Value& jsonData );
	void				_SetCommonData( Node* target, Json::Value& jsonData );

	int					mTotalDamage = 0;
	float				mCreatureAttackTime= 0.f;
	float				mLightEnhanceTime = 0.f;
	enum				{ mOrbCount = 10, };
	Sprite				*mClearPoint[mOrbCount];
	Sprite				*mYellow[mOrbCount];
	Sprite				*mGameover;
	//Sprite				*mPink[mOrbCount];
	pooptube::Camera*	mCamera = nullptr;
	pooptube::SkyBox*	mSkyBox = nullptr;

	FMOD::Studio::EventInstance		*mBgm = nullptr;

	std::list<LightOrb*> mLightOrbList;
	std::list<Creature*> mCreatureList;
};

