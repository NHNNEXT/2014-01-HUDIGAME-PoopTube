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

	void TEST(float x, float y);
	D3DXVECTOR2 PICK(float x, float y);


	//아래 둘을 어디로 옮길지 생각중
	void MainCharacterJumpUpdate(float dTime);

	void KeyDown(pooptube::KeyEvent* pKeyEvent);
	void KeyPressed(pooptube::KeyEvent* pKeyEvent);
	void KeyUp(pooptube::KeyEvent* pKeyEvent);
	void MouseDown(pooptube::MouseEvent* pMouseEvent);
	void MouseMove(pooptube::MouseEvent* pMouseEvent);
	void MouseUp(pooptube::MouseEvent* pMouseEvent);
	void MousePressed(pooptube::MouseEvent* pMouseEvent);
	void MouseWheel(pooptube::MouseEvent* pMouseEvent);
	pooptube::Ground *GetGroundModule() const { return mGround; }

protected:

private:
	pooptube::ThirdPersonCamera		*mCamera = nullptr;
	MainCharacter					*mCharacter = nullptr;
	pooptube::SkinnedMesh			*mSkinnedMesh = nullptr;
	pooptube::Ground				*mGround = nullptr;
	pooptube::SkyBox				*mSkyBox = nullptr;
	pooptube::Light					*mLight = nullptr;
	pooptube::SunLight				*mSunLight = nullptr;
	pooptube::XMesh					*mXMesh = nullptr;

	Creature						*mCreature = nullptr; // 크리처
	LightOrb						*mLightOrb = nullptr; // 환경 물체

	float				mTimeForFPS = 0.f;
	float				mTimeForJump = 0.f;
	float				mBeforeJumpYPos = 0.f;
	bool				mRecordJumpPos = false;

	pooptube::CollisionBox	*testDummy;
};