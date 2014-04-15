#pragma once
#include "Scene.h"
#include "ThirdPersonCamera.h"
#include "SkinnedMesh.h"
#include "Ground.h"
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

	void Render() {

		if( mSkinnedMesh ){
			mSkinnedMesh->Render();
			mSkinnedMesh->mCollisionBox->Render();
		}

		if( mGround )
			mGround->Render();
		//		if (mSkinnedMesh_2)
		//			mSkinnedMesh_2->Render();

		mCamera->Render();

		if( testDummy )
			testDummy->Render();
	}

	void Update( float dTime ) {
		if( mSkinnedMesh ){
			mSkinnedMesh->Update( dTime );
			mSkinnedMesh->mCollisionBox->Update( dTime );
		}
	}

	void KeyDown( KeyEvent* pKeyEvent ) {
		printf( "KeyDown\n" );
	}
	void KeyPressed( KeyEvent* pKeyEvent ) {
		switch( pKeyEvent->GetKeyCode() )
		{
		case 'W':
			mSkinnedMesh->Translation( 0, 0, 0.1 );
			mSkinnedMesh->mCollisionBox->Translation( 0, 0, 0.1 );
			//mCamera->MoveForward(-0.1f);
			break;
		case 'S':
			mSkinnedMesh->Translation( 0, 0, -0.1 );
			mSkinnedMesh->mCollisionBox->Translation( 0, 0, -0.1 );
			//mCamera->MoveForward(0.1f);
			break;
		case 'A':
			mSkinnedMesh->Translation( -0.1, 0, 0 );
			mSkinnedMesh->mCollisionBox->Translation( -0.1, 0, 0 );
			//mCamera->MoveSide(0.1f);
			break;
		case 'D':
			mSkinnedMesh->Translation( 0.1, 0, 0 );
			mSkinnedMesh->mCollisionBox->Translation( 0.1, 0, 0 );
			//mCamera->MoveSide(-0.1f);
			break;

		case VK_LEFT:
			mSkinnedMesh->RotationY( 0.1 );
			mSkinnedMesh->mCollisionBox->RotationY( 0.1 );
			//mCamera->Rotate(D3DXVECTOR3(3.f, 0, 0));
			break;
		case VK_RIGHT:
			mSkinnedMesh->RotationY( -0.1 );
			mSkinnedMesh->mCollisionBox->RotationY( -0.1 );
			//mCamera->Rotate(D3DXVECTOR3(-3.f, 0, 0));
			break;
		}

		printf( "KeyPressed\n" );
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

	CollisionBox*		testDummy;
};

