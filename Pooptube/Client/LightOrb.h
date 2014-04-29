/**
* @author ����ȯ
* @version 
* @brief
* ���������� �����ϴ� ��ü��
* @date 2014/04/29
* @file LightOrb.h
*/

// �÷��̾ ȹ���ϴ� '���� ����', ����, ���� ���� �����ϴ� Ŭ����

#pragma once
#include "stdafx.h"
#include "Node.h"
#include "MainCharacter.h"
#include "StageOne.h"
#include "Ground.h"
#include "CollisionManager.h"

namespace pooptube {
	class SkinnedMesh;
	class CollisionBox;
	class Ground;
}
class StageOne;

class LightOrb : public pooptube::Node
{
public:
	LightOrb();
	~LightOrb();

	static std::shared_ptr<LightOrb> Create();

	bool Init(std::shared_ptr<LightOrb> pCreature);

	void Render();
	void Update(float dTime);

	std::shared_ptr<pooptube::SkinnedMesh> GetSkinnedMesh() const { return mSkinnedMesh; }

private:
	D3DXVECTOR3 mInitialPosition = { 10.f, 0.f, 0.f };

	std::shared_ptr<pooptube::SkinnedMesh>	mSkinnedMesh = nullptr;
	std::shared_ptr<pooptube::CollisionBox> mCollisionBox = nullptr;
};