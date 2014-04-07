
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
		mSharedObjectList.push_front( std::shared_ptr<Object>(pObject) );
	}
	void ObjectManager::RemoveObject( Object* pObject ) {
		/*for (auto object : mSharedObjectList ) {
			if ( object == pObject ) {
				object.reset();
				//mSharedObjectList.erase_after(  );
			}
		}*/
		for (auto iter=mSharedObjectList.begin(); iter!=mSharedObjectList.end(); iter++ ) {
			if ( (*iter).get() == pObject ) {
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
		// clear�� �ص� �� �������� �� ������.. (shared_ptr�� ���������� �ڵ����� �Ҹ��� ȣ��..)
	}
}