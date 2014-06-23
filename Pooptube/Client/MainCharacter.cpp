#include "stdafx.h"
#include "MainCharacter.h"
#include "SkinnedMesh.h"
#include "CollisionBox.h"
#include "CollisionManager.h"
#include "SoundManager.h"
#include "InputManager.h"
#include "Light.h"
#include "Ground.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "PooMath.h"

MainCharacter::MainCharacter() {
}


MainCharacter::~MainCharacter() {
}

MainCharacter *MainCharacter::Create( pooptube::Scene* scene ) {
	MainCharacter *pMainCharacter(new MainCharacter);

	if( pMainCharacter->Init( scene ) ){
		return pMainCharacter;
	}
	else
		return nullptr;
}

void MainCharacter::Render() {

	mMesh->GetMeshData()->GetEffect()->SetTechnique("t5");
	Node::Render();
	//mSkinnedMesh->Render();

	//printf_s("%f, %f, %f\n", GetPosition().x, GetPosition().y, GetPosition().z);
}

void MainCharacter::Update(float dTime) {
	Node::Update(dTime);

	mState = NONE;

	float lightPos[3];
	D3DXVECTOR3 charPos = GetPosition();
	D3DXVECTOR3 charFront = GetFrontVector();
	D3DXVec3Normalize(&charFront, &charFront);
	charPos += charFront;
	lightPos[0] = charPos.x;
	lightPos[1] = charPos.y + 2.f;
	lightPos[2] = charPos.z;

	pooptube::ResourceManager::GetInstance()->LoadHLSL(L"Shader\\SkinnedMesh.fx")->SetFloatArray("lightPos", lightPos, 3);

	UpdateInput(dTime);

	switch (mState) {
	case NONE:
		mMesh->SetAnimationTrack(3);
		break;
	case MOVE:
		mMesh->SetAnimationTrack(8);
		//mMesh->SetAnimationBlend(8, 5);
		break;
	default:
		break;
	}
 
// 	mSkinnedMesh->SetPosition(GetPosition());
// 	mSkinnedMesh->Update(dTime);
	_CollsionHandle( pooptube::CollisionManager::GetInstance()->CollisionCheckNode( this ) );
// 	SetPosition( mPosition.x, mScene->GetGroundModule()->GetHeight( mPosition.x, mPosition.z ), mPosition.z );
	SetPosition( mScene->GetGroundModule()->GetValidPosition( mPosition ) );
	
	pooptube::SoundManager::GetInstance()->NodeToFmod3DAttribute( *this, mListener );
	pooptube::SoundManager::GetInstance()->SetListener( &mListener );
}
bool MainCharacter::Init( pooptube::Scene* scene ) {
	Node::Init();
	mScene = scene;

	//mLight = pooptube::Light::Create();
	//AddChild(mLight);
	// mScene->AddRenderZone( mLight, D3DXVECTOR3( 0.f, 0.f, 0.f ), mLight->GetRange()+15.f ); // Range+여유분 거리
	mScene->AddRenderZone(this, D3DXVECTOR3(0.f, 0.f, 0.f), 30.f); // Range+여유분 거리

	mMesh = pooptube::SkinnedMesh::Create(PATH_MAINCHAR);
	mMesh->RotateFrontVectorY(3.14f);
	mMesh->SetScale(D3DXVECTOR3(3.f, 3.f, 3.f));
	AddChild(mMesh);
	
	pooptube::CollisionBox* collisionBox = pooptube::CollisionBox::Create( this );
	collisionBox->SetCollisionType( pooptube::CollisionBox::COLLISION_TYPE( pooptube::CollisionBox::COLLISION_TYPE::PLAYER | pooptube::CollisionBox::COLLISION_TYPE::BLOCK ) );
	collisionBox->Translation(D3DXVECTOR3(0.f, 1.2f, 0.f));
	AddChild( collisionBox );
	mCollisionBox = collisionBox;
	

	return true;
}

void MainCharacter::_CollsionHandle( pooptube::CollisionBox* collisionResult )
{
	if( collisionResult == nullptr )
		return;
	if( collisionResult->GetCollisionType() & pooptube::CollisionBox::COLLISION_TYPE::BLOCK ){
// 		D3DXVECTOR3 dPos = GetPosition() - collisionResult->GetParent()->GetPosition();
		D3DXVECTOR3 dPos = mCollisionBox->GetPosition() - collisionResult->GetPosition();
		float dLen = D3DXVec3Length( &dPos );
		
		// 두 충돌 박스가 겹친 길이 구하기
		D3DXVECTOR3 resLenVec, resLenVecTmp( collisionResult->GetAxisLenX(), collisionResult->GetAxisLenY(), collisionResult->GetAxisLenZ() );
		D3DXVECTOR3 thisLenVec, thisLenVecTmp( mCollisionBox->GetAxisLenX(), mCollisionBox->GetAxisLenY(), mCollisionBox->GetAxisLenZ() );
		float dPos_resLenVec_angle(D3DX_PI), dPos_thisLenVec_angle(0.f);
		float dPos_resLenVec_angleTmp, dPos_thisLenVec_angleTmp;
		for( int xIdx = 1; xIdx > -2; xIdx -= 2 ){
			resLenVecTmp.x *= xIdx;
			thisLenVecTmp.x *= xIdx;
			for( int yIdx = 1; yIdx > -2; yIdx -= 2 ){
				resLenVecTmp.y *= yIdx;
				thisLenVecTmp.y *= yIdx;
				for( int zIdx = 1; zIdx > -2; zIdx -= 2 ){
					resLenVecTmp.z *= zIdx;
					thisLenVecTmp.z *= zIdx;

					dPos_resLenVec_angleTmp = pooptube::CalculateAngle( dPos, resLenVecTmp );
					dPos_thisLenVec_angleTmp = pooptube::CalculateAngle( dPos, thisLenVecTmp );

					if( dPos_resLenVec_angle > dPos_resLenVec_angleTmp ){
						dPos_resLenVec_angle = dPos_resLenVec_angleTmp;
						resLenVec = resLenVecTmp;
					}
					if( dPos_thisLenVec_angle < dPos_thisLenVec_angleTmp ){
						dPos_thisLenVec_angle = dPos_thisLenVec_angleTmp;
						thisLenVec = thisLenVecTmp;
					}
				}
			}
		}

		D3DXVec3Normalize( &dPos, &dPos );
		dLen = D3DXVec3Dot( &dPos, &resLenVec ) - D3DXVec3Dot( &dPos, &thisLenVec ) - dLen;

		Translation( dPos * dLen );
	}

	/*if (collisionResult->GetCollisionType() && pooptube::CollisionBox::COLLISION_TYPE::LIGHTORB) {
		InCreaseHP(1);
	}*/
	
}

void MainCharacter::UpdateInput(float dTime) {
	if (pooptube::GetInputManager().KeyState(VK_UP) == pooptube::KeyState::KEY_PRESSED) {
		Translation(Node::GetFrontVector()*mSpeed*dTime);
		mState = MOVE;
	}

	if (pooptube::GetInputManager().KeyState(VK_DOWN) == pooptube::KeyState::KEY_PRESSED) {
		Translation(Node::GetFrontVector()*mSpeed*-1.f*dTime);
		mState = MOVE;
	}

	if (pooptube::GetInputManager().KeyState(VK_LEFT) == pooptube::KeyState::KEY_PRESSED) {
		RotationY(-mRotateSpeed*dTime);
		mState = MOVE;
	}
		
	if (pooptube::GetInputManager().KeyState(VK_RIGHT) == pooptube::KeyState::KEY_PRESSED) {
		RotationY(mRotateSpeed*dTime);
		mState = MOVE;
	}
}

void MainCharacter::_JumpUpdate( float dTime ) {
	//캐릭터 점프 알고리즘
	static float mTimeForJump = 0.f;
	pooptube::Ground* ground = pooptube::Application::GetInstance()->GetSceneManager()->GetCurrentScene()->GetGroundModule();
	D3DXVECTOR3 CharPos = GetPosition();
	float		MapHeight = ground->GetHeight( mPosition.x, CharPos.z );
	float		GroundAccel = ground->GetGravity();

	if( mState == JUMP ) {
		mTimeForJump += dTime;
		float		currentSpeed = mJumpSpeed - GroundAccel * mTimeForJump;

		Translation( 0.f, currentSpeed * dTime, 0.f );

		if( MapHeight > CharPos.y ) {
			CharPos.y = MapHeight;
			SetPosition( CharPos );
			SetState( NONE );

			mTimeForJump = 0.f;
		}
	}
	else if( mState == NONE ) {
		CharPos.y = MapHeight;
		SetPosition( CharPos );

	}
}

//
////void MainCharacter::CollsionReceive( std::shared_ptr<Node> target )
//void MainCharacter::CollsionReceive( Node* target )
//{
//
//}
//
////void MainCharacter::CollsionFeedBack( std::shared_ptr<Node> target )
//void MainCharacter::CollsionFeedBack( Node* target )
//{
//	D3DXVECTOR3 dPos = GetPosition() - target->GetPosition();
//	D3DXVec3Normalize( &dPos, &dPos );
//	dPos *= mSpeed;
//	Translation( dPos );
//}
