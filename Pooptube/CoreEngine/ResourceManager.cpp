
#include "stdafx.h"
#include "ResourceManager.h"

namespace pooptube {

	//singletone
	ResourceManager* ResourceManager::mInstance = nullptr;
	ResourceManager* ResourceManager::GetInstance() {
		if (mInstance == nullptr) {
			mInstance = new ResourceManager();
			if (mInstance->_Init())
				ObjectManager::GetInstance()->AddObject(mInstance);
			else {
				delete mInstance;
				mInstance = nullptr;
			}
		}

		return mInstance;
	}

	ResourceManager::ResourceManager() {

	}

	ResourceManager::~ResourceManager() {

	}



	bool ResourceManager::_Init() {
		return true;
	}






}