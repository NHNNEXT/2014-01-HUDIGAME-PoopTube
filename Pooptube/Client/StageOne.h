#pragma once
#include "stdafx.h"
#include "Scene.h"

namespace pooptube {
	class ThirdPersonCamera;
	class Camera;
	class SkinnedMesh;
	class CollisionBox;
	class SkyBox;
}

class MainCharacter;

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

	void KeyDown(pooptube::KeyEvent* pKeyEvent);
	void KeyPressed(pooptube::KeyEvent* pKeyEvent);
	void KeyUp(pooptube::KeyEvent* pKeyEvent);
	void MouseDown(pooptube::MouseEvent* pMouseEvent);
	void MouseMove(pooptube::MouseEvent* pMouseEvent);
	void MouseUp(pooptube::MouseEvent* pMouseEvent);
	void MousePressed(pooptube::MouseEvent* pMouseEvent);
	void MouseWheel(pooptube::MouseEvent* pMouseEvent);

protected:

private:
	std::shared_ptr<pooptube::ThirdPersonCamera>	mCamera = nullptr;
	std::shared_ptr<pooptube::Camera>				mCamera_2 = nullptr;
	std::shared_ptr<MainCharacter>					mCharacter = nullptr;
	std::shared_ptr<pooptube::SkinnedMesh>			mSkinnedMesh = nullptr;
	std::shared_ptr<pooptube::SkinnedMesh>			mGround = nullptr;
	std::shared_ptr<pooptube::SkyBox>	mSkyBox = nullptr;

	float				mTimeForFPS = 0.f;

	std::shared_ptr<pooptube::CollisionBox>	testDummy;

	LPDIRECT3DDEVICE9	mDevice = nullptr;
};

