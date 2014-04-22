#pragma once
#include "Scene.h"
#include "ThirdPersonCamera.h"
#include "SkinnedMesh.h"
#include "CollisionBox.h"

using namespace pooptube;

class StageOne
	: public Scene
{
public:
	StageOne();
	~StageOne();

	bool Init();

	static StageOne* Create();

	void Render();

	void Update( float dTime ) {
		if( mSkinnedMesh ){
			mSkinnedMesh->Update( dTime );
		}
 		if (mCamera)
			mCamera->Update(dTime);

		if (mTimeForFPS > 2.f) {
			printf("FPS : %f\n", Application::GetInstance()->GetFps());
			mTimeForFPS = 0.f;
		}
		
		mTimeForFPS += dTime;
	}

	void KeyDown( KeyEvent* pKeyEvent ) {
		//printf( "KeyDown\n" );
	}
	void KeyPressed( KeyEvent* pKeyEvent ) {
		switch( pKeyEvent->GetKeyCode() )
		{
		case 'W':
			mSkinnedMesh->Translation( 0, 0, 0.1f );
			break;
		case 'S':
			mSkinnedMesh->Translation( 0, 0, -0.1f );
			break;
		case 'A':
			mSkinnedMesh->Translation( -0.1f, 0, 0 );
			break;
		case 'D':
			mSkinnedMesh->Translation( 0.1f, 0, 0 );
			break;

		case VK_LEFT:
			mSkinnedMesh->RotationY( -0.1f );
			break;
		case VK_RIGHT:
			mSkinnedMesh->RotationY( 0.1f );
			break;

		case 'Q':
			mDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			break;
		case 'E':
			mDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			break;
		}

		//printf( "KeyPressed\n" );
	}
	void KeyUp(KeyEvent* pKeyEvent) {
		//printf("KeyUp\n");
	}

	void MouseDown(MouseEvent* pMouseEvent) {
		//printf("MouseDown\n");
	}

	void MouseMove(MouseEvent* pMouseEvent) {
		//printf("MouseMove %d %d\n", pMouseEvent->GetX(), pMouseEvent->GetY());
	}

	void MouseUp(MouseEvent* pMouseEvent) {
		//printf("MouseUp\n");
	}

	void MousePressed(MouseEvent* pMouseEvent) {
		//printf("MousePressed\n");
	}

	void MouseWheel(MouseEvent* pMouseEvent) {
		//printf("MouseWheel\n");
	}



protected:

private:
	ThirdPersonCamera	*mCamera = nullptr;
	SkinnedMesh			*mSkinnedMesh = nullptr;
	SkinnedMesh			*mSkinnedMesh_2 = nullptr;

	SkinnedMesh			*mGround_2 = nullptr;

	float				xTrans = 0.f;
	float				zTrans = 0.f;
	float				yAngle = 0.f;

	float				mTimeForFPS = 0.f;

	CollisionBox*		testDummy = nullptr;

	LPDIRECT3DDEVICE9	mDevice = nullptr;
};

