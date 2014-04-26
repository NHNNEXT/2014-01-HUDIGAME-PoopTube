#include "stdafx.h"
#include "Ground.h"
#include "HeightMap.h"

Ground::Ground() {
}


Ground::~Ground() {
}

std::shared_ptr<Ground> Ground::Create() {
	std::shared_ptr<Ground> pGround(new Ground);
	if (pGround->Init())
		return pGround;
	else
		return nullptr;

}

void Ground::Render() {
	mHeightMap->Render();
}

void Ground::Update(float dTime) {
	mHeightMap->Update(dTime);
}

bool Ground::Init() {
	if (!Node::Init())
		return false;
	
	mHeightMap = pooptube::HeightMap::Create("test.bmp");

	return true;
}

float Ground::GetHeight(float x, float z) {
	return mHeightMap->GetHeight(x, z);
}


