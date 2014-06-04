#pragma once
#include "stdafx.h"
#include "Scene.h"

namespace pooptube {
	class ThirdPersonCamera;
	class Camera;
	class SkinnedMesh;
	class CollisionBox;
	class SkyBox;
	class Light;
	class SunLight;
	class XMesh;
	class Ground;
	class BillBoard;
	class Sprite;
}

class MainCharacter;
class Creature;
class LightOrb;
class Tree3;

class IntroScene
	: public pooptube::Scene
{
public:
	IntroScene();
	~IntroScene();

	virtual bool Init();

	static IntroScene* Create();

	virtual void Render();

	virtual void Update(float dTime);
	virtual void UpdateInput();

protected:

private:
	MainCharacter					*mCharacter = nullptr;
	Tree3							*mTree[50][50];
	LightOrb						*mLightOrb[3];
	pooptube::ThirdPersonCamera		*mCamera = nullptr;
	pooptube::SkyBox				*mSkyBox = nullptr;
	pooptube::SunLight				*mSunLight = nullptr;
	pooptube::XMesh					*mXMesh = nullptr;
	pooptube::BillBoard				*mBoard[3];
	Creature						*mCreature = nullptr;
 	pooptube::Sprite				*mClearPoint[3];
	pooptube::Sprite				*mYellow[3];
 	pooptube::Sprite				*mPink[3];
	
	float				mTimeForFPS = 0.f;
	float				mTimeForJump = 0.f;
	float				mBeforeJumpYPos = 0.f;
	bool				mRecordJumpPos = false;
	bool				mIsCleared = false;
	float				mCreatureAttackTime = 0.f;
	int					mTotalDamage = 0;

	pooptube::CollisionBox	*testDummy;
};