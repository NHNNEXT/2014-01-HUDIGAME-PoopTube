﻿#include "stdafx.h"
#include "MainCharacter.h"
#include "SkinnedMesh.h"
#include "CollisionBox.h"
#include "CollisionManager.h"
#include "SoundManager.h"
#include "InputManager.h"
#include "Light.h"
#include "Ground.h"

MainCharacter::MainCharacter() {
}


MainCharacter::~MainCharacter() {
}

MainCharacter *MainCharacter::Create() {
	MainCharacter *pMainCharacter(new MainCharacter);

	if( pMainCharacter->Init() ){
		return pMainCharacter;
	}
	else
		return nullptr;
}

void MainCharacter::Render() {
	Node::Render();
	//mSkinnedMesh->Render();

	//printf_s("%f, %f, %f\n", GetPosition().x, GetPosition().y, GetPosition().z);
}

void MainCharacter::Update(float dTime) {
	Node::Update(dTime);

	UpdateInput(dTime);
	//_JumpUpdate( dTime );
// 
// 	switch (mState) {
// 	case NONE:
// 		mSkinnedMesh->SetAnimationTrack(3);
// 		break;
// 	case MOVE:
// 		mSkinnedMesh->SetAnimationTrack(2);
// 		break;
// 	case RUN:
// 		mSkinnedMesh->SetAnimationTrack(1);
// 		break;
// 	case SHAKEHAND:
// 		mSkinnedMesh->SetAnimationTrack(0);
// 		break;
// 	default:
// 		break;
// 	}
// 
// 	mSkinnedMesh->SetPosition(GetPosition());
// 	mSkinnedMesh->Update(dTime);
	_CollsionHandle( dTime, pooptube::CollisionManager::GetInstance()->CollisionCheckNode( this ) );

	pooptube::SoundManager::GetInstance()->NodeToFmod3DAttribute( *this, mListener );
	pooptube::SoundManager::GetInstance()->SetListener( &mListener );

	if (mState == MOVE) {
		mState = NONE;
	}
}
bool MainCharacter::Init() {
	Node::Init();

	mLight = pooptube::Light::Create();
	AddChild(mLight);

	mMesh = pooptube::SkinnedMesh::Create(PATH_MAINCHAR);
	//mMesh->RotateFrontVectorX(-0.3f);
	mMesh->RotateFrontVectorY(3.14f);
	mMesh->SetAnimationTrack(8);
	mMesh->SetScale(D3DXVECTOR3(3.f, 3.f, 3.f));
	AddChild(mMesh);
	
	pooptube::CollisionBox* collisionBox = pooptube::CollisionBox::Create( this );
	collisionBox->SetCollisionType( pooptube::CollisionBox::COLLISION_TYPE( pooptube::CollisionBox::COLLISION_TYPE::PLAYER | pooptube::CollisionBox::COLLISION_TYPE::BLOCK ) );
	collisionBox->Translation(D3DXVECTOR3(0.f, 1.2f, 0.f));
	collisionBox->SetAxisLenY(1.2f);
	AddChild( collisionBox );
	//AddChild( mSkinnedMesh );
	

	return true;
}

void MainCharacter::_CollsionHandle( float dTime, pooptube::CollisionBox* collisionResult )
{
	if( collisionResult == nullptr )
		return;
	if( collisionResult->GetCollisionType() & pooptube::CollisionBox::COLLISION_TYPE::BLOCK ){
		D3DXVECTOR3 dPos = GetPosition() - collisionResult->GetParent()->GetPosition();
		D3DXVec3Normalize( &dPos, &dPos );
		dPos *= mSpeed * dTime;
		Translation( dPos );
	}

	/*if (collisionResult->GetCollisionType() && pooptube::CollisionBox::COLLISION_TYPE::LIGHTORB) {
		InCreaseHP(1);
	}*/
	
}

void MainCharacter::UpdateInput(float dTime) {
	if (pooptube::GetInputManager().KeyState('W') == pooptube::KeyState::KEY_PRESSED) {
		Translation(Node::GetFrontVector()*mSpeed*dTime);
		if (mState != JUMP)
			mState = MOVE;
	}
		
	if (pooptube::GetInputManager().KeyState('S') == pooptube::KeyState::KEY_PRESSED) {
		Translation(Node::GetFrontVector()*mSpeed*-1.f*dTime);
		if (mState != JUMP)
			mState = MOVE;
	}
		
	if (pooptube::GetInputManager().KeyState('A') == pooptube::KeyState::KEY_PRESSED) {
		Translation(Node::GetLeftVector()*mSpeed*dTime);
		if (mState != JUMP)
			mState = MOVE;
	}
		
	if (pooptube::GetInputManager().KeyState('D') == pooptube::KeyState::KEY_PRESSED) {
		Translation(Node::GetRightVector()*mSpeed*dTime);
		if (mState != JUMP)
			mState = MOVE;
	}

	if (pooptube::GetInputManager().KeyState('Q') == pooptube::KeyState::KEY_PRESSED) {
		if (mState != JUMP)
			mState = SHAKEHAND;
	}

	if (pooptube::GetInputManager().KeyState(VK_LEFT) == pooptube::KeyState::KEY_PRESSED) {
		RotationY(-mRotateSpeed*dTime);
	}
		
	if (pooptube::GetInputManager().KeyState(VK_RIGHT) == pooptube::KeyState::KEY_PRESSED) {
		RotationY(mRotateSpeed*dTime);
	}
		
	if (pooptube::GetInputManager().KeyState(VK_SPACE) == pooptube::KeyState::KEY_DOWN)
		mState = JUMP;
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
