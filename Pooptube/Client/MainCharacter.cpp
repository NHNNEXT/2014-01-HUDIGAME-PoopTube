﻿#include "stdafx.h"
#include "MainCharacter.h"
#include "SkinnedMesh.h"
#include "CollisionBox.h"
#include "CollisionManager.h"
#include "SoundManager.h"
#include "InputManager.h"


MainCharacter::MainCharacter() {
}


MainCharacter::~MainCharacter() {
	delete mSkinnedMesh;
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
	mSkinnedMesh->Render();

	//printf_s("%f, %f, %f\n", GetPosition().x, GetPosition().y, GetPosition().z);
}

void MainCharacter::Update(float dTime) {
	Node::Update(dTime);

	UpdateInput(dTime);

	switch (mState) {
	case NONE:
		mSkinnedMesh->SetAnimationTrack(3);
		break;
	case MOVE:
		mSkinnedMesh->SetAnimationTrack(2);
		break;
	case RUN:
		mSkinnedMesh->SetAnimationTrack(1);
		break;
	case SHAKEHAND:
		mSkinnedMesh->SetAnimationTrack(0);
		break;
	default:
		break;
	}

	mSkinnedMesh->SetPosition(GetPosition());
	mSkinnedMesh->Update(dTime);
	_CollsionHandle( pooptube::CollisionManager::GetInstance()->CollisionCheckNode( this ) );

	pooptube::SoundManager::GetInstance()->NodeToFmod3DAttribute( *this, mListener );
	pooptube::SoundManager::GetInstance()->SetListener( &mListener );

	if (mState == MOVE) {
		mState = NONE;
	}
}
bool MainCharacter::Init() {
	Node::Init();

	//사용하는 메쉬가 이상하게 좌표축이 설정되어있어서 아래처럼 처리했음
	mSkinnedMesh = pooptube::SkinnedMesh::Create(PATH_TINY);
	mSkinnedMesh->SetAnimationTrack(3);
	mSkinnedMesh->SetScale(D3DXVECTOR3(0.005f, 0.005f, 0.005f));
	mSkinnedMesh->SetFrontVector(D3DXVECTOR3(0.f, 1.f, 0.f));
	mSkinnedMesh->SetUpVec(D3DXVECTOR3(0.f, 0.f, -1.f));
	
	pooptube::CollisionBox* collisionBox = pooptube::CollisionBox::Create( this );
	collisionBox->SetCollisionType( pooptube::CollisionBox::COLLISION_TYPE( pooptube::CollisionBox::COLLISION_TYPE::PLAYER | pooptube::CollisionBox::COLLISION_TYPE::BLOCK ) );
	collisionBox->SetAxisLenY(2.5f);
	AddChild( collisionBox );
	//AddChild( mSkinnedMesh );
	

	return true;
}

void MainCharacter::_CollsionHandle( pooptube::CollisionBox* collisionResult )
{
	if( collisionResult == nullptr )
		return;
	if( collisionResult->GetCollisionType() & pooptube::CollisionBox::COLLISION_TYPE::BLOCK ){
		D3DXVECTOR3 dPos = GetPosition() - collisionResult->GetParent()->GetPosition();
		D3DXVec3Normalize( &dPos, &dPos );
		dPos *= mSpeed;
		Translation( dPos );
	}
}

void MainCharacter::UpdateInput(float dTime) {
	if (pooptube::gInputManager.KeyState('W') == pooptube::KeyState::KEY_PRESSED) {
		Translation(Node::GetFrontVector()*mSpeed*dTime);
		if (mState != JUMP)
			mState = MOVE;
	}
		
	if (pooptube::gInputManager.KeyState('S') == pooptube::KeyState::KEY_PRESSED) {
		Translation(Node::GetFrontVector()*mSpeed*-1.f*dTime);
		if (mState != JUMP)
			mState = MOVE;
	}
		
	if (pooptube::gInputManager.KeyState('A') == pooptube::KeyState::KEY_PRESSED) {
		Translation(Node::GetLeftVector()*mSpeed*dTime);
		if (mState != JUMP)
			mState = MOVE;
	}
		
	if (pooptube::gInputManager.KeyState('D') == pooptube::KeyState::KEY_PRESSED) {
		Translation(Node::GetRightVector()*mSpeed*dTime);
		if (mState != JUMP)
			mState = MOVE;
	}

	if (pooptube::gInputManager.KeyState('Q') == pooptube::KeyState::KEY_PRESSED) {
		if (mState != JUMP)
			mState = SHAKEHAND;
	}

	if (pooptube::gInputManager.KeyState(VK_LEFT) == pooptube::KeyState::KEY_PRESSED) {
		RotationY(-mRotateSpeed*dTime);
		mSkinnedMesh->RotationY(-mRotateSpeed*dTime);
	}
		
	if (pooptube::gInputManager.KeyState(VK_RIGHT) == pooptube::KeyState::KEY_PRESSED) {
		RotationY(mRotateSpeed*dTime);
		mSkinnedMesh->RotationY(mRotateSpeed*dTime);
	}
		
	if (pooptube::gInputManager.KeyState(VK_SPACE) == pooptube::KeyState::KEY_DOWN)
		mState = JUMP;
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
