#pragma once
#include "Scene.h"
#include "ThirdPersonCamera.h"
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
		case 'W':
			zTrans -= 0.1f;
			mCamera->MoveForward(-0.1f);
			break;
		case 'S':
			zTrans += 0.1f;
			mCamera->MoveForward(0.1f);
			break;
		case 'A':
			xTrans += 0.1f;
			mCamera->MoveSide(0.1f);
			break;
		case 'D':
			xTrans -= 0.1f;
			mCamera->MoveSide(-0.1f);
			break;

		case VK_LEFT:
			yAngle += 0.1f;
			mCamera->Rotate(D3DXVECTOR3(3.f, 0, 0));
			break;
		case VK_RIGHT:
			yAngle -= 0.1f;
			mCamera->Rotate(D3DXVECTOR3(-3.f, 0, 0));
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
	Ground				*mGround;
	ThirdPersonCamera	*mCamera;
	SkinnedMesh			*mSkinnedMesh;
	SkinnedMesh			*mSkinnedMesh_2;
	float				xTrans;
	float				zTrans;
	float				yAngle;
};

