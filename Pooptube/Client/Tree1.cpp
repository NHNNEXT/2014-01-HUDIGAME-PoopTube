#include "stdafx.h"
#include "Tree1.h"
#include "XMesh.h"
#include "SkinnedMesh.h"
#include "ResourceDef.h"
#include "CollisionBox.h"
#include "Scene.h"
#include "BillBoard.h"
#include "PooMath.h"
#include "Ground.h"

Tree1::Tree1()
{
}


Tree1::~Tree1()
{
}

Tree1 * Tree1::Create() {
	Tree1 *pTree(new Tree1);

	if (pTree->Init()){
		return pTree;
	}
	else
		return nullptr;
}

bool Tree1::Init() {
	Node::Init();

// 	mSkinnedMesh = pooptube::SkinnedMesh::Create(PATH_TREE_1);
// 	//mSkinnedMesh->SetScale(0.5f, 0.5f, 0.5f);
// 	AddChild(mSkinnedMesh);

	//멥툴에서는 아래로 사용하면 될듯
	mXMesh = pooptube::XMesh::Create(PATH_TREE_1);
	mXMesh->SetPosition( 0.f, -0.5f, 0.f );
	AddChild(mXMesh);
	mBillBoard = pooptube::BillBoard::Create();
	mBillBoard->SetPosition( 0.f, 12.9f, 0.f );
	mBillBoard->SetScale( 10.2f, 14.2f, 1.f );
	AddChild( mBillBoard );

	pooptube::CollisionBox* collisionBox = pooptube::CollisionBox::Create(this);
	collisionBox->Translation(D3DXVECTOR3(1.2f, 1.2f, 0.f));
	collisionBox->SetCollisionType(pooptube::CollisionBox::BLOCK);
	collisionBox->SetAxisLenX(1.2f);
	collisionBox->SetAxisLenY(2.0f);
	AddChild(collisionBox);

	return true;
}

void Tree1::Render() {
	if( pooptube::Application::GetInstance()->GetSceneManager()->GetCurrentScene()->CheckRenderZone( GetPosition(), 2.f ) ){
		mXMesh->SetVisible( true );
		mBillBoard->SetVisible( false );
		Node::Render();
	}
	else{
		mXMesh->SetVisible( false );
		mBillBoard->SetVisible( true );
		_SetBillBoardTexture();
		Node::Render();
	}
}

void Tree1::Update(float dTime) {
}

void Tree1::_SetBillBoardTexture()
{
	float angle = pooptube::CalculateAngle( GetFrontVector(), mBillBoard->GetFrontVector() );
	angle /= D3DX_PI;
	angle *= 4;
	int dir( static_cast<int>(angle) );
	dir += 8;
	dir %= 8;
	switch( dir ){
	case 0:
		mBillBoard->SetTexture( L"Model\\Tree1_0.png" );
		break;
	case 1:
		mBillBoard->SetTexture( L"Model\\Tree1_1.png" );
		break;
	case 2:
		mBillBoard->SetTexture( L"Model\\Tree1_2.png" );
		break;
	case 3:
		mBillBoard->SetTexture( L"Model\\Tree1_3.png" );
		break;
	case 4:
		mBillBoard->SetTexture( L"Model\\Tree1_4.png" );
		break;
	case 5:
		mBillBoard->SetTexture( L"Model\\Tree1_5.png" );
		break;
	case 6:
		mBillBoard->SetTexture( L"Model\\Tree1_6.png" );
		break;
	case 7:
		mBillBoard->SetTexture( L"Model\\Tree1_7.png" );
		break;
	}
}

