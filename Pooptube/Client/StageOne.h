#pragma once
#include "Scene.h"
#include "Camera.h"
#include "SkinnedMesh.h"

using namespace pooptube;

class StageOne
	: public Scene
{
public:
	StageOne();
	~StageOne();

	bool Init();

	static StageOne* Create();

	void Render() {
		mCamera->Render();

		if (mSkinnedMesh)
			mSkinnedMesh->Render();
	}

	void Update(float dTime) {
	}

	void KeyDown(KeyEvent* pKeyEvent) {
		printf("KeyDown\n");
	}
	void KeyPressed(KeyEvent* pKeyEvent) {
		printf("KeyPressed\n");
	}
	void KeyUp(KeyEvent* pKeyEvent) {
		printf("KeyUp\n");
	}

	void MouseDown(MouseEvent* pMouseEvent) {
		printf("MouseDown\n");
	}

	void MouseMove(MouseEvent* pMouseEvent) {
		printf("MouseMove %d %d\n", pMouseEvent->GetX(), pMouseEvent->GetY());
	}

	void MouseUp(MouseEvent* pMouseEvent) {
		printf("MouseUp\n");
	}

	void MousePressed(MouseEvent* pMouseEvent) {
		printf("MousePressed\n");
	}

	void MouseWheel(MouseEvent* pMouseEvent) {
		printf("MouseWheel\n");
	}

	

protected:

private:
	Camera* mCamera;
	SkinnedMesh* mSkinnedMesh;
};

