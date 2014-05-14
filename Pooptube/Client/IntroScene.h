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

	//아래 둘을 어디로 옮길지 생각중

	void KeyDown(pooptube::KeyEvent* pKeyEvent);
	void KeyPressed(pooptube::KeyEvent* pKeyEvent);
	pooptube::Ground *GetGroundModule() const { return mGround; }

protected:

private:
	MainCharacter					*mCharacter = nullptr;
	pooptube::ThirdPersonCamera		*mCamera = nullptr;
	pooptube::Ground				*mGround = nullptr;
	pooptube::SkyBox				*mSkyBox = nullptr;
	pooptube::Light					*mLight = nullptr;
	pooptube::SunLight				*mSunLight = nullptr;
	pooptube::XMesh					*mXMesh = nullptr;
};