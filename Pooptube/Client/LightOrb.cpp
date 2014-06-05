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
#include "XMesh.h"

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

	mMesh = pooptube::SkinnedMesh::Create(PATH_ORB);
	mMesh->SetScale(D3DXVECTOR3(0.1f, 0.1f, 0.1f));
	AddChild(mMesh);

	//¸ÊÅø¿¡¼­´Â ¹Ø¿¡ Ã³·³
// 	mXMesh = pooptube::XMesh::Create(PATH_SPHERE);
// 	mXMesh->SetScale(D3DXVECTOR3(0.01f, 0.01f, 0.01f));
// 	AddChild(mXMesh);

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
	
	GetDevice()->SetRenderState(D3DRS_LIGHTING, false);

	if (mIsRender == true)
		Node::Render();

	GetDevice()->SetRenderState(D3DRS_LIGHTING, true);
}

void LightOrb::Update(float dTime)
{
	Node::Update(dTime);

	_CollsionHandle( pooptube::CollisionManager::GetInstance()->CollisionCheckNode( this ) );

	D3DXVECTOR3 pos = GetPosition();

	mTime += dTime;

	D3DXVECTOR3 temp = LightOrb::GetPosition();
	float tempTheta = mTime * 3.14f / 180.f;
	temp.y = 2.f + sinf(tempTheta * 50);
	LightOrb::SetPosition(temp);
	//float height = (dynamic_cast<StageOne*>(pooptube::Application::GetInstance()->GetSceneManager()->GetCurrentScene()))->GetGroundModule()->GetHeight(GetPosition().x, GetPosition().z);

	//if (height != pos.y) {
	//	pos.y = height;
	//	SetPosition(pos);
	//}

}

void LightOrb::_CollsionHandle( pooptube::CollisionBox* collisionResult )
{
	if( collisionResult == nullptr )
		return;
	if( collisionResult->GetCollisionType() & pooptube::CollisionBox::COLLISION_TYPE::PLAYER ) {
		mIsRender = false;
		//mIsHealable = false;
		if( mEffectSound != nullptr )
			pooptube::SoundManager::GetInstance()->PlayOnce( *mEffectSound );
	}
}