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
	class Sprite;
}

class MainCharacter;
class Creature;
class LightOrb;
class Tree1;

class StageOne
	: public pooptube::Scene
{
public:
	StageOne();
	~StageOne();

	virtual bool Init();

	static StageOne* Create();

	virtual void Render();

	virtual void Update( float dTime );
	virtual void UpdateInput();

protected:

private:
	pooptube::ThirdPersonCamera		*mCamera = nullptr;
	MainCharacter					*mCharacter = nullptr;
	pooptube::SkyBox				*mSkyBox = nullptr;
	pooptube::Light					*mLight = nullptr;
	pooptube::SunLight				*mSunLight = nullptr;
	pooptube::XMesh					*mXMesh = nullptr;
	Tree1							*mTree = nullptr;

	Creature						*mCreature = nullptr; // 크리처
	LightOrb						*mLightOrb = nullptr; // 환경 물체
	pooptube::Sprite				*mSprite = nullptr; // sprite

	float				mTimeForFPS = 0.f;
	float				mTimeForJump = 0.f;
	float				mBeforeJumpYPos = 0.f;
	bool				mRecordJumpPos = false;

	pooptube::CollisionBox	*testDummy;
};