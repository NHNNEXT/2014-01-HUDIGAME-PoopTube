#include "stdafx.h"
#include "MainCharacter.h"
#include "SkinnedMesh.h"
#include "CollisionBox.h"
#include "CollisionManager.h"

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
	mCollisionBox->Render();
}

void MainCharacter::Update(float dTime) {
	Node::Update(dTime);

	// agebreak : CollisionBox도 오브젝로 만들어서 Attach 시키는 구조로 만드는게 좋다.
	mSkinnedMesh->SetPosition(Node::GetPosition());
	mSkinnedMesh->SetFrontVector(Node::GetFrontVector());
	mSkinnedMesh->Update(dTime);

	mCollisionBox->SetPosition(Node::GetPosition());
	mCollisionBox->Translation(D3DXVECTOR3(0.f, mCollisionBox->GetAxisLenY(), 0.f));
	mCollisionBox->SetFrontVector(Node::GetFrontVector());
	mCollisionBox->Update(dTime);
	Node* collisionResult = pooptube::CollisionManager::GetInstance()->CollisionCheck( mCollisionBox );
	if( collisionResult != nullptr ){
		D3DXVECTOR3 dPos = GetPosition( ) - collisionResult->GetPosition( );
		D3DXVec3Normalize( &dPos, &dPos );
		dPos *= mSpeed;
		Translation( dPos );
	}
}
bool MainCharacter::Init( MainCharacter *pMainCharacter ) {
	Node::Init();

	EnableKeyEvent();
	EnableMouseEvent();

	mSkinnedMesh = pooptube::SkinnedMesh::Create("batman70.fbx");
	mCollisionBox = pooptube::CollisionBox::Create( pMainCharacter );
	mCollisionBox->SetAABBCollisionBoxFromSkinnedMesh(mSkinnedMesh);

	return true;
}

void MainCharacter::KeyDown(pooptube::KeyEvent* pKeyEvent) {
}

void MainCharacter::KeyPressed(pooptube::KeyEvent* pKeyEvent) {
	switch (pKeyEvent->GetKeyCode())
	{
	case 'W':
		Translation(Node::GetFrontVector()*mSpeed);	// agebreak : Node에 각 방향 벡터를 가져오는 함수를 만든것은 잘했음. 나중에도 쓸데가 많음.
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

// agebreak : 점프는 캐릭터 자체 내에서 처리할것!
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
