/**
* @author 양현찬
* @brief pooptube 프레임워크의 뿌리가 되는 객체를 관리하는 풀
* 싱글톤
* 지금은 다른 싱글톤들하고 기타 오브젝트만 관리
* @date 2014/04/17
* @file ObjectManager.h
*/
#pragma once

#include "Object.h"

namespace pooptube {
	class ObjectManager {
	public:
		static ObjectManager* GetInstance();
		static void ReleaseInstance();

		void AddObject( Object* pObject );
		void RemoveObject( Object* pObject );
	private:
		ObjectManager();
		~ObjectManager();

	private:
		static ObjectManager* mInstance;
		std::forward_list<Object *> mSharedObjectList;
	};
}