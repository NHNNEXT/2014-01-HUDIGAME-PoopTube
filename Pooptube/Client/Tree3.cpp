#include "stdafx.h"
#include "Tree3.h"
#include "XMesh.h"
#include "SkinnedMesh.h"
#include "ResourceDef.h"
#include "CollisionBox.h"
#include "Scene.h"
#include "BillBoard.h"
#include "PooMath.h"

Tree3::Tree3()
{
}


Tree3::~Tree3()
{
}

Tree3 * Tree3::Create() {
	Tree3 *pTree1(new Tree3);

	if (pTree1->Init()){
		return pTree1;
	}
	else
		return nullptr;
}

bool Tree3::Init() {
	Node::Init();

	// 	mSkinnedMesh = pooptube::SkinnedMesh::Create(PATH_Tree1_1);
	// 	//mSkinnedMesh->SetScale(0.5f, 0.5f, 0.5f);
	// 	AddChild(mSkinnedMesh);

	//���������� �Ʒ��� ����ϸ� �ɵ�
	mXMesh = pooptube::XMesh::Create(PATH_TREE_3);
	mXMesh->SetScale(0.2f, 0.5f, 0.2f);
	AddChild(mXMesh);
	mBillBoard = pooptube::BillBoard::Create();
	mBillBoard->SetPosition( 0.f, 14.95f, 0.f );
	mBillBoard->SetScale( 3.95f, 16.4f, 1.f );
	AddChild( mBillBoard );

	pooptube::CollisionBox* collisionBox = pooptube::CollisionBox::Create(this);
	collisionBox->Translation(D3DXVECTOR3(0.f, 1.2f, 0.f));
	collisionBox->SetCollisionType(pooptube::CollisionBox::BLOCK);
	collisionBox->SetAxisLenY(1.2f);
	collisionBox->SetAxisLenX(1.2f);
	collisionBox->SetAxisLenZ(1.2f);
	AddChild(collisionBox);

	return true;
}

void Tree3::Render() {
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

void Tree3::Update(float dTime) {
}

void Tree3::_SetBillBoardTexture()
{
	float angle = pooptube::CalculateAngle( GetFrontVector(), mBillBoard->GetFrontVector() );
	angle /= D3DX_PI;
	angle *= 4;
	int dir( angle );
	dir += 8;
	dir %= 8;
	switch( dir ){
	case 0:
		mBillBoard->SetTexture( L"Model\\Tree3_0.png" );
		break;
	case 1:
		mBillBoard->SetTexture( L"Model\\Tree3_1.png" );
		break;
	case 2:
		mBillBoard->SetTexture( L"Model\\Tree3_2.png" );
		break;
	case 3:
		mBillBoard->SetTexture( L"Model\\Tree3_3.png" );
		break;
	case 4:
		mBillBoard->SetTexture( L"Model\\Tree3_4.png" );
		break;
	case 5:
		mBillBoard->SetTexture( L"Model\\Tree3_5.png" );
		break;
	case 6:
		mBillBoard->SetTexture( L"Model\\Tree3_6.png" );
		break;
	case 7:
		mBillBoard->SetTexture( L"Model\\Tree3_7.png" );
		break;
	}
}

