
#include "stdafx.h"
#include "ObjectManager.h"

namespace pooptube {
	ObjectManager* ObjectManager::mInstance = nullptr;
	ObjectManager* ObjectManager::GetInstance() {
		if ( mInstance == nullptr ) {
			mInstance = new ObjectManager();
		}

		return mInstance;
	}

	void ObjectManager::ReleaseInstance() {
		if ( mInstance != nullptr ) {
			delete mInstance;
			mInstance = nullptr;
		}
	}

	void ObjectManager::AddObject( Object* pObject ) {
		mSharedObjectList.push_front( pObject );
	}
	void ObjectManager::RemoveObject( Object* pObject ) {
		/*for (auto object : mSharedObjectList ) {
			if ( object == pObject ) {
				object.reset();
				//mSharedObjectList.erase_after(  );
			}
		}*/
		for (auto iter=mSharedObjectList.begin(); iter!=mSharedObjectList.end(); iter++ ) {
			if ( (*iter) == pObject ) {
				//(*iter).reset();
				mSharedObjectList.erase_after( iter );
				break;
			}
		}
	}

	ObjectManager::ObjectManager() {
	}
	ObjectManager::~ObjectManager() {
		/*for (auto& object : mSharedObjectList ) {
			object.reset();
		}*/
		mSharedObjectList.clear();
		// clear만 해도 다 지워지는 거 같은데.. (shared_ptr이 지역변수라 자동으로 소멸자 호출..)
	}
}