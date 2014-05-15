#include "stdafx.h"
#include "Creature.h"
#include "SkinnedMesh.h"
#include "CollisionBox.h"
#include "CollisionManager.h"
#include "XMesh.h"

Creature::Creature()
{
}


Creature::~Creature()
{
}

Creature *Creature::Create()
{
	Creature *pCreature( new Creature );

	if( pCreature->Init( pCreature ) )
		return pCreature;
	else
		return nullptr;
}

bool Creature::Init( Creature *pCreature )
{
	Node::Init();

	EnableKeyEvent();
	EnableMouseEvent();

	//mXMesh = pooptube::XMesh::Create("kuma.x");
	mSkinnedMesh = pooptube::SkinnedMesh::Create("batman70.fbx");
	mCollisionBox = pooptube::CollisionBox::Create( pCreature );
	mCollisionBox->SetAABBCollisionBoxFromSkinnedMesh(mSkinnedMesh);

	Creature::SetPosition(mInitialPosition);

	AddChild(mSkinnedMesh);
	AddChild(mCollisionBox);
	//AddChild(mXMesh.get());

	return true;

}

void Creature::Render()
{
	Node::Render();	
}

void Creature::Update(float dTime)
{
	Node::Update(dTime);

	mSkinnedMesh->SetPosition(Node::GetPosition());
	mSkinnedMesh->SetFrontVector(Node::GetFrontVector());
	mSkinnedMesh->Update(dTime);

	mCollisionBox->SetPosition(Node::GetPosition());
	mCollisionBox->Translation(D3DXVECTOR3(0.f, mCollisionBox->GetAxisLenY(), 0.f));
	mCollisionBox->SetFrontVector(Node::GetFrontVector());
	mCollisionBox->Update(dTime);

	Node* collisionResult = pooptube::CollisionManager::GetInstance()->CollisionCheck( mCollisionBox );

	if( collisionResult != nullptr ) {
		D3DXVECTOR3 dPos = GetPosition() - collisionResult->GetPosition();
		D3DXVec3Normalize( &dPos, &dPos );
		dPos *= mSpeed;
		Translation( dPos );
	}

	FSM();
	switch (mState) {
	case IDLE:
		DoIdle(dTime);
		break;
	case ANGRY:
		DoAngry();
		break;
	case RAGE:
		DoRage();
		break;
	}

	D3DXVECTOR3 pos = GetPosition();
	
	// agebreak : ㅋㅋㅋ 아놔.. 이거 고쳐!! 높이값하나 가져오는데 이게 뭐하는 짓인감.. 높이갚은 다른데도 자주 쓸건데!! 바로 접근해서 가져올 수 있도록 구조 수정할것!! 그리고 인자는 Vector로 받을수 있게!
	// 아님 차라리 Scene을 받아오고, Height를 받아오는 코드 두줄로 나누던가!!
	float height = (dynamic_cast<StageOne*>(pooptube::Application::GetInstance()->GetSceneManager()->GetCurrentScene()))->GetGroundModule()->GetHeight(GetPosition().x, GetPosition().z);	
	//float height = (dynamic_cast<StageOne*>(pooptube::Application::GetInstance()->GetSceneManager()->GetCurrentScene()))->GetGroundModule()->GetHeight(GetPosition().x, GetPosition().z);

	if (height != pos.y) {
		pos.y = height;
		SetPosition(pos);
	}
}

CREATURE_STATE Creature::FSM()
{
	D3DXVECTOR3 CharacterPosition = pss->GetPosition(); // agebreak : 이 두 벡터는 쓰지도 않는데?
	D3DXVECTOR3 CreaturePosition = GetPosition();		
	D3DXVECTOR3 distance = pss->GetPosition() - GetPosition();

	// agebreak : D3DXVec3Length는 나누기 연산이 있기 때문에, 부하가 크다.. 한번만 연산해서 결과값을 사용할 것
	if (mIdleDistance < D3DXVec3Length(&distance)) {

		if (mState != IDLE)

		SetState(CREATURE_STATE::IDLE);

		//printf("idle\n");
	} else if (mIdleDistance >= D3DXVec3Length(&distance) && D3DXVec3Length(&distance) >= mRageDistance) {
		SetState(ANGRY);
		//printf("angry\n");
	} else if (mRageDistance > D3DXVec3Length(&distance)) {
		SetState(RAGE);
		//printf("rage\n");
	}

	return IDLE;
}

void Creature::DoIdle(float dTime)
{
	D3DXVECTOR3 CreaturePosition = GetPosition();
	D3DXVECTOR3 distance = mInitialPosition - GetPosition();
	D3DXVECTOR3 CreatureFrontVector = GetFrontVector();

	if (IDLE == GetState() && D3DXVec3Length(&distance) < 0.5f) {
		RotationY(0.1f);
		SetPosition(mInitialPosition);
	}
	else //if (CreaturePosition != initialPosition) 
	{
		D3DXVECTOR3 dir = mInitialPosition - CreaturePosition;
		D3DXVec3Normalize(&dir, &dir);

		if (Turn(mInitialPosition, 0.05f) == false)
			SetPosition(CreaturePosition + dir / 10);
			
		
	}
}

void Creature::DoAngry()
{
	D3DXVECTOR3 CharacterPosition = pss->GetPosition();
	D3DXVECTOR3 CreaturePosition = GetPosition();

	Turn(CharacterPosition, 0.05f);
	SetPosition(CreaturePosition + (CharacterPosition - CreaturePosition) / 100);
}

void Creature::DoRage()
{
	RotationY(0.4f);
}