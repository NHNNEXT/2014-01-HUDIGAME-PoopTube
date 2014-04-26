/**
* @author 양현찬
* @brief
* 무서운 크리쳐
* @date 2014/04/22
* @file Creature.h
*/
#pragma once
#include "stdafx.h"
#include "Node.h"

namespace pooptube {
	class SkinnedMesh;
	class CollisionBox;
}

enum CREATURE_STATE {
	IDLE,
	CHASE,
	ATTACK,
};

class Creature : public pooptube::Node
{
public:
	Creature();
	~Creature();

	bool Init();

	void Render();
	void Update(float dTime);

	CREATURE_STATE GetState() const { return mState; }
	void Setstate(CREATURE_STATE state) { mState = state; }

	std::shared_ptr<pooptube::SkinnedMesh> GetSkinnedMesh() const { return mSkinnedMesh; }

private:
	CREATURE_STATE	mState = IDLE;

	float mSpeed = 0.1f;

	std::shared_ptr<pooptube::SkinnedMesh>	mSkinnedMesh = nullptr;
	std::shared_ptr<pooptube::CollisionBox> mCollisionBox = nullptr;
};

