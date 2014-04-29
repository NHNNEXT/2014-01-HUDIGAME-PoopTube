#include "stdafx.h"
#include "MainCharacter.h"
#include "SkinnedMesh.h"
#include "CollisionBox.h"
#include "CollisionManager.h"

MainCharacter::MainCharacter() {
}


MainCharacter::~MainCharacter() {
}

std::shared_ptr<MainCharacter> MainCharacter::Create() {
	std::shared_ptr<MainCharacter> pMainCharacter(new MainCharacter);

	if( pMainCharacter->Init( pMainCharacter ) ){
		return pMainCharacter;
	}
	else
		return nullptr;
}

void MainCharacter::Render() {
	Node::Render();
	
	mSkinnedMesh->Render();
	mCollisionBox->Render();
}

void MainCharacter::Update(float dTime) {
	Node::Update(dTime);

	mSkinnedMesh->SetPosition(Node::GetPosition());
	mSkinnedMesh->SetFrontVector(Node::GetFrontVector());
	mSkinnedMesh->Update(dTime);

	mCollisionBox->SetPosition(Node::GetPosition());
	mCollisionBox->Translation(D3DXVECTOR3(0.f, mCollisionBox->GetAxisLenY(), 0.f));
	mCollisionBox->SetFrontVector(Node::GetFrontVector());
	mCollisionBox->Update(dTime);
	Node* collisionResult = pooptube::CollisionManager::GetInstance()->CollisionCheck( mCollisionBox.get() );
	if( collisionResult != nullptr ){
		D3DXVECTOR3 dPos = GetPosition( ) - collisionResult->GetPosition( );
		D3DXVec3Normalize( &dPos, &dPos );
		dPos *= mSpeed;
		Translation( dPos );
	}
}
bool MainCharacter::Init( std::shared_ptr<MainCharacter> pMainCharacter ) {
	Node::Init();

	EnableKeyEvent();
	EnableMouseEvent();

	mSkinnedMesh = pooptube::SkinnedMesh::Create("batman70.fbx");
	mCollisionBox = pooptube::CollisionBox::Create( pMainCharacter.get() );
	mCollisionBox->SetAABBCollisionBoxFromSkinnedMesh(mSkinnedMesh);

	return true;
}

void MainCharacter::KeyDown(pooptube::KeyEvent* pKeyEvent) {
}

void MainCharacter::KeyPressed(pooptube::KeyEvent* pKeyEvent) {
	switch (pKeyEvent->GetKeyCode())
	{
	case 'W':
		Translation(Node::GetFrontVector()*mSpeed);
		break;
	case 'S':
		Translation(Node::GetFrontVector()*mSpeed*-1.f);
		break;
	case 'A':
		Translation(Node::GetLeftVector()*mSpeed);
		break;
	case 'D':
		Translation(Node::GetRightVector()*mSpeed);
		break;

	case VK_LEFT:
		RotationY(-0.1f);
		break;
	case VK_RIGHT:
		RotationY(0.1f);
		break;
	}
}

void MainCharacter::KeyUp(pooptube::KeyEvent* pKeyEvent) {
	switch (pKeyEvent->GetKeyCode()) {
	case VK_SPACE:
		mState = JUMP;
		break;
	}
}

void MainCharacter::MouseDown(pooptube::MouseEvent* pMouseEvent) {
}

void MainCharacter::MouseMove(pooptube::MouseEvent* pMouseEvent) {
}

void MainCharacter::MouseUp(pooptube::MouseEvent* pMouseEvent) {
}

void MainCharacter::MousePressed(pooptube::MouseEvent* pMouseEvent) {
}

void MainCharacter::MouseWheel(pooptube::MouseEvent* pMouseEvent) {

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
