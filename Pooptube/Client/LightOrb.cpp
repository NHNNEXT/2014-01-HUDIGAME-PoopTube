#include "stdafx.h"
#include "LightOrb.h"
#include "SkinnedMesh.h"
#include "CollisionBox.h"
#include "CollisionManager.h"

LightOrb::LightOrb()
{
}


LightOrb::~LightOrb()
{
}

std::shared_ptr<LightOrb> LightOrb::Create()
{
	std::shared_ptr<LightOrb> pCreature(new LightOrb);

	if (pCreature->Init(pCreature))
		return pCreature;
	else
		return nullptr;
}

bool LightOrb::Init(std::shared_ptr<LightOrb> pCreature)
{
	Node::Init();

	EnableKeyEvent();
	EnableMouseEvent();

	mSkinnedMesh = pooptube::SkinnedMesh::Create("batman70.fbx");
	mCollisionBox = pooptube::CollisionBox::Create(pCreature.get());
	mCollisionBox->SetAABBCollisionBoxFromSkinnedMesh(mSkinnedMesh);

	LightOrb::SetPosition(mInitialPosition);

	AddChild(&*mSkinnedMesh);
	AddChild(&*mCollisionBox);

	return true;
}

class aaaa
{

};
class bbbb : public aaaa
{

};
void LightOrb::Render()
{
	static bool isRender = false;
	Node *test = pooptube::CollisionManager::GetInstance()->CollisionCheck(&*mCollisionBox);
	if (test != nullptr && typeid(dynamic_cast<MainCharacter*>(test)).name() == typeid(MainCharacter *).name())
		isRender = true;
		printf("%s :: %s\n", typeid(dynamic_cast<MainCharacter*>(pooptube::CollisionManager::GetInstance()->CollisionCheck(&*mCollisionBox))).name(), typeid(MainCharacter *).name());
	
		if (isRender == false)
		Node::Render();
}

void LightOrb::Update(float dTime)
{
	Node::Update(dTime);

	mSkinnedMesh->SetPosition(Node::GetPosition());
	mSkinnedMesh->SetFrontPoint(Node::GetFrontPoint());
	mSkinnedMesh->Update(dTime);

	mCollisionBox->SetPosition(Node::GetPosition());
	mCollisionBox->Translation(D3DXVECTOR3(0.f, mCollisionBox->GetAxisLenY(), 0.f));
	mCollisionBox->SetFrontPoint(Node::GetFrontPoint());
	mCollisionBox->Update(dTime);

	Node* collisionResult = pooptube::CollisionManager::GetInstance()->CollisionCheck(mCollisionBox.get());

	if (collisionResult != nullptr) {
		D3DXVECTOR3 dPos = GetPosition() - collisionResult->GetPosition();
		D3DXVec3Normalize(&dPos, &dPos);
		Translation(dPos);
	}

	D3DXVECTOR3 pos = GetPosition();
	float height = (dynamic_cast<StageOne*>(pooptube::Application::GetInstance()->GetSceneManager()->GetCurrentScene()))->GetGroundModule()->GetHeight(GetPosition().x, GetPosition().z);

	if (height != pos.y) {
		pos.y = height;
		SetPosition(pos);
	}
}