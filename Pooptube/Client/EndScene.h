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

class EndScene
	: public pooptube::Scene
{
public:
	EndScene();
	~EndScene();

	virtual bool Init();

	static EndScene* Create();

	virtual void Render();

	virtual void Update(float dTime);
	virtual void UpdateInput();

protected:

private:
	MainCharacter					*mCharacter = nullptr;
	Tree3							*mTree = nullptr;
	LightOrb						*mLightOrb[4];
	pooptube::ThirdPersonCamera		*mCamera = nullptr;
	pooptube::SkyBox				*mSkyBox = nullptr;
	pooptube::Light					*mSpotLight = nullptr;
	pooptube::XMesh					*mXMesh = nullptr;
	pooptube::BillBoard				*mBoard;
	pooptube::Sprite				*mSprite = nullptr;

	float				lightEnhanceTime = 0.f;
	float				mTimeForCircle = 0.f;
	float				mTimeForFPS = 0.f;
	float				mTimeForJump = 0.f;
	float				mBeforeJumpYPos = 0.f;
	bool				mRecordJumpPos = false;
	//bool				mIsCleared = false;
	//float				mCreatureAttackTime = 0.f;
	//int					mTotalDamage = 0;

	pooptube::CollisionBox	*testDummy;
};