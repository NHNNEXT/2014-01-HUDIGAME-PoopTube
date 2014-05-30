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
class Tree;

class IntroScene
	: public pooptube::Scene
{
public:
	IntroScene();
	~IntroScene();

	bool Init();

	static IntroScene* Create();

	void Render();

	void Update(float dTime);
	void UpdateInput();

	void MainCharacterJumpUpdate(float dTime);
	//아래 둘을 어디로 옮길지 생각중

	pooptube::Ground *GetGroundModule() const { return mGround; }

protected:

private:
	MainCharacter					*mCharacter = nullptr;
	Tree							*mTree[9][9];
	LightOrb						*mLightOrb1 = nullptr;
	LightOrb						*mLightOrb2 = nullptr;
	LightOrb						*mLightOrb3 = nullptr;
	pooptube::ThirdPersonCamera		*mCamera = nullptr;
	pooptube::Ground				*mGround = nullptr;
	pooptube::SkyBox				*mSkyBox = nullptr;
	pooptube::SunLight				*mSunLight = nullptr;
	pooptube::XMesh					*mXMesh = nullptr;
	pooptube::BillBoard				*mBoard = nullptr;

	float				mTime = 0.f;
	float				mTimeForFPS = 0.f;
	float				mTimeForJump = 0.f;
	float				mBeforeJumpYPos = 0.f;
	bool				mRecordJumpPos = false;
	bool				mIsCleared = false;

	pooptube::CollisionBox	*testDummy;
};