#pragma once
#include "Scene.h"
#include "Camera.h"
#include "SkinnedMesh.h"
#include "Ground.h"

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

		mSkinnedMesh->Translation(xTrans, 0.f, zTrans);
		mSkinnedMesh->RotationY(yAngle);

		if (mSkinnedMesh)
			mSkinnedMesh->Render();

		if (mGround)
			mGround->Render();
//		if (mSkinnedMesh_2)
//			mSkinnedMesh_2->Render();

		mCamera->Render();
	}

	void Update(float dTime) {

	}

	void KeyDown(KeyEvent* pKeyEvent) {
		printf("KeyDown\n");
	}
	void KeyPressed(KeyEvent* pKeyEvent) {
		switch (pKeyEvent->GetKeyCode())
		{
		case VK_UP:
			zTrans -= 0.1f;
			break;
		case VK_DOWN:
			zTrans += 0.1f;
			break;
		case VK_RIGHT:
			xTrans -= 0.1f;
			break;
		case VK_LEFT:
			xTrans += 0.1f;
			break;
		case 'A':
			yAngle += 0.1f;
			break;
		case 'D':
			yAngle -= 0.1f;
			break;
		}

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
	Ground			*mGround;
	Camera			*mCamera;
	SkinnedMesh		*mSkinnedMesh;
	SkinnedMesh		*mSkinnedMesh_2;
	float			xTrans;
	float			zTrans;
	float			yAngle;
};

