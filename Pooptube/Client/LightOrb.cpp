#include "stdafx.h"
#include "LightOrb.h"
#include "SkinnedMesh.h"
#include "CollisionBox.h"
#include "CollisionManager.h"
#include "ResourceDef.h"
#include "SoundManager.h"
#include "CollisionManager.h"
#include "Ground.h"
#include "StageOne.h"
#include "MainCharacter.h"

LightOrb::LightOrb()
{
}


LightOrb::~LightOrb()
{
}

LightOrb *LightOrb::Create()
{
	LightOrb *pCreature(new LightOrb);

	if (pCreature->Init(pCreature))
		return pCreature;
	else
		return nullptr;
}

bool LightOrb::Init(LightOrb *pCreature)
{
	Node::Init();

	mClassName = "LightOrb";
	mObjectName = "LightOrb";

	mMesh = pooptube::SkinnedMesh::Create(L"Model//sphere.x");
	mMesh->SetScale(D3DXVECTOR3(0.01f, 0.01f, 0.01f));
	AddChild(mMesh);

	pooptube::CollisionBox* collisionBox = pooptube::CollisionBox::Create( pCreature );
	AddChild( collisionBox );

	LightOrb::SetPosition(mInitialPosition);

	mEffectSound = pooptube::SoundManager::GetInstance()->GetSound( "event:/Character/Okay" );

	return true;
}

void LightOrb::Render()
{
//	static bool isRender = false;
//	Node *test = pooptube::CollisionManager::GetInstance()->CollisionCheckNode( this );
//	if (test != nullptr && typeid(dynamic_cast<MainCharacter*>(test)).name() == typeid(MainCharacter *).name())
//		isRender = true;
		//printf("%s :: %s\n", typeid(dynamic_cast<MainCharacter*>(pooptube::CollisionManager::GetInstance()->CollisionCheck(&*mCollisionBox))).name(), typeid(MainCharacter *).name());
	
		if (isRender == true)
		Node::Render();
}

void LightOrb::Update(float dTime)
{
	Node::Update(dTime);

	_CollsionHandle( pooptube::CollisionManager::GetInstance()->CollisionCheckNode( this ) );

	D3DXVECTOR3 pos = GetPosition();
	float height = (dynamic_cast<StageOne*>(pooptube::Application::GetInstance()->GetSceneManager()->GetCurrentScene()))->GetGroundModule()->GetHeight(GetPosition().x, GetPosition().z);

	if (height != pos.y) {
		pos.y = height;
		SetPosition(pos);
	}
}

void LightOrb::_CollsionHandle( pooptube::CollisionBox* collisionResult )
{
	if( collisionResult == nullptr )
		return;
	if( collisionResult->GetCollisionType() & pooptube::CollisionBox::COLLISION_TYPE::PLAYER ) {
		isRender = false;
		if( mEffectSound != nullptr )
			pooptube::SoundManager::GetInstance()->PlayOnce( *mEffectSound );
	}
}