#pragma once
#include "stdafx.h"
#include "Object.h"


namespace pooptube {

	class ResourceManager : public Object {
	public:
		ResourceManager();
		~ResourceManager();

		static ResourceManager* GetInstance();

	public:


	protected:
		bool _Init();


	private:

		static  ResourceManager* mInstance;

		//sdk에서 제공하는 FbxManager
		FbxManager*	mManager;
		std::map<std::string, FbxScene*> mFBXMeshTable;



	};

}