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
#include "MainCharacter.h"
#include "StageOne.h"
#include "Ground.h"

namespace pooptube {
	class SkinnedMesh;
	class CollisionBox;
	class Ground;
}
class StageOne;

enum CREATURE_STATE : int {
	IDLE,
	ANGRY,
	RAGE,
};

class Creature : public pooptube::Node
{
public:
	std::shared_ptr<MainCharacter> pss; // mainCharacter를 가리키는 임시 포인터
										// stageManager를 작성하면 지운다.
	Creature();
	~Creature();

	static std::shared_ptr<Creature> Create();

	bool Init( std::shared_ptr<Creature> pCreature );

	void Render();
	void Update(float dTime);

	CREATURE_STATE FSM();

	void DoIdle(float dTime);
	void DoAngry();
	void DoRage();

	CREATURE_STATE GetState() const { return mState; }
	void SetState(CREATURE_STATE state) { mState = state; }

	std::shared_ptr<pooptube::SkinnedMesh> GetSkinnedMesh() const { return mSkinnedMesh; }

private:
	CREATURE_STATE	mState = IDLE;

	float mIdleDistance = 8.f;
	float mRageDistance = 2.f;
	float mSpeed = 0.03f;
	float mDistanceFromMainCharater = 0;
	D3DXVECTOR3 mInitialPosition = { 10.f, 0.f, 10.f };

	std::shared_ptr<pooptube::SkinnedMesh>	mSkinnedMesh = nullptr;
	std::shared_ptr<pooptube::CollisionBox> mCollisionBox = nullptr;
};

