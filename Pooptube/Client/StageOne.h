#pragma once
#include "Scene.h"
#include "ThirdPersonCamera.h"
#include "SkinnedMesh.h"
#include "CollisionBox.h"
#include "Camera.h"

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

		if (mTimeForFPS > 2.f) {
			printf("FPS : %f\n", Application::GetInstance()->GetFps());
			mTimeForFPS = 0.f;
		}
		
		mTimeForFPS += dTime;
	}

	void KeyDown( KeyEvent* pKeyEvent ) {
	}
	void KeyPressed( KeyEvent* pKeyEvent ) {
		switch( pKeyEvent->GetKeyCode() )
		{
		case 'W':
			mSkinnedMesh->Translation( 0, 0, -0.1f );
			break;
		case 'S':
			mSkinnedMesh->Translation( 0, 0, 0.1f );
			break;
		case 'A':
			mSkinnedMesh->Translation( 0.1f, 0, 0 );
			break;
		case 'D':
			mSkinnedMesh->Translation( -0.1f, 0, 0 );
			break;

		case VK_LEFT:
			mSkinnedMesh->RotationY( 0.1f );
			break;
		case VK_RIGHT:
			mSkinnedMesh->RotationY( -0.1f );
			break;

		case 'Q':
			mDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			break;
		case 'E':
			mDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			break;
		}
	}
	void KeyUp(KeyEvent* pKeyEvent) {
		
	}

	void MouseDown(MouseEvent* pMouseEvent) {
		
	}

	void MouseMove(MouseEvent* pMouseEvent) {
	}

	void MouseUp(MouseEvent* pMouseEvent) {
	}

	void MousePressed(MouseEvent* pMouseEvent) {
	}

	void MouseWheel(MouseEvent* pMouseEvent) {
	}



protected:

private:
	Camera				*mCamera = nullptr;


	SkinnedMesh			*mSkinnedMesh = nullptr;
	SkinnedMesh			*mSkinnedMesh_2 = nullptr;

	SkinnedMesh			*mGround_2 = nullptr;

	float				xTrans = 0.f;
	float				zTrans = 0.f;
	float				yAngle = 0.f;

	float				mTimeForFPS = 0.f;

	std::shared_ptr<CollisionBox>	testDummy;

	LPDIRECT3DDEVICE9	mDevice = nullptr;
};

