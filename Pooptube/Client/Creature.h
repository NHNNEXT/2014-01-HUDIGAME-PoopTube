/**
* @author 양현찬
* @brief
* 무서운 크리쳐
* @date 2014/04/22
* @file Creature.h
*/
#include "Node.h"

#pragma once
class Creature : public pooptube::Node
{
public:
	Creature();
	virtual ~Creature();

	/*bool Init();

	void Render();
	void Update(float dTime);*/

private:
	float mSpeed = 0.1f;

};

