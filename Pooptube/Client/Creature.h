/**
* @author 양현찬
* @version 2014/04/29 김지환 크리쳐의 지형에 따른 y값 변화
* @brief
* 무서운 크리쳐
* @date 2014/04/22
* @file Creature.h
*/

// 플레이어를 괴롭힐 크리쳐 클래스
// 플레이어가 접근하면 이를 감지하고 추격한다.
// 플레이어에게 접촉하면 괴롭힌다.(FSM 방식으로 구현한다)

#pragma once
#include "stdafx.h"
#include "Node.h"
#include "SoundManager.h"
#include "SkinnedMesh.h"

namespace pooptube {
	class CollisionBox;
	class Ground;
	class XMesh;
}
class StageOne;
class MainCharacter;

enum CREATURE_STATE : int {
	IDLE,
	ANGRY,
	RAGE,
	GOBACK,
};

class Creature : public pooptube::Node
{
public:
	MainCharacter *pss; // mainCharacter를 가리키는 임시 포인터
										// stageManager를 작성하면 지운다.
	Creature();
	~Creature();

	static Creature *Create();

	bool Init();

	void Render();
	void Update(float dTime);

	CREATURE_STATE FSM();

	void DoIdle(float dTime);
	void DoAngry();
	bool DoRage(float dTime);
	void DoGoBack();

	CREATURE_STATE GetState() const { return mState; }
	void SetState(CREATURE_STATE state) { mState = state; }

	void SetAttackRate(float attackRate) { mAttackRate = attackRate; }
	float GetAttackRate() { return mAttackRate; }

	int GetTotalDamage() { return mTotalDamage; }
	float GetSpeed() { return mRunSpeed; }

	void SetInitialPosition(D3DXVECTOR3 position) { mInitialPosition = position; }

	void IncreaseIdleDistance(int orbCount) { mIdleDistance = static_cast<float>(orbCount) * 1.5f + 16.f; }
	float GetIdleDistance() { return mIdleDistance; }

	virtual std::vector<D3DXVECTOR3> *GetVertices() { return mMesh->GetVertices(); }
	virtual std::vector<D3DXVECTOR3> *GetIndices() { return mMesh->GetIndices(); }

	//void IsAttackable();

private:
	void _CollsionHandle( pooptube::CollisionBox* collisionResult );

	CREATURE_STATE	mState = IDLE;

	float mIdleDistance = 16.0f;
	float mRageDistance = 3.5f;
	float mTurnSpeed = 0.2f;
	float mRunSpeed = 0.02f;
	float mDistanceFromMainCharater = 0;
	float mAttackRate = 1.0f;
	float mAttackTime = 0.f;
	int mAttackDamage = 1;
	int	mTotalDamage = 0;
	bool mAttackable = false;
	

	//float mTime = 0.f;
	D3DXVECTOR3 mInitialPosition = { 10, 10, 10 };

	pooptube::SkinnedMesh	*mMesh = nullptr;
	pooptube::SkinnedMesh	*mMesh2 = nullptr;
	pooptube::SkinnedMesh	*mMesh3 = nullptr;
	//pooptube::XMesh			*mXMesh = nullptr;

//	pooptube::CollisionBox *mCollisionBox = nullptr;
	FMOD::Studio::EventInstance *mAttackSound = nullptr;
	FMOD::Studio::EventInstance *mWalkSound = nullptr;
	FMOD::Studio::EventInstance *mAngrySound = nullptr;

	FMOD_3D_ATTRIBUTES		mSoundPos;
};

