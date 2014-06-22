#pragma once
#include "stdafx.h"
#include "Scene.h"

namespace pooptube {
	class Sprite;
}

class GameoverScene
	: public pooptube::Scene
{
public:
	GameoverScene();
	~GameoverScene();

	virtual bool Init();

	static GameoverScene* Create();

	virtual void Render();

	virtual void Update(float dTime);
	virtual void UpdateInput();

protected:

private:
	pooptube::Sprite	*mGameover = nullptr;
	float				mTime = 0.f;
};