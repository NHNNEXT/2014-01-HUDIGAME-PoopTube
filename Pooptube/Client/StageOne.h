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
	class Sprite;
}

class MainCharacter;
class Creature;
class LightOrb;


class StageOne
	: public pooptube::Scene
{
public:
	StageOne();
	~StageOne();

	bool Init();

	static StageOne* Create();

	void Render();

	void Update(float dTime);
	void UpdateInput();

	//아래 둘을 어디로 옮길지 생각중
	void MainCharacterJumpUpdate(float dTime);

	pooptube::Ground *GetGroundModule() const { return mGround; }

protected:

private:
	pooptube::ThirdPersonCamera		*mCamera = nullptr;
	MainCharacter					*mCharacter = nullptr;
	pooptube::Ground				*mGround = nullptr;
	pooptube::SkyBox				*mSkyBox = nullptr;
	pooptube::Light					*mLight = nullptr;
	pooptube::SunLight				*mSunLight = nullptr;
	pooptube::XMesh					*mXMesh = nullptr;

	Creature						*mCreature = nullptr; // 크리처
	LightOrb						*mLightOrb = nullptr; // 환경 물체
	pooptube::Sprite				*mSprite = nullptr; // sprite

	float				mTimeForFPS = 0.f;
	float				mTimeForJump = 0.f;
	float				mBeforeJumpYPos = 0.f;
	bool				mRecordJumpPos = false;

	pooptube::CollisionBox	*testDummy;
};