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
	LightOrb						*mLightOrb1 = nullptr;
	LightOrb						*mLightOrb2 = nullptr;
	LightOrb						*mLightOrb3 = nullptr;
	pooptube::ThirdPersonCamera		*mCamera = nullptr;
	pooptube::SkyBox				*mSkyBox = nullptr;
	pooptube::SunLight				*mSunLight = nullptr;
	pooptube::XMesh					*mXMesh = nullptr;
	pooptube::BillBoard				*mBoard = nullptr;
	Creature						*mCreature = nullptr;

	float				mTime = 0.f;
	float				mTimeForFPS = 0.f;
	float				mTimeForJump = 0.f;
	float				mBeforeJumpYPos = 0.f;
	bool				mRecordJumpPos = false;
	bool				mIsCleared = false;

	pooptube::CollisionBox	*testDummy;
};