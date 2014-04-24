
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
		std::forward_list<std::shared_ptr<Object>> mSharedObjectList;
	};
}