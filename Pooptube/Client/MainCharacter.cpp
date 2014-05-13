#include "stdafx.h"
#include "MainCharacter.h"
#include "SkinnedMesh.h"
#include "CollisionBox.h"
#include "CollisionManager.h"
#include "SoundManager.h"

MainCharacter::MainCharacter() {
}


MainCharacter::~MainCharacter() {
}

MainCharacter *MainCharacter::Create() {
	MainCharacter *pMainCharacter(new MainCharacter);

	if( pMainCharacter->Init( pMainCharacter ) ){
		return pMainCharacter;
	}
	else
		return nullptr;
}

void MainCharacter::Render() {
	Node::Render();
	
	mSkinnedMesh->Render();
}

void MainCharacter::Update(float dTime) {
	Node::Update(dTime);

	mSkinnedMesh->SetPosition(Node::GetPosition());
	mSkinnedMesh->SetFrontVector(Node::GetFrontVector());
	mSkinnedMesh->Update(dTime);

	_CollsionHandle( pooptube::CollisionManager::GetInstance()->CollisionCheckNode( this ) );

	pooptube::SoundManager::GetInstance()->NodeToFmod3DAttribute( *this, mListener );
	pooptube::SoundManager::GetInstance()->SetListener( &mListener );
}
bool MainCharacter::Init( MainCharacter *pMainCharacter ) {
	Node::Init();

	EnableKeyEvent();
	EnableMouseEvent();

	mSkinnedMesh = pooptube::SkinnedMesh::Create("batman70.fbx");
	pooptube::CollisionBox* collisionBox = pooptube::CollisionBox::Create( pMainCharacter );
	collisionBox->SetAABBCollisionBoxFromSkinnedMesh( mSkinnedMesh );
	collisionBox->SetCollisionType( pooptube::CollisionBox::COLLISION_TYPE( pooptube::CollisionBox::COLLISION_TYPE::PLAYER | pooptube::CollisionBox::COLLISION_TYPE::BLOCK ) );
	AddChild( collisionBox );

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
